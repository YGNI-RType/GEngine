/*
** ════════════════════════════════════════════════════════════════════════════
**                           GEngine (libdev) Component
** ════════════════════════════════════════════════════════════════════════════
**  File        : Animation.hpp
**  Create at   : 2024-10-15 05:25
**  Auteur      : AUTHOR
**  Description : DESCRIPTION // TODO popoche
** ════════════════════════════════════════════════════════════════════════════
*/

#pragma once

#include <cstring>

#include "GEngine/libdev/Component.hpp"
#include "GEngine/net/net_string.hpp"

namespace gengine::component::driver::output {
struct AnimationTrack {
    enum PlaybackMode {
        Forward, // Animation plays from start to end
        Reverse, // Animation plays from end to start
        Idling,  // Animation go to targeted idle frame
        None     // Static frame (no animation)
    };
    std::vector<int> frames; // Indices of frames in the animation sequence
    size_t idleFrameIdx;     // Index of the idling frame in the track
    bool looping;            // Should this track loop?

    AnimationTrack(const std::vector<int> &frames, size_t idleFrameIdx = 0, bool looping = true)
        : frames(frames)
        , idleFrameIdx(idleFrameIdx)
        , looping(looping) {
    }
};
struct Animation : public Component<Animation> {
    Network::NetString<32> trackName;    // Name of the current track (no std::string)
    size_t currentFrameIndex;            // Index of the current frame in the track
    float frameDuration;                 // Duration of each frame (in seconds)
    float currentTime;                   // Time accumulated to track frame switching
    AnimationTrack::PlaybackMode m_mode; // Playback mode (Forward, Reverse, PingPong, Idling)

    Animation()
        : trackName("")
        , currentFrameIndex(0)
        , frameDuration(0)
        , currentTime(0)
        , m_mode(AnimationTrack::PlaybackMode::Forward) {
    }
    Animation(const std::string &initialTrackName, float frameDuration,
              AnimationTrack::PlaybackMode mode = AnimationTrack::PlaybackMode::Forward, size_t startingFrame = 0)
        : trackName(initialTrackName)
        , currentFrameIndex(startingFrame)
        , frameDuration(frameDuration)
        , currentTime(0)
        , m_mode(mode) {
    }

    bool operator==(const Animation &) const = default;

    void setPlaybackMode(AnimationTrack::PlaybackMode mode) {
        if (m_mode == mode)
            return;
        m_mode = mode;
        currentTime = 0;
    }

    AnimationTrack::PlaybackMode getPlaybackMode(void) const {
        return m_mode;
    }
};
} // namespace gengine::component::driver::output
