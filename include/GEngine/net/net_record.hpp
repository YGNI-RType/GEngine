/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_record
*/

#pragma once

#include "msg.hpp"

#include <atomic>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <thread>

namespace Network {
constexpr uint64_t MAGIC_NUMBER = 0x4d4544474547;

/*
Magic Number : 0x47454744454d
hash of binary (so we close if the binary is a different one...)
<content compressed with glibz>
*/
class NetRecord {
public:
    NetRecord() = default;
    ~NetRecord();

    void init(void);

    /* force the server to send fullsnapshot */
    bool startRecord(void);
    bool endRecord(void);

    bool startWatch(const std::string &filePath);
    bool endWatch(void);
    void startWatchThread(void);
    void startWatchFakeNet(void);

    void update(const AMessage &msg);
    void watchLoop(void);

    bool isEnabled(void) const {
        return m_enabled;
    }
    bool isRecording(void) const {
        return m_recording;
    }
    bool isWatching(void) const {
        return m_watching;
    }

private:
    bool updateWatch(SerializedMessage &msg, uint8_t type, uint32_t sleepDuration);

    bool read(SerializedMessage &msg, uint8_t &type, uint32_t &sleepDuration);

    void openFile(const std::string &filename, bool checkHash = false);

private:
    std::fstream m_fs;
    bool m_enabled = false;
    bool m_alive = true;
    std::atomic_bool m_recording = false;
    std::atomic_bool m_recordingCompressed = false;
    std::atomic_bool m_watching = false;
    bool m_watchThreadRunning = false;

    std::thread m_watchThread;
    std::thread m_compressionThread;
    std::mutex m_watchMutex;
    std::condition_variable m_watchCV;

    std::string m_recordFilePath;
    std::string m_decompressFilePath;

    size_t m_execHash;

    uint32_t m_lastWriteTime = 0;
    bool m_hasFirstUpdate;
};
} // namespace Network
