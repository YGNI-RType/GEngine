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

#include <opus/opus.h>
#include <portaudio.h>

namespace gengine::system::driver::output {

constexpr size_t SAMPLE_RATE = 48000;
constexpr size_t FRAME_SIZE = 960;

// Opus encoder and decoder
OpusDecoder *decoder;
PaStream *playbackStream;

static int playbackCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                            void *userData) {
    float *output = static_cast<float *>(outputBuffer);
    VoIPAudio *voipHandler = static_cast<VoIPAudio *>(userData);
    if (!voipHandler->isEnabled())
        return paContinue;

    auto buffer = voipHandler->getBuffer();
    if (buffer.empty())
        return paContinue;

    auto volumeMultiplier = voipHandler->getVolume();

    /* reduces the volume */
    for (size_t i = 0; i < std::min(buffer.size(), framesPerBuffer); ++i)
        buffer[i] *= volumeMultiplier;

    if (buffer.size() < framesPerBuffer) {
        std::fill(output, output + framesPerBuffer, 0.0f);
        return paContinue;
    }

    std::copy(buffer.begin(), buffer.begin() + framesPerBuffer, output);
    buffer.erase(buffer.begin(), buffer.begin() + framesPerBuffer);
    return paContinue;
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

    auto index = Pa_GetDefaultInputDevice();
    auto deviceconut = Pa_GetDeviceCount();
    auto apicount = Pa_GetHostApiCount();
    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(index);
    if (deviceInfo == nullptr)
        throw std::runtime_error("No default input device available");

    // Open playback stream
    paErr = Pa_OpenDefaultStream(&playbackStream, 0, 1, paFloat32, SAMPLE_RATE, FRAME_SIZE, playbackCallback, this);
    if (paErr != paNoError)
        throw std::runtime_error("PortAudio Open stream error: " + std::string(Pa_GetErrorText(paErr)));

    m_running = true;
    m_soundThread = std::thread([this]() { processSoundInput(); });
}

void VoIPAudio::onMainLoop(gengine::system::event::MainLoop &e) {
    auto &client = Network::NET::getClient();
    size_t nbPackets = client.getSizeIncommingData(Network::SV_VOIP, false);
    for (size_t i = 0; i < nbPackets; i++) {
        size_t readCount = 0;

        /* todo : not compressedsince segmented is hard */
        auto msg = Network::UDPMessage(0, Network::SV_VOIP);
        if (!client.popIncommingData(msg, readCount, false))
            break;

        /*
        Todo, the sv_voip shyuld have segments with:
        uint16_t index of player (tbd the size, max is 4 lol)
        uint16_t size of the data
        .... data

        the size is not more than 1400 bytes
        tbd : should the data be a full opus frame or having some fragments is good ?
        Will opus know where a frame starts and don't advance ?
        appparently it's ok, just keep the rest inside the buffer, the decoded goes directly to the port audio output
        buffer
        */

        uint8_t eof = 0;
        do {
            msg.readContinuousData(eof, readCount);
            if (eof == -1)
                break;
            readCount--;
            Network::UDPG_VoIPSegment segment;
            msg.readContinuousData(segment, readCount);

            size_t playerIndex = segment.playerIndex;
            size_t dataSize = segment.size;

            std::vector<uint8_t> encodedData(dataSize);
            msg.readData(encodedData.data(), readCount, dataSize);

            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_inputBuffer.push(encodedData);
            }
        } while (readCount == msg.getSize());

        std::vector<uint8_t> encodedData(msg.getSize());
        msg.readDataCompressed(encodedData.data(), readCount, msg.getSize());
    }
}

/* This code is only run by the sound thread */
void VoIPAudio::processSoundInput(void) {
    std::vector<float> decodedBuffer(FRAME_SIZE);

    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (!m_enabled || m_inputBuffer.empty())
            continue;

        std::vector<uint8_t> encodedData;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            encodedData = m_inputBuffer.front();
            m_inputBuffer.pop();
        }

        // Encode captured audio using Opus
        int decodedSize =
            opus_decode_float(decoder, encodedData.data(), encodedData.size(), decodedBuffer.data(), FRAME_SIZE, 0);
        if (decodedSize < 0) {
            std::cerr << "Opus decoding error: " << opus_strerror(decodedSize) << std::endl;
            continue;
        }

        // Add decoded audio to playback buffer
        m_outputBuffer.insert(m_outputBuffer.end(), decodedBuffer.begin(), decodedBuffer.end());
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
        volume = volume;
}

} // namespace gengine::system::driver::input
