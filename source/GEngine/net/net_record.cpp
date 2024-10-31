/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_record
*/

#include "GEngine/net/net_record.hpp"

#include <bzlib.h>
#include <filesystem>
#include <iostream>
#include <thread>
#include <vector>

namespace Network {
NetRecord::NetRecord(bool shouldWrite, const std::string &demoFilePath)
    : m_enabled(true) {
    init(shouldWrite, demoFilePath);
}

void NetRecord::init(bool shouldWrite, const std::string &demoFilePath) {
    m_enabled = true;

#ifdef _WIN32
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    std::ifstream execFile(exePath, std::ios::binary);
#else
    std::ifstream execFile("/proc/self/exe", std::ios::binary);
#endif
    if (!execFile.is_open())
        throw std::runtime_error("Failed to open executable file for hashing");
    std::vector<char> execBuffer((std::istreambuf_iterator<char>(execFile)), std::istreambuf_iterator<char>());
    m_execHash = std::hash<std::string>{}(std::string(execBuffer.begin(), execBuffer.end()));

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

    if (!shouldWrite) {
        openFile(demoFilePath);
        return;
    } else {
        std::cout << "Started recording..." << std::endl;
        m_fs.open(m_recordFilePath, std::ios::out | std::ios::binary);
        if (!m_fs.is_open())
            throw std::runtime_error("Failed to open record file");
    }
}

bool NetRecord::startRecord(void) {
    if (isRecording() || isWatching() || !m_fs.is_open())
        return false;

    m_fs.write(reinterpret_cast<const char *>(&MAGIC_NUMBER), sizeof(MAGIC_NUMBER));
    m_fs.write(reinterpret_cast<const char *>(&m_execHash), sizeof(m_execHash));
    m_recording = true;
    return true;
}

bool NetRecord::endRecord(void) {
    if (!isRecording() || isWatching() || !m_fs.is_open())
        return false;

    uint32_t end = -1;
    m_fs.write(reinterpret_cast<const char *>(&end), sizeof(end));

    m_fs.close();

    std::cout << "Ended record demo file" << std::endl;

    std::thread compressionThread([this]() {
        std::cout << "Compressing the demo file..." << std::endl;

        std::ifstream fs(m_recordFilePath, std::ios::in | std::ios::binary);
        if (!fs.is_open())
            throw std::runtime_error("Failed to open record file for compression");

        /* Compress the file content after the magic number and the hash */
        fs.seekg(sizeof(MAGIC_NUMBER) + sizeof(std::size_t), std::ios::beg);
        std::vector<char> buffer((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

        uint32_t compressedSize = buffer.size() * 1.01 + 600;
        std::vector<char> compressedBuffer(compressedSize);

        int result =
            BZ2_bzBuffToBuffCompress(compressedBuffer.data(), &compressedSize, buffer.data(), buffer.size(), 9, 0, 30);
        if (result != BZ_OK)
            throw std::runtime_error("Failed to compress data");

        fs.close();
        std::filesystem::create_directory("demos");
        std::ofstream demoFile("demos/demo_" + std::to_string(std::time(nullptr)), std::ios::out | std::ios::binary);
        if (!demoFile.is_open())
            throw std::runtime_error("Failed to open compressed file for writing");

        demoFile.write(reinterpret_cast<const char *>(&MAGIC_NUMBER), sizeof(MAGIC_NUMBER));
        demoFile.write(reinterpret_cast<const char *>(&m_execHash), sizeof(m_execHash));
        demoFile.write(compressedBuffer.data(), compressedSize);
        demoFile.close();

        m_recording = false;
        std::cout << "Done compressing the demo file !" << std::endl;
    });
    compressionThread.detach();

    return true;
}

void NetRecord::openFile(const std::string &filename) {
    m_fs.open(filename, std::ios::out | std::ios::binary);
    if (!m_fs.is_open())
        throw std::runtime_error("Failed to open record file");

    uint32_t magicNumber;
    m_fs.read(reinterpret_cast<char *>(&magicNumber), sizeof(magicNumber));
    if (magicNumber != MAGIC_NUMBER)
        throw std::runtime_error("Invalid magic number in record file");

    std::size_t execHash;
    m_fs.read(reinterpret_cast<char *>(&execHash), sizeof(execHash));
    if (execHash != m_execHash)
        throw std::runtime_error("Invalid executable hash in record file");

    m_fs.seekg(sizeof(MAGIC_NUMBER) + sizeof(std::size_t), std::ios::beg);
    std::vector<char> compressedBuffer((std::istreambuf_iterator<char>(m_fs)), std::istreambuf_iterator<char>());
    uint32_t decompressedSize = compressedBuffer.size() * 10; // Estimate decompressed size
    std::vector<char> decompressedBuffer(decompressedSize);

    int result = BZ2_bzBuffToBuffDecompress(decompressedBuffer.data(), &decompressedSize, compressedBuffer.data(),
                                            compressedBuffer.size(), 0, 0);
    if (result != BZ_OK)
        throw std::runtime_error("Failed to decompress data");

    // Resize the buffer to the actual decompressed size
    decompressedBuffer.resize(decompressedSize);

    // Replace the file stream with the decompressed data
    m_fs.close();
    m_fs.clear();

    // Create a temporary file in a cross-platform way
    std::string tempFilePath;
#ifdef _WIN32
    char tempPath[MAX_PATH];
    GetTempPath(MAX_PATH, tempPath);
    tempFilePath = std::string(tempPath) + "decompressed_record_" + std::to_string(std::time(nullptr)) + ".bin";
#else
    tempFilePath = "/tmp/decompressed_record_" + std::to_string(std::time(nullptr)) + ".bin";
#endif

    std::ofstream tempFile(tempFilePath, std::ios::out | std::ios::binary);
    if (!tempFile.is_open())
        throw std::runtime_error("Failed to open temporary file for writing");

    tempFile.write(decompressedBuffer.data(), decompressedBuffer.size());
    tempFile.close();

    m_fs.open(tempFilePath, std::ios::in | std::ios::binary);
    if (!m_fs.is_open())
        throw std::runtime_error("Failed to open temporary file for reading");

    m_fs.seekg(0, std::ios::beg);
    m_watching = true;
}

void NetRecord::updateWatch() {
    if (!isWatching())
        return;

    uint32_t sleepDuration;
    while (read(nullptr, sleepDuration))
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
}

void NetRecord::write(const void *data, uint32_t size) {
    m_fs.write((const char *)&size, sizeof(size));
    m_fs.write(reinterpret_cast<const char *>(data), size);
}

bool NetRecord::read(void *data, uint32_t &sleepDuration) {
    uint32_t size;
    m_fs.read((char *)&size, sizeof(size));
    if ((int64_t)size == -1)
        return false;

    m_fs.read((char *)&sleepDuration, sizeof(size));
    m_fs.read(reinterpret_cast<char *>(data), size);
    return true;
}

NetRecord::~NetRecord() {
    if (m_fs.is_open())
        m_fs.close();
}
} // namespace Network
