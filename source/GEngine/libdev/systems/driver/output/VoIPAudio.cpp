/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** VoIPAudio.cpp
*/

#include "GEngine/libdev/systems/driver/output/VoIPAudio.hpp"

#include "GEngine/net/msg.hpp"
#include "GEngine/net/net.hpp"
#include "GEngine/net/structs/msg_udp_structs.hpp"

#include <algorithm>
#include <opus/opus.h>
#include <portaudio.h>
#include <math.h>

namespace gengine::system::driver::output {

constexpr size_t SAMPLE_RATE = 16000;
constexpr size_t FRAME_SIZE = 960;

// Opus encoder and decoder
OpusDecoder *decoder;
PaStream *playbackStream = nullptr;

void applyHannWindow(std::vector<float>& buffer, int windowLength) {
    for (int i = 0; i < windowLength && i < buffer.size(); ++i) {
        float window = 0.5 * (1 - std::cos(2 * M_PI * i / (windowLength - 1)));
        buffer[i] *= window;
        buffer[buffer.size() - 1 - i] *= window;
    }
}

static int playbackCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                            void *userData) {
    float *output = static_cast<float *>(outputBuffer);
    VoIPAudio *voipHandler = static_cast<VoIPAudio *>(userData);

    if (!voipHandler->isEnabled()) {
        std::fill(output, output + framesPerBuffer, 0.0f);
        return paContinue;
    }

    {
        std::lock_guard<std::mutex> lock(voipHandler->getSndMutex());
        std::vector<float> buffer;
        auto &bufferMap = voipHandler->getBuffers();
        if (bufferMap.empty()) {
            std::fill(output, output + framesPerBuffer, 0.0f);
            return paContinue;
        }

        for (auto &[_, playerBuffer] : bufferMap) {
            if (playerBuffer.empty())
                continue;

            if (buffer.empty())
                buffer = playerBuffer;
            else {
                std::transform(playerBuffer.begin(), playerBuffer.begin() + FRAME_SIZE, buffer.begin(), buffer.begin(),
                               [](float a, float b) { return (a + b) / 2.f; });
            }

            playerBuffer.erase(playerBuffer.begin(), playerBuffer.begin() + FRAME_SIZE);
        }
        if (buffer.empty()) {
            std::fill(output, output + framesPerBuffer, 0.0f);
            return paContinue;
        }

        /* reduces the volume */
        auto volumeMultiplier = voipHandler->getVolume();
        for (size_t i = 0; i < framesPerBuffer; ++i)
            buffer[i] *= volumeMultiplier;

        std::copy(buffer.begin(), buffer.begin() + framesPerBuffer, output);
        return paContinue;
    }
}

//////////////

void VoIPAudio::init(void) {
    subscribeToEvent<gengine::system::event::MainLoop>(&VoIPAudio::onMainLoop);

    /** Port Audio + Opus **/

    /* only one having this responsabiliy !!! !*/
    PaError paErr = Pa_Initialize();
    if (paErr != paNoError)
        throw std::runtime_error("PortAudio error: " + std::string(Pa_GetErrorText(paErr)));

    // Initialize Opus encoder and decoder
    int iErr;
    decoder = opus_decoder_create(SAMPLE_RATE, 1, &iErr);
    if (iErr != OPUS_OK)
        throw std::runtime_error("Failed to create Opus decoder: " + std::string(opus_strerror(iErr)));

    // Open capture stream

    PaDeviceIndex defaultInputDevice = Pa_GetDefaultInputDevice();
    if (defaultInputDevice == paNoDevice)
        throw std::runtime_error("No default input device available.");

    const PaDeviceInfo *inputDeviceInfo = Pa_GetDeviceInfo(defaultInputDevice);
    if (inputDeviceInfo == nullptr)
        throw std::runtime_error("Failed to get input device info.");

    // if(m_sampleRate != static_cast<size_t>(inputDeviceInfo->defaultSampleRate))
    //     std::cerr << "VoIP: Your default sample rate is not 48kHz, this may cause issues." << std::endl;

    m_numChannel = inputDeviceInfo->maxInputChannels;

    // Open playback stream
    paErr = Pa_OpenDefaultStream(&playbackStream, 0, 1, paFloat32, SAMPLE_RATE, FRAME_SIZE, playbackCallback, this);
    if (paErr != paNoError)
        throw std::runtime_error("PortAudio Open stream error: " + std::string(Pa_GetErrorText(paErr)));

    paErr = Pa_StartStream(playbackStream);
    if (paErr != paNoError)
        throw std::runtime_error("PortAudio Start stream error: " + std::string(Pa_GetErrorText(paErr)));

    m_running = true;
    m_soundThread = std::thread([this]() { processSoundInput(); });
}

void VoIPAudio::onMainLoop(gengine::system::event::MainLoop &e) {
    auto &client = Network::NET::getClient();
    size_t nbPackets = client.getSizeIncommingData(Network::SV_VOIP, false);
    std::unordered_map<uint64_t, std::vector<std::vector<uint8_t>>> tempbuffer;
    for (size_t i = 0; i < nbPackets; i++) {
        size_t readCount = 0;

        /* todo : not compressedsince segmented is hard */
        auto msg = Network::UDPMessage(Network::UDPMessage::HEADER, Network::SV_VOIP);
        if (!client.popIncommingData(msg, readCount, false))
            break;

        do {
            Network::UDPG_VoIPSegment segment;
            msg.readContinuousData(segment, readCount);

            /*todo:  playerid is a array<16>, uuid some sort */
            uint64_t playerId = segment.playerIndex1;
            size_t dataSize = segment.size;

            std::vector<uint8_t> encodedData(dataSize);
            msg.readData(encodedData.data(), readCount, dataSize);
            if (tempbuffer.find(playerId) == tempbuffer.end())
                tempbuffer[playerId] = std::vector<std::vector<uint8_t>>();

            tempbuffer[playerId].push_back(encodedData);
        } while (readCount != msg.getSize());

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_inputBuffer.push(tempbuffer);
            tempbuffer.clear();
        }
    }
}

/* This code is only run by the sound thread */
void VoIPAudio::processSoundInput(void) {
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!m_enabled)
            continue;

        std::unordered_map<uint64_t, std::vector<std::vector<uint8_t>>> vecsMap;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_inputBuffer.empty())
                continue;
            vecsMap = m_inputBuffer.front();
            m_inputBuffer.pop();
        }

        std::unordered_map<uint64_t, std::vector<float>> decodeVecs;
        for (const auto &pair : vecsMap)
            decodeVecs[pair.first] = std::vector<float>();

        for (auto &[playerId, vecs] : vecsMap) {
            for (auto &encodedData : vecs) {
                std::vector<float> decodedBuffer(FRAME_SIZE, 0.0f);
                int decodedSize = opus_decode_float(decoder, encodedData.data(), encodedData.size(),
                                                    decodedBuffer.data(), FRAME_SIZE, 0);
                if (decodedSize < 0) {
                    std::cerr << "Opus decoding error: " << opus_strerror(decodedSize) << std::endl;
                    continue;
                }
                decodeVecs[playerId].insert(decodeVecs[playerId].end(), decodedBuffer.begin(),
                                            decodedBuffer.begin() + decodedSize);
            }
        }

        {
            std::lock_guard<std::mutex> lock(m_sndmutex);
            for (auto &[playerId, buffer] : decodeVecs) {
                if (m_outputBuffers.find(playerId) == m_outputBuffers.end())
                    m_outputBuffers[playerId] = std::vector<float>();

                m_outputBuffers[playerId].insert(m_outputBuffers[playerId].end(), buffer.begin(), buffer.end());
            }
        }

        // std::vector<float> mixedBuffer(FRAME_SIZE, 0.0f);

        // for (const auto &decodedBuffer : decodeVecs) {
        //     // std::transform(decodedBuffer.begin(), decodedBuffer.end(), mixedBuffer.begin(), mixedBuffer.begin(),
        //     // std::plus<float>());
        //     std::transform(decodedBuffer.begin(), decodedBuffer.end(), mixedBuffer.begin(), mixedBuffer.begin(),
        //                    [decodeVecs](float a, float b) { return (a + b) / float(decodeVecs.size()); });
        // }

        // // m_outputBuffer.insert(m_outputBuffer.end(), mixedBuffer.begin(), mixedBuffer.end());
        // {
        //     std::lock_guard<std::mutex> lock(m_sndmutex);
        //     if (m_outputBuffer.empty() || m_outputBuffer.size() < FRAME_SIZE * 2) {
        //         m_outputBuffer.insert(m_outputBuffer.end(), mixedBuffer.begin(), mixedBuffer.end());
        //         continue;
        //     }
        //     // if (m_outputBuffer.size() < mixedBuffer.size()) {
        //     //     std::cout << "jojo" << std::endl;
        //     //     m_outputBuffer.insert(m_outputBuffer.end(), mixedBuffer.begin(), mixedBuffer.end());
        //     //     continue;
        //     // }
        //     std::cout << m_outputBuffer.size() << " b" << std::endl;
        //     std::transform(mixedBuffer.begin(), mixedBuffer.end(), m_outputBuffer.begin() + FRAME_SIZE,
        //                    m_outputBuffer.begin() + FRAME_SIZE, [](float a, float b) { return (a + b / 2.f); });
        // }
        // }
        // if (mixedBuffer.size() > m_outputBuffer.size())
        //     m_outputBuffer.insert(m_outputBuffer.end(), mixedBuffer.begin() + m_outputBuffer.size(),
        //     mixedBuffer.end());
        // }

        // if (m_outputBuffer.empty())
        //     m_outputBuffer.insert(m_outputBuffer.end(), decodedBuffer.begin(), decodedBuffer.end());
        // else {
        //     // std::cout << "fuck" << std::endl;
        //     if (vecs.size() == 1) {
        //         m_outputBuffer.insert(m_outputBuffer.end(), decodedBuffer.begin(), decodedBuffer.end());
        //         continue;
        //     }

        //     std::cout << "buf" << std::endl;
        //     std::transform(decodedBuffer.begin(), decodedBuffer.end(), m_outputBuffer.begin(),
        //     m_outputBuffer.begin(), [](float a, float b) {
        //         return (a + b);
        //     });
        //     if (decodedBuffer.size() > m_outputBuffer.size())
        //         m_outputBuffer.insert(m_outputBuffer.end(), decodedBuffer.begin() + m_outputBuffer.size(),
        //         decodedBuffer.end());
        // }
        // Encode captured audio using Opus

        // Add decoded audio to playback buffer
        // m_outputBuffer.insert(m_outputBuffer.end(), finalBuffer.begin(), finalBuffer.end());
    }
}

VoIPAudio::~VoIPAudio() {
    m_running = false;
    m_soundThread.join();

    auto err = Pa_StopStream(playbackStream);
    err = Pa_CloseStream(playbackStream);

    opus_decoder_destroy(decoder);
    Pa_Terminate();
}

void VoIPAudio::setVolume(float wantedVolume, bool relative) {
    if (wantedVolume < 0.0f || wantedVolume > 1.0f)
        wantedVolume = 0.3f;

    if (relative) {
        volume += volume;
        volume = CF_NET_CLAMP(volume, 0.0f, 1.0f);
    } else
        volume = wantedVolume;
}

} // namespace gengine::system::driver::output
