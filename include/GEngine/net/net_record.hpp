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
/* = GEGDEM */
constexpr uint64_t MAGIC_NUMBER = 0x4d4544474547;

/*
Content of the file :
Magic Number : 0x47454744454d
hash of binary (so we close if the binary is a different one...)
<content compressed with bzip2>
Time to wait : size_t in ms or -1 => end of file
Msgtype : UDP or TCP
MsgSize : size_t
MsgType : uint8_t
MsgFlag : uint8_t
MsgData : MsgSize
<content compressed with bzip2>
*/

/**
 * @class NetRecord
 * @brief Manages network recording and watching functionalities.
 *
 * The NetRecord class provides methods to start and stop recording network data,
 * as well as to watch recorded data. It supports multi-threaded operations for
 * watching and compressing recorded data. Te recorded data segmentation is defined above.
 */
class NetRecord {
public:
    NetRecord() = default;
    ~NetRecord();

    void init(void);

    /* force the server to send fullsnapshot */
    bool startRecord(void);
    bool endRecord(bool isDestructing = false);

    bool startWatch(const std::string &filePath);
    bool endWatch(bool wanted);
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

    void compressFile(void);

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
