/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** VoIPAudioHandler.cpp
*/

#include "GEngine/libdev/systems/driver/VoIPAudioHandler.hpp"
#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"
#include <opus/opus.h>
#include <portaudio.h>

namespace gengine::system::driver::input {

constexpr size_t SAMPLE_RATE = 48000;
constexpr size_t FRAME_SIZE = 960;

// Opus encoder and decoder
OpusEncoder *encoder;
OpusDecoder *decoder;
PaStream *captureStream;
PaStream *playbackStream;
std::vector<float> playbackBuffer;

static int captureCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                           void *userData) {
    VoIPAudioHandler *voipHandler = static_cast<VoIPAudioHandler *>(userData);

    const float *input = static_cast<const float *>(inputBuffer);
    auto &buffer = voipHandler->getRecorderBuffer();
    buffer.insert(buffer.end(), input, input + framesPerBuffer);
    return paContinue;
}

int playbackCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
    float *output = static_cast<float *>(outputBuffer);
    std::vector<float> &buffer = *static_cast<std::vector<float> *>(userData);

    /* reduces the volume */
    for (size_t i = 0; i < std::min(buffer.size(), framesPerBuffer); ++i) {
        buffer[i] *= 0.3f;
    }

    if (buffer.size() < framesPerBuffer) {
        std::fill(output, output + framesPerBuffer, 0.0f);
        return paContinue;
    }

    std::copy(buffer.begin(), buffer.begin() + framesPerBuffer, output);
    buffer.erase(buffer.begin(), buffer.begin() + framesPerBuffer);
    return paContinue;
}

//////////////

void VoIPAudioHandler::init(void) {
    subscribeToEvent<gengine::system::driver::input::KeyVEvent>(&VoIPAudioHandler::onCapture);

    /** Port Audio + Opus **/

    PaError paErr = Pa_Initialize();
    if (paErr != paNoError)
        throw std::runtime_error("PortAudio error: " + std::string(Pa_GetErrorText(paErr)));

    // Initialize Opus encoder and decoder
    int iErr;
    encoder = opus_encoder_create(SAMPLE_RATE, 1, OPUS_APPLICATION_AUDIO, &iErr);
    if (iErr != OPUS_OK)
        throw std::runtime_error("Failed to create Opus encoder: " + std::string(opus_strerror(iErr)));

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

    paErr = Pa_OpenDefaultStream(&captureStream, 1, 0, paFloat32, SAMPLE_RATE, FRAME_SIZE, captureCallback, this);
    if (paErr != paNoError)
        throw std::runtime_error("PortAudio Open stream error: " + std::string(Pa_GetErrorText(paErr)));

    // Open playback stream
    paErr = Pa_OpenDefaultStream(&playbackStream, 0, 1, paFloat32, SAMPLE_RATE, FRAME_SIZE, playbackCallback, &playbackBuffer);
    if (paErr != paNoError)
        throw std::runtime_error("PortAudio Open stream error: " + std::string(Pa_GetErrorText(paErr)));

    m_running = true;
    m_soundThread = std::thread([this]() { processSoundInput(); });
}

void VoIPAudioHandler::onCapture(gengine::system::driver::input::KeyVEvent &e) {
    PaError paErr;

    switch (e.state) {
    case gengine::system::driver::input::InputState::PRESSED:
        paErr = Pa_StartStream(captureStream);
        if (paErr != paNoError)
            throw std::runtime_error("PortAudio Start stream error: " + std::string(Pa_GetErrorText(paErr)));
        paErr = Pa_StartStream(playbackStream);
        if (paErr != paNoError)
            throw std::runtime_error("PortAudio Start stream error: " + std::string(Pa_GetErrorText(paErr)));
        m_capturing = true;
        break;
    case gengine::system::driver::input::InputState::RELEASE:
        paErr = Pa_StopStream(captureStream);
        if (paErr != paNoError)
            throw std::runtime_error("PortAudio STOP stream error: " + std::string(Pa_GetErrorText(paErr)));
        paErr = Pa_StopStream(playbackStream);
        if (paErr != paNoError)
            throw std::runtime_error("PortAudio STOP stream error: " + std::string(Pa_GetErrorText(paErr)));
        m_capturing = false;
        break;
    default:
        break;
    }
}

/* This code is only run by the sound thread */
void VoIPAudioHandler::processSoundInput(void) {
    std::vector<unsigned char> encodedData(4000); // Allocate enough space for encoded data
    std::vector<float> decodedBuffer(FRAME_SIZE);

    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep briefly to reduce CPU usage

        if (!m_capturing) // temp temp temp, we need to hear in the future !!!!
            continue;

        if (m_captureBuffer.size() >= FRAME_SIZE) {
            // Encode captured audio using Opus
            int encodedSize = opus_encode_float(encoder, m_captureBuffer.data(), FRAME_SIZE, encodedData.data(), encodedData.size());
            if (encodedSize < 0) {
                std::cerr << "Opus encoding error: " << opus_strerror(encodedSize) << std::endl;
                continue;
            }

            // Decode encoded audio using Opus
            int decodedSize = opus_decode_float(decoder, encodedData.data(), encodedSize, decodedBuffer.data(), FRAME_SIZE, 0);
            if (decodedSize < 0) {
                std::cerr << "Opus decoding error: " << opus_strerror(decodedSize) << std::endl;
                continue;
            }

            // Add decoded audio to playback buffer
            playbackBuffer.insert(playbackBuffer.end(), decodedBuffer.begin(), decodedBuffer.end());

            // Remove processed audio from capture buffer
            m_captureBuffer.erase(m_captureBuffer.begin(), m_captureBuffer.begin() + FRAME_SIZE);
        }

    }
}

VoIPAudioHandler::~VoIPAudioHandler() {
    m_running = false;
    m_soundThread.join();

    auto err = Pa_StopStream(playbackStream);
    if (err != paNoError)
        throw std::runtime_error("PortAudio error: " + std::string(Pa_GetErrorText(err)));
    err = Pa_StopStream(captureStream);
    if (err != paNoError)
        throw std::runtime_error("PortAudio error: " + std::string(Pa_GetErrorText(err)));

    err = Pa_CloseStream(playbackStream);
    if (err != paNoError)
        throw std::runtime_error("PortAudio error: " + std::string(Pa_GetErrorText(err)));
    err = Pa_CloseStream(captureStream);
    if (err != paNoError)
        throw std::runtime_error("PortAudio error: " + std::string(Pa_GetErrorText(err)));

    opus_encoder_destroy(encoder);
    opus_decoder_destroy(decoder);
    Pa_Terminate();
}

} // namespace gengine::system::driver::input
