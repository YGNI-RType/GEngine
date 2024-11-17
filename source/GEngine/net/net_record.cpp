/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_record
*/

#include "GEngine/net/net_record.hpp"
#include "GEngine/net/net.hpp"
#include "GEngine/net/net_exception.hpp"

#include <bzlib.h>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>
#include <vector>

namespace Network {
void NetRecord::init(void) {
    m_enabled = true;

#ifdef _WIN32
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::ifstream execFile(exePath, std::ios::binary);
#else
    std::ifstream execFile("/proc/self/exe", std::ios::binary);
#endif

#if 0
    if (!execFile.is_open())
        throw NetException("Failed to open executable file for hashing", EL_ERR_RECORDING);
    std::vector<char> execBuffer((std::istreambuf_iterator<char>(execFile)), std::istreambuf_iterator<char>());
    m_execHash = std::hash<std::string>{}(std::string(execBuffer.begin(), execBuffer.end()));
#else
    m_execHash = 0;
#endif

#ifdef _WIN32
    m_recordFilePath = "C:\\Windows\\Temp\\record.bin";
#else
    m_recordFilePath = "/tmp/record.bin";
#endif

    // Check if the file already exists and change the name if it does
    size_t counter = 1;
    const std::string ogRecordFilePath = m_recordFilePath;
    while (std::filesystem::exists(m_recordFilePath)) {
        m_recordFilePath = ogRecordFilePath + "." + std::to_string(counter);
        counter++;
    }
}

void NetRecord::update(const AMessage &msg) {
    if (!isRecording())
        return;

    if (!m_hasFirstUpdate) {
        if (!msg.isFullAck())
            return;
        m_hasFirstUpdate = true;
    }

    uint32_t dt;
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::high_resolution_clock::now().time_since_epoch())
                   .count();
    dt = m_lastWriteTime == 0 ? 0 : now - m_lastWriteTime;
    m_lastWriteTime = now;

    m_fs.write(reinterpret_cast<const char *>(&dt), sizeof(dt));

    auto msgType = msg.getType();
    auto msgFlags = msg.getFlags();
    auto msgSize = msg.getSize();
    auto msgData = msg.getData();
    auto msgClassType = msg.getClassType();
    m_fs.write(reinterpret_cast<const char *>(&msgClassType), sizeof(msgClassType));
    m_fs.write(reinterpret_cast<const char *>(&msgType), sizeof(msgType));
    m_fs.write(reinterpret_cast<const char *>(&msgFlags), sizeof(msgFlags));
    m_fs.write(reinterpret_cast<const char *>(&msgSize), sizeof(msgSize));
    m_fs.write(reinterpret_cast<const char *>(msgData), msgSize);

    // std::cout << "updating record: " << dt << ", size: " << msgSize << std::endl;
}

bool NetRecord::read(SerializedMessage &msgBuffer, uint8_t &classType, uint32_t &sleepDuration) {
    if (!m_fs.read((char *)&sleepDuration, sizeof(sleepDuration)))
        return false;
    if ((int64_t)sleepDuration == -1)
        return false;

    if (!m_fs.read(reinterpret_cast<char *>(&classType), sizeof(classType)))
        return false;
    if (!m_fs.read(reinterpret_cast<char *>(&msgBuffer.type), sizeof(msgBuffer.type)))
        return false;
    if (!m_fs.read(reinterpret_cast<char *>(&msgBuffer.flag), sizeof(msgBuffer.flag)))
        return false;
    if (!m_fs.read(reinterpret_cast<char *>(&msgBuffer.curSize), sizeof(msgBuffer.curSize)))
        return false;
    if (!m_fs.read(reinterpret_cast<char *>(msgBuffer.data), msgBuffer.curSize))
        return false;
    return true;
}

bool NetRecord::startRecord(void) {
    if (isRecording() || isWatching() || m_recordingCompressed)
        return false;

    std::cout << "Started recording..." << std::endl;
    m_fs.open(m_recordFilePath, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!m_fs.is_open())
        throw NetException("Failed to open record file. Abort...", EL_ERR_RECORDING);
    m_fs.write(reinterpret_cast<const char *>(&MAGIC_NUMBER), sizeof(MAGIC_NUMBER));
    m_fs.write(reinterpret_cast<const char *>(&m_execHash), sizeof(m_execHash));
    m_recording = true;
    m_hasFirstUpdate = false;
    return true;
}

bool NetRecord::endRecord(bool isDestructing) {
    if (!isRecording() || isWatching() || !m_fs.is_open())
        return false;

    uint32_t end = -1;
    m_fs.write(reinterpret_cast<const char *>(&end), sizeof(end));
    m_fs.close();

    m_recording = false;
    m_recordingCompressed = true;

    std::cout << "Ended record demo file" << std::endl;

    if (!isDestructing)
        m_compressionThread = std::thread([this]() { compressFile(); });
    else
        compressFile();

    return true;
}

void NetRecord::compressFile(void) {
    std::cout << "Compressing the demo file..." << std::endl;

    std::ifstream fs(m_recordFilePath, std::ios::in | std::ios::binary);
    if (!fs.is_open())
        throw NetException("Failed to open record file for compression", EL_ERR_RECORDING);

    /* Compress the file content after the magic number and the hash */
    fs.seekg(sizeof(MAGIC_NUMBER) + sizeof(std::size_t), std::ios::beg);
    std::vector<char> buffer((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

    uint32_t compressedSize = buffer.size() * 1.01 + 600;
    std::vector<char> compressedBuffer(compressedSize);

    int result =
        BZ2_bzBuffToBuffCompress(compressedBuffer.data(), &compressedSize, buffer.data(), buffer.size(), 9, 0, 30);
    if (result != BZ_OK)
        throw NetException("Failed to compress data", EL_ERR_RECORDING);

    fs.close();
    std::filesystem::create_directory("demos");
    std::ofstream demoFile("demos/demo_" + std::to_string(std::time(nullptr)), std::ios::out | std::ios::binary);
    if (!demoFile.is_open())
        throw NetException("Failed to open compressed file for writing", EL_ERR_RECORDING);

    demoFile.write(reinterpret_cast<const char *>(&MAGIC_NUMBER), sizeof(MAGIC_NUMBER));
    demoFile.write(reinterpret_cast<const char *>(&m_execHash), sizeof(m_execHash));
    demoFile.write(compressedBuffer.data(), compressedSize);
    demoFile.close();

    std::cout << "Done compressing the demo file !" << std::endl;

    if (std::remove(m_recordFilePath.c_str()) != 0)
        std::cerr << "Error deleting temporary record file: " << m_recordFilePath << std::endl;

    m_recordingCompressed = false;
}

void NetRecord::openFile(const std::string &filename, bool checkHash) {
    m_fs.open(filename, std::ios::in | std::ios::binary);
    if (!m_fs.is_open())
        throw NetException("Failed to open record file", EL_ERR_RECORDING);

    uint64_t magicNumber;
    m_fs.read(reinterpret_cast<char *>(&magicNumber), sizeof(magicNumber));
    if (magicNumber != MAGIC_NUMBER)
        throw NetException("Invalid magic number in record file", EL_ERR_RECORDING);

    if (checkHash) {
        std::size_t execHash;
        m_fs.read(reinterpret_cast<char *>(&execHash), sizeof(execHash));
        if (execHash != m_execHash)
            throw NetException("Invalid executable hash in record file", EL_ERR_RECORDING);
    }

    m_fs.seekg(sizeof(MAGIC_NUMBER) + sizeof(std::size_t), std::ios::beg);
    std::vector<char> compressedBuffer((std::istreambuf_iterator<char>(m_fs)), std::istreambuf_iterator<char>());
    uint32_t decompressedSize = compressedBuffer.size() * 10; // Estimate decompressed size
    std::vector<char> decompressedBuffer(decompressedSize);

    int result = BZ2_bzBuffToBuffDecompress(decompressedBuffer.data(), &decompressedSize, compressedBuffer.data(),
                                            compressedBuffer.size(), 0, 0);
    if (result != BZ_OK)
        throw NetException("Failed to decompress data", EL_ERR_RECORDING);

    // Resize the buffer to the actual decompressed size
    decompressedBuffer.resize(decompressedSize);

    // Replace the file stream with the decompressed data
    m_fs.close();
    m_fs.clear();

    // Create a temporary file in a cross-platform way
#ifdef _WIN32
    char tempPath[MAX_PATH];
    GetTempPath(MAX_PATH, tempPath);
    m_decompressFilePath = std::string(tempPath) + "decompressed_record_" + std::to_string(std::time(nullptr)) + ".bin";
#else
    m_decompressFilePath = "/tmp/decompressed_record_" + std::to_string(std::time(nullptr)) + ".bin";
#endif

    std::ofstream tempFile(m_decompressFilePath, std::ios::out | std::ios::binary);
    if (!tempFile.is_open())
        throw NetException("Failed to open temporary file for writing", EL_ERR_RECORDING);

    tempFile.write(decompressedBuffer.data(), decompressedBuffer.size());
    tempFile.close();

    m_fs.open(m_decompressFilePath, std::ios::in | std::ios::binary);
    if (!m_fs.is_open())
        throw NetException("Failed to open temporary file for reading", EL_ERR_RECORDING);

    m_fs.seekg(0, std::ios::beg);
}

bool NetRecord::startWatch(const std::string &filePath) {
    if (isRecording() || isWatching())
        return false;

    try {
        openFile(filePath);
    } catch (const std::exception &e) {
        std::cerr << "Failed to open record file: " << e.what() << std::endl;
        return false;
    }
    std::cout << "Started demo watch..." << std::endl;
    m_watching = true;
    return true;
}

bool NetRecord::updateWatch(SerializedMessage &msg, uint8_t classType, uint32_t sleepDuration) {
    if (sleepDuration > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
    auto &client = NET::getClient();

    if (!m_watching || !m_alive)
        return false;

    if (classType == 'T') {
        TCPMessage tcpMsg(0);
        tcpMsg.setSerialize(reinterpret_cast<const TCPSerializedMessage &>(msg));
        return client.handleServerTCP(tcpMsg);
    } else if (classType == 'U') {
        UDPMessage udpMsg(0, 0);
        size_t readCount = 0;
        udpMsg.setSerialize(reinterpret_cast<const UDPSerializedMessage &>(msg));

        if (udpMsg.hasHeader())
            readCount += sizeof(UDPG_NetChannelHeader);
        return client.handleServerUDPSanitized(udpMsg, readCount);
    }
    return false;
}

/* this is in a seperated thread */
void NetRecord::watchLoop(void) {
    if (!isWatching())
        return;

    while (m_watching) {
        SerializedMessage msg;
        uint8_t classType;
        uint32_t sleepDuration;

        if (!read(msg, classType, sleepDuration))
            break;

        if (!updateWatch(msg, classType, sleepDuration))
            break;
    }
    endWatch(false);
}

void NetRecord::startWatchThread(void) {
    m_watchThread = std::thread([this]() {
        while (true) {
            std::unique_lock<std::mutex> lock(m_watchMutex);
            m_watchCV.wait(lock, [this]() { return m_watchThreadRunning; });
            if (!m_alive)
                break;
            m_watchThreadRunning = false;
            watchLoop();
        }
    });
}

void NetRecord::startWatchFakeNet(void) {
    if (!isWatching())
        return;

    {
        std::lock_guard<std::mutex> lock(m_watchMutex);
        m_watchThreadRunning = true;
    }
    m_watchCV.notify_all();
}

bool NetRecord::endWatch(bool wanted) {
    if (!isWatching())
        return false;

    if (m_fs.is_open())
        m_fs.close();

    m_watching = false;
    std::cout << "Demo ended." << std::endl;
    if (std::remove(m_decompressFilePath.c_str()) != 0)
        std::cerr << "Error deleting temporary decompressed record file: " << m_decompressFilePath << std::endl;

    if (!wanted) {
        auto &em = NET::getEventManager();
        em.pushResult(EL_RECORDING_END, false);
    }
    return true;
}

NetRecord::~NetRecord() {
    if (isRecording())
        endRecord(true);
    else if (isWatching())
        endWatch(true);

    if (m_fs.is_open())
        m_fs.close();

    if (m_watchThread.joinable()) {
        m_alive = false;
        {
            std::lock_guard<std::mutex> lock(m_watchMutex);
            m_watchThreadRunning = true;
        }
        m_watchCV.notify_one();
        m_watchThread.join();
    }
    if (m_compressionThread.joinable())
        m_compressionThread.join();
}
} // namespace Network
