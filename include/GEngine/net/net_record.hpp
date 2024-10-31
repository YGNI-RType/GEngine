/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_record
*/

#pragma once

#include <atomic>
#include <fstream>

#include "msg.hpp"

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
    NetRecord(bool shouldWrite, const std::string &demoFile = "");
    ~NetRecord();

    void init(bool shouldWrite, const std::string &demoFile = "");

    /* force the server to send fullsnapshot */
    bool startRecord(void);
    bool endRecord(void);

    void update(const AMessage &msg);

    void updateWatch();

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
    void write(const void *data, uint32_t size);
    bool read(void *data, uint32_t &sleepDuration);

    void openFile(const std::string &filename);

private:
    std::fstream m_fs;
    bool m_enabled = false;
    std::atomic_bool m_recording = false;
    std::atomic_bool m_watching = false;

    std::string m_recordFilePath;

    size_t m_execHash;

    uint32_t m_lastWriteTime = 0;
    bool m_hasFirstUpdate;
};
} // namespace Network
