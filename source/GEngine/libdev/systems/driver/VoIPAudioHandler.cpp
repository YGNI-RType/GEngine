/*
** EPITECH PROJECT, 2024
** GameEngine
** File description:
** VoIPAudioHandler.cpp
*/

#include "GEngine/libdev/systems/driver/VoIPAudioHandler.hpp"
#include "GEngine/libdev/systems/driver/input/KeyboardCatcher.hpp"

/**********************************************************************************************/
/*
    Since "raylib" already defines miniaudio (in the worst way for voip), we define minilib again but statically.
    This mean this file will be the only compile file using miniaudio for the gengine, can't split it.
    You can thank raylib for this
*/
/**********************************************************************************************/

#define MA_API static
#define MA_NO_JACK
#define MA_NO_FLAC
#define MA_NO_MP3
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace gengine::system::driver::input {

static ma_encoder g_encoder;
static ma_decoder g_decoder;
static ma_device g_recordDevice;

static void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount) {
    VoIPAudioHandler *voipHandler = static_cast<VoIPAudioHandler *>(pDevice->pUserData);
    if (!voipHandler->isCapturing())
        return;

    ma_encoder_write_pcm_frames(&g_encoder, pInput, frameCount, NULL);

    // temp temp temp

    auto recorderBuffer = voipHandler->getRecorderBufferOutput();
    recorderBuffer = voipHandler->getRecorderBuffer();

    ma_decoder_read_pcm_frames(&g_decoder, pOutput, frameCount, NULL);

    std::cout << "data_callback" << std::endl;
}

static ma_result encoderOnWrite(ma_encoder *encoder, const void *data, const size_t data_size,
                                size_t *out_written_data_size) {
    VoIPAudioHandler *voipHandler = static_cast<VoIPAudioHandler *>(encoder->pUserData);
    if (!voipHandler->isCapturing())
        return MA_SUCCESS;

    RecorderBuffer *rc = &voipHandler->getRecorderBuffer();
    const size_t min_target_size = rc->offset + data_size;

    /* todo: maybe just append may be as fast, but beware of encoderOnSeek */

    if (rc->offset + min_target_size > rc->buffer.size())
        rc->buffer.resize(rc->offset + min_target_size);

    std::copy(reinterpret_cast<const uint8_t *>(data), reinterpret_cast<const uint8_t *>(data) + data_size,
              rc->buffer.begin() + rc->offset);

    rc->offset += data_size;
    return *out_written_data_size = data_size, MA_SUCCESS;
}

static ma_result encoderOnSeek(ma_encoder *encoder, ma_int64 off_from_origin, ma_seek_origin origin) {
    VoIPAudioHandler *voipHandler = static_cast<VoIPAudioHandler *>(encoder->pUserData);
    if (!voipHandler->isCapturing())
        return MA_SUCCESS;

    RecorderBuffer *rc = &voipHandler->getRecorderBuffer();
    size_t off = 0;
    switch (origin) {
    case ma_seek_origin_start:
        off = off_from_origin;
        break;
    case ma_seek_origin_current:
        off = rc->offset + off_from_origin;
        break;
    case ma_seek_origin_end:
        off = rc->buffer.size() - 1 - off_from_origin;
        break;
    }

    if (off >= rc->buffer.size())
        return MA_OUT_OF_RANGE;
    rc->offset = off;
    return MA_SUCCESS;
}

static ma_result decoderOnRead(ma_decoder *decoder, void *buffer, size_t bytesToRead, size_t *bytesRead) {
    VoIPAudioHandler *voipHandler = static_cast<VoIPAudioHandler *>(decoder->pUserData);

    auto output = voipHandler->getRecorderBufferOutput();
    ma_uint64 ubytesRead = 0;
    if (ma_decoder_read_pcm_frames(&g_decoder, static_cast<uint16_t *>(buffer), bytesToRead / 2, &ubytesRead) != MA_SUCCESS)
        return MA_ERROR;

    *bytesRead = ubytesRead * 2;
    return MA_SUCCESS;
}


static ma_result decoderOnSeek(ma_decoder *decoder, ma_int64 byteOffset, ma_seek_origin origin) {
    VoIPAudioHandler *voipHandler = static_cast<VoIPAudioHandler *>(decoder->pUserData);

    RecorderBuffer *rc = &voipHandler->getRecorderBufferOutput();
    size_t off = 0;
    switch (origin) {
    case ma_seek_origin_start:
        off = byteOffset;
        break;
    case ma_seek_origin_current:
        off = rc->offset + byteOffset;
        break;
    case ma_seek_origin_end:
        off = rc->buffer.size() - 1 - byteOffset;
        break;
    }

    if (off >= rc->buffer.size())
        return MA_OUT_OF_RANGE;
    rc->offset = off;
    return MA_SUCCESS;
}

//////////////

void VoIPAudioHandler::init(void) {
    subscribeToEvent<gengine::system::driver::input::KeyVEvent>(&VoIPAudioHandler::onCapture);

    /** Miniaudio **/

    ma_encoder_config encoderConfig;
    ma_device_config deviceConfig;

    encoderConfig = ma_encoder_config_init(ma_encoding_format_wav, ma_format_s16, 2, 44100);

    auto res = ma_encoder_init(encoderOnWrite, encoderOnSeek, static_cast<void *>(this), &encoderConfig, &g_encoder);
    if (res != MA_SUCCESS)
        throw std::runtime_error("Failed to initialize output file.");

    deviceConfig = ma_device_config_init(ma_device_type_capture);
    deviceConfig.capture.format = g_encoder.config.format;
    deviceConfig.capture.channels = g_encoder.config.channels;
    deviceConfig.sampleRate = g_encoder.config.sampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = this;

    if (ma_device_init(NULL, &deviceConfig, &g_recordDevice) != MA_SUCCESS)
        throw std::runtime_error("Failed to initialize capture device.\n");

    // if (ma_device_start(&g_recordDevice) != MA_SUCCESS) {
    //     ma_device_uninit(&g_recordDevice);
    //     throw std::runtime_error("Failed to start device.\n");
    // }

    //////////////

    ma_decoder_config decoderConfig;
    // decoderConfig = ma_decoder_config_init(ma_format_s16, 2, 44100);
    decoderConfig = ma_decoder_config_init_default();
    decoderConfig.format = ma_format_s16;
    decoderConfig.encodingFormat = ma_encoding_format_wav;

    res = ma_decoder_init(decoderOnRead, decoderOnSeek, static_cast<void *>(this), &decoderConfig, &g_decoder);
    if (res != MA_SUCCESS)
        throw std::runtime_error("Failed to initialize output file.");
}

void VoIPAudioHandler::onCapture(gengine::system::driver::input::KeyVEvent &e) {
    switch (e.state) {
    case gengine::system::driver::input::InputState::PRESSED:
        m_capturing = true;
        break;
    case gengine::system::driver::input::InputState::RELEASE:
        m_capturing = false;
        break;
    default:
        break;
    }
}

VoIPAudioHandler::~VoIPAudioHandler() {
    ma_device_uninit(&g_recordDevice);
    ma_encoder_uninit(&g_encoder);

    // temp temptet tmp
    ma_decoder_uninit(&g_decoder);
}

} // namespace gengine::system::driver::input
