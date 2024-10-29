/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) System
** ════════════════════════════════════════════════════════════════════════════
**  File        : VoIPAudioHandler.hpp
**  Create at   : 2024-10-29 12:10
**  Author      : AUTHOR
**  Description : This system is dedicated to the DriverEngine, it captures the sound
                    from the system, likely the microphone.
** ═══════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "module/raylib_safe.h"

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include <thread>
#include <atomic>
#include <vector>

namespace gengine::system::driver::input {

class VoIPAudioHandler : public gengine::System<VoIPAudioHandler>, public LocalSystem {
public:
    ~VoIPAudioHandler() override;

    void init(void) override;

    void onCapture(gengine::system::driver::input::KeyVEvent &e);

    bool isCapturing(void) {
        return m_capturing;
    }
    auto &getRecorderBuffer(void) {
        return m_captureBuffer;
    }

private:
    void processSoundInput(void);

    std::atomic_bool m_capturing = false;
    std::atomic_bool m_running = false;
    std::thread m_soundThread;

    std::vector<float> m_captureBuffer;
};

} // namespace gengine::system::driver::input
