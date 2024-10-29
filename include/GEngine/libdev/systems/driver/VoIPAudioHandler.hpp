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

#include <atomic>
#include <vector>

namespace gengine::system::driver::input {

struct RecorderBuffer {
    std::vector<uint8_t> buffer;
    size_t offset;
};

class VoIPAudioHandler : public gengine::System<VoIPAudioHandler>, public LocalSystem {
public:
    ~VoIPAudioHandler() override;

    void init(void) override;

    void onCapture(gengine::system::driver::input::KeyVEvent &e);

    bool isCapturing(void) {
        return m_capturing;
    }
    RecorderBuffer &getRecorderBuffer(void) {
        return m_recorderBuffer;
    }
    RecorderBuffer &getRecorderBufferOutput(void) {
        return m_recorderBufferOutput;
    }

private:
    void processSoundInput(void);

    RecorderBuffer m_recorderBuffer;
    RecorderBuffer m_recorderBufferOutput;
    std::atomic_bool m_capturing = false;
};

} // namespace gengine::system::driver::input
