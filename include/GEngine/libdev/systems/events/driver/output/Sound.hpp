/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Event
** ════════════════════════════════════════════════════════════════════════════
**  File        : Sound.hpp
**  Create at   : 2024-10-15 05:14
**  Author      : AUTHOR
**  Description : This event is listened to natively by the AudioManager system
                    which will launch the sound linked to the path passed as an
                    argument when it is triggered by this event.
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include "GEngine/libdev/Entity.hpp"
#include "GEngine/libdev/System.hpp"

namespace gengine::system::event::driver::output {
class Sound : public Event {
public:
    Sound(const std::string &path)
        : m_path(path) {
    }

    const std::string &getPath(void) const {
        return m_path;
    }

private:
    const std::string m_path;
};

struct SoundPlayed : public Event {
    std::uint64_t soundId;

    SoundPlayed(std::uint64_t soundId)
        : soundId(soundId) {
    }
};

struct Music : public Event {
    Music(const std::string &path, float volume = 1.f)
        : path(path)
        , volume(volume) {
    }

    const std::string path;
    const float volume;
};
} // namespace gengine::system::event::driver::output
