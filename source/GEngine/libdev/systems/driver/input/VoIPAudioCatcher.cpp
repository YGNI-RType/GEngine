/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** VoIPAudioCatcher
*/

#include "GEngine/libdev/systems/driver/input/VoIPAudioCatcher.hpp"

#include "GEngine/net/msg.hpp"
#include "GEngine/net/structs/msg_udp_structs.hpp"
#include "GEngine/net/net.hpp"

#include <opus/opus.h>
#include <portaudio.h>

namespace gengine::system::driver::input {

constexpr size_t SAMPLE_RATE = 48000;
constexpr size_t FRAME_SIZE = 960;

// Opus encoder and decoder
OpusEncoder *encoder;
PaStream *captureStream;

static int captureCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                           void *userData) {
    VoIPAudioCatcher *voipHandler = static_cast<VoIPAudioCatcher *>(userData);
    if (voipHandler->isCapturing() == false)
        return paContinue;

    const float *input = static_cast<const float *>(inputBuffer);
    std::vector<float> buffer;
    buffer.insert(buffer.end(), input, input + framesPerBuffer);

    std::vector<unsigned char> encodedData(4000); // Allocate enough space for encoded data
    int encodedSize = opus_encode_float(encoder, buffer.data(), FRAME_SIZE, encodedData.data(), encodedData.size());
    if (encodedSize < 0) {
        std::cerr << "Opus encoding error: " << opus_strerror(encodedSize) << std::endl;
        return paAbort;
    }

    voipHandler->getRecorderBuffer().insert(voipHandler->getRecorderBuffer().end(), encodedData.begin(),
                                            encodedData.end());
    return paContinue;
}

//////////////

void VoIPAudioCatcher::init(void) {
    subscribeToEvent<gengine::system::driver::input::KeyVEvent>(&VoIPAudioCatcher::onCapture);
    subscribeToEvent<gengine::system::event::MainLoop>(&VoIPAudioCatcher::onMainLoop);

    /** Port Audio + Opus **/
    int iErr;
    PaError paErr;

    encoder = opus_encoder_create(SAMPLE_RATE, 1, OPUS_APPLICATION_AUDIO, &iErr);
    if (iErr != OPUS_OK)
        throw std::runtime_error("Failed to create Opus encoder: " + std::string(opus_strerror(iErr)));

    paErr = Pa_OpenDefaultStream(&captureStream, 1, 0, paFloat32, SAMPLE_RATE, FRAME_SIZE, captureCallback, this);
    if (paErr != paNoError)
        throw std::runtime_error("PortAudio Open stream error: " + std::string(Pa_GetErrorText(paErr)));
}

void VoIPAudioCatcher::onMainLoop(gengine::system::event::MainLoop &e) {
    if (m_captureBuffer.empty())
        return;

    Network::UDPMessage msg(Network::UDPMessage::COMPRESSED, Network::CL_VOIP);
    msg.startCompressingSegment(false);
    msg.appendData((const void *)m_captureBuffer.data(), CF_NET_MIN(1400, m_captureBuffer.size()));
    msg.stopCompressingSegment(false);

    Network::NET::getClient().pushData(msg);

    m_captureBuffer.erase(m_captureBuffer.begin(), m_captureBuffer.begin() + CF_NET_MIN(1400, m_captureBuffer.size()));
}

void VoIPAudioCatcher::onCapture(gengine::system::driver::input::KeyVEvent &e) {
    PaError paErr;

    switch (e.state) {
    case gengine::system::driver::input::InputState::PRESSED:
        paErr = Pa_StartStream(captureStream);
        if (paErr != paNoError)
            throw std::runtime_error("PortAudio Start stream error: " + std::string(Pa_GetErrorText(paErr)));
        m_capturing = true;
        break;
    case gengine::system::driver::input::InputState::RELEASE:
        paErr = Pa_StopStream(captureStream);
        if (paErr != paNoError)
            throw std::runtime_error("PortAudio STOP stream error: " + std::string(Pa_GetErrorText(paErr)));
        m_capturing = false;
        break;
    default:
        break;
    }
}

VoIPAudioCatcher::~VoIPAudioCatcher() {
    PaError err = Pa_StopStream(captureStream);
    err = Pa_CloseStream(captureStream);

    opus_encoder_destroy(encoder);
}

} // namespace gengine::system::driver::input
