/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** net_record
*/

#pragma once

#include <fstream>
#include <atomic>

namespace Network
{
    constexpr uint64_t MAGIC_NUMBER = 0x716971686977;

    /*
    Magic Number : 0x716971686977
    hash of binary (so we close if the binary is a different one...)
    <content compressed with glibz>
    */
    class NetRecord {
        public:
            NetRecord() = default;
            NetRecord(bool shouldWrite, const std::string &demoFile = "");
            ~NetRecord();

            /* force the server to send fullsnapshot */
            bool startRecord(void);
            bool endRecord(void);

            void updateWatch();
        private:
            void write(const void *data, size_t size);
            bool read(void *data, size_t &sleepDuration);

            bool isWatching(void) const { return m_watching; }
            bool isRecording(void) const { return m_recording; }

            void openFile(const std::string &filename);

        private:
            std::fstream fs;
            std::atomic_bool m_recording = false;
            std::atomic_bool m_watching = false;

            size_t m_execHash;

    };
} // namespace Network
