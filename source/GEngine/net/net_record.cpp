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
NetRecord::NetRecord(bool shouldWrite, const std::string &demoFilePath){
    std::ifstream execFile("/proc/self/exe", std::ios::binary);
    if (!execFile.is_open())
        throw std::runtime_error("Failed to open executable file for hashing");
    std::vector<char> execBuffer((std::istreambuf_iterator<char>(execFile)), std::istreambuf_iterator<char>());
    m_execHash = std::hash<std::string>{}(std::string(execBuffer.begin(), execBuffer.end()));

    #ifdef _WIN32
        fs.open("C:\\Windows\\Temp\\record.bin", std::ios::in | std::ios::binary);
    #else
        fs.open("/tmp/record.bin", std::ios::in | std::ios::binary);
    #endif
    if (!fs.is_open())
        throw std::runtime_error("Failed to open record file");

    if (!shouldWrite) {
        openFile(demoFilePath);
        return;
    }
}

bool NetRecord::startRecord(void) {
    if (isRecording() || isWatching())
        return false;

    fs.write(reinterpret_cast<const char *>(&MAGIC_NUMBER), sizeof(MAGIC_NUMBER));
    fs.write(reinterpret_cast<const char *>(&m_execHash), sizeof(m_execHash));
    m_recording = true;
    return true;
}

bool NetRecord::endRecord(void) {
    if (!isRecording() || isWatching())
        return false;

    size_t end = -1;
    fs.write(reinterpret_cast<const char *>(&end), sizeof(end));

    std::cout << "Ended record demo file" << std::endl;

    std::thread compressionThread([this]() {
        std::cout << "Compressing the demo file..." << std::endl;

        /* Compress the file content after the magic number and the hash */
        fs.seekg(sizeof(MAGIC_NUMBER) + sizeof(std::size_t), std::ios::beg);
        std::vector<char> buffer((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

        uint32_t compressedSize = buffer.size() * 1.01 + 600;
        std::vector<char> compressedBuffer(compressedSize);

        int result =
            BZ2_bzBuffToBuffCompress(compressedBuffer.data(), &compressedSize, buffer.data(), buffer.size(), 9, 0, 30);
        if (result != BZ_OK)
            throw std::runtime_error("Failed to compress data");

        std::filesystem::create_directory("demos");
        std::ofstream demoFile("demos/demo_" + std::to_string(std::time(nullptr)), std::ios::out | std::ios::binary);
        if (!demoFile.is_open())
            throw std::runtime_error("Failed to open compressed file for writing");

        demoFile.write(compressedBuffer.data(), compressedSize);
        demoFile.close();

        m_recording = false;
        std::cout << "Done compressing the demo file !" << std::endl;
    });
    compressionThread.detach();

    return true;
}

void NetRecord::openFile(const std::string &filename) {
    fs.open(filename, std::ios::out | std::ios::binary);
    if (!fs.is_open())
        throw std::runtime_error("Failed to open record file");

    uint32_t magicNumber;
    fs.read(reinterpret_cast<char *>(&magicNumber), sizeof(magicNumber));
    if (magicNumber != MAGIC_NUMBER)
        throw std::runtime_error("Invalid magic number in record file");

    std::size_t execHash;
    fs.read(reinterpret_cast<char *>(&execHash), sizeof(execHash));
    if (execHash != m_execHash)
        throw std::runtime_error("Invalid executable hash in record file");

    fs.seekg(sizeof(MAGIC_NUMBER) + sizeof(std::size_t), std::ios::beg);
    std::vector<char> compressedBuffer((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    uint32_t decompressedSize = compressedBuffer.size() * 10; // Estimate decompressed size
    std::vector<char> decompressedBuffer(decompressedSize);

    int result = BZ2_bzBuffToBuffDecompress(decompressedBuffer.data(), &decompressedSize, compressedBuffer.data(), compressedBuffer.size(), 0, 0);
    if (result != BZ_OK)
        throw std::runtime_error("Failed to decompress data");

    // Resize the buffer to the actual decompressed size
    decompressedBuffer.resize(decompressedSize);

    // Replace the file stream with the decompressed data
    fs.close();
    fs.clear();
    fs.open("/dev/null", std::ios::in | std::ios::out | std::ios::binary);
    fs.write(decompressedBuffer.data(), decompressedBuffer.size());
    fs.seekg(0, std::ios::beg);
    m_watching = true;

}

void NetRecord::updateWatch() {
    if (!isWatching())
        return;

    size_t sleepDuration;
    while (read(nullptr, sleepDuration))
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
}

void NetRecord::write(const void *data, size_t size) {
    fs.write((const char *)&size, sizeof(size));
    fs.write(reinterpret_cast<const char *>(data), size);
}

bool NetRecord::read(void *data, size_t &sleepDuration) {
    size_t size;
    fs.read((char *)&size, sizeof(size));
    if ((int64_t)size == -1)
        return false;

    fs.read((char *)&sleepDuration, sizeof(size));
    fs.read(reinterpret_cast<char *>(data), size);
    return true;
}

NetRecord::~NetRecord() {
    if (fs.is_open())
        fs.close();
}
} // namespace Network
