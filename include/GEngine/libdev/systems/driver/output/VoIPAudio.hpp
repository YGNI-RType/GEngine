/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) System
** ════════════════════════════════════════════════════════════════════════════
**  File        : VoIPAudio.hpp
**  Create at   : 2024-10-29 12:10
**  Author      : AUTHOR
**  Description : This system is dedicated to the DriverEngine, it output the sound
                    from the system, likely the microphone of the other users.
** ═══════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/System.hpp"
#include "GEngine/libdev/systems/events/MainLoop.hpp"
#include "GEngine/libdev/systems/events/Native.hpp"

#include <atomic>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

namespace gengine::system::driver::output {

class VoIPAudio : public gengine::System<VoIPAudio>, public LocalSystem {
public:
    ~VoIPAudio() override;

    void onMainLoop(gengine::system::event::MainLoop &e);
    void onStartEngine(gengine::system::event::StartEngine &e);

    void init(void) override;

    bool isEnabled(void) const {
        return m_enabled;
    }

    auto &getBuffers(void) {
        return m_outputBuffers;
    }

    float getVolume(void) const {
        return volume;
    }
    bool isStreaming(void) const {
        return m_streaming;
    }
    void setStreaming(bool streaming) {
        m_streaming = streaming;
    }
    void setVolume(float volume, bool relative = false);

    std::mutex &getSndMutex(void) {
        return m_sndmutex;
    }

private:
    void processSoundInput(void);

    std::atomic_bool m_running = false;
    std::atomic_bool m_enabled = true;
    std::atomic_bool m_streaming = false;
    std::thread m_soundThread;

    size_t m_sampleRate = 16000;
    size_t m_numChannel = 1;

    float volume = 0.8f; /* in % */
    // std::vector<float> m_outputBuffer; /* otherpeoplebuffer for port sound */
    std::unordered_map<uint64_t, std::vector<float>> m_outputBuffers; /* people id, their buffer */
    std::queue<std::unordered_map<uint64_t, std::vector<std::vector<uint8_t>>>> m_inputBuffer;

    mutable std::mutex m_mutex;
    mutable std::mutex m_sndmutex;
};

} // namespace gengine::system::driver::output
