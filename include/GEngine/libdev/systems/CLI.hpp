/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) System
** ════════════════════════════════════════════════════════════════════════════
**  File        : cli.hpp
**  Create at   : 2024-10-28 15:39
**  Author      : b3w
**  Description : DESCRIPTION
** ═══════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/systems/events/MainLoop.hpp"
#include "GEngine/libdev/systems/events/CLI.hpp"

#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <chrono>
#include <vector>

namespace gengine::system {
class CLI: public gengine::System<CLI> {
public:
    CLI();
    void init(void) override;
    void onStartEngine(gengine::system::event::StartEngine &);
    void onStopEngine(gengine::system::event::StopEngine &);
    void onMainLoop(gengine::system::event::MainLoop &);

private:
    void getInputs(void);
    std::vector<std::string> splitInput(const std::string& input);
    std::thread inputThread;
    std::atomic<bool> stopReading;
    std::vector<std::string> userInputHistory;
    std::mutex historyMutex;
};
}
