/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** record
*/

#pragma once

#include <string>

namespace Network::Event {
struct RecordInfo {

    enum Mode { RECORD, WATCH, STOP };

    std::string demoFile;
    Mode mode;

    RecordInfo(Mode recordMode)
        : mode(recordMode) {
    }
};
} // namespace Network::Event
