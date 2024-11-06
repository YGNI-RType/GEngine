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

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"
#include "GEngine/libdev/systems/events/MainLoop.hpp"
#include "GEngine/libdev/systems/events/driver/input/VoIP.hpp"

#include <vector>

namespace gengine::system::driver::input {

class VoIPAudioCatcher : public gengine::System<VoIPAudioCatcher>, public LocalSystem {
public:
    ~VoIPAudioCatcher() override;

    void init(void) override;

    void onMainLoop(gengine::system::event::MainLoop &e);
    void onStartVoIP(gengine::system::event::driver::input::StartVoIP &);
    void onEndVoIP(gengine::system::event::driver::input::EndVoIP &);

    bool isCapturing(void) {
        return m_capturing;
    }
    auto &getRecorderBuffer(void) {
        return m_captureBuffer;
    }

private:
    bool m_capturing = false;
    size_t m_sampleRate = 16000;
    size_t m_numChannel = 1;

    std::vector<uint8_t> m_captureBuffer; /* your buffer */
};

} // namespace gengine::system::driver::input
