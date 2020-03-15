#include "InputSignalReader.h"

#include <utility>

InputSignalReader::InputSignalReader() {
    pcm_handle = nullptr;

    snd_pcm_hw_params_t *hwparams;

    int open_err = snd_pcm_open(&pcm_handle, strdup("plughw:0,0"), SND_PCM_STREAM_CAPTURE, 0);
    if(open_err < 0) {
        cerr << snd_strerror(open_err) << endl;
    }

    snd_pcm_hw_params_alloca(&hwparams);

    snd_pcm_hw_params_any(pcm_handle, hwparams);

    if (snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
        cerr << "Failed to set access" << endl;
    if (snd_pcm_hw_params_set_format(pcm_handle, hwparams, SND_PCM_FORMAT_FLOAT) < 0)
        cerr << "Failed to set format" << endl;
    if (snd_pcm_hw_params_set_channels(pcm_handle, hwparams, 1) < 0)
        cerr << "Failed to set channels" << endl;

    if (snd_pcm_hw_params_set_rate(pcm_handle, hwparams, SAMPLE_RATE, (int) 0) < 0)
        cerr << "Failed to set rate" << endl;
    if (snd_pcm_hw_params_set_periods(pcm_handle, hwparams, 4, 0) < 0)
        cerr << "Failed to set periods" << endl;

    snd_pcm_uframes_t period_size = 4096;
    if (snd_pcm_hw_params_set_period_size_near(pcm_handle, hwparams, &period_size, nullptr) < 0)
        cerr << "Failed to set period size" << endl;

    bufferSize = period_size * (4 / 2);

    cout << "Suggested in buffer size: " << bufferSize << endl;

    if (snd_pcm_hw_params_set_buffer_size_near(pcm_handle, hwparams, &bufferSize) < 0)
        cerr << "Failed to set buffer size" << endl;

    cout << "Actual in buffer size: " << bufferSize << endl;

    snd_pcm_hw_params(pcm_handle, hwparams);

    snd_pcm_prepare(pcm_handle);
}

void InputSignalReader::registerCallback(function<void(snd_pcm_uframes_t, float*)> callbackparam) {
    this->callback = std::move(callbackparam);
}

void InputSignalReader::readSamples() {
    float vals[bufferSize];

    while(running) {
        snd_pcm_readi(pcm_handle, vals, bufferSize);
        this->callback(bufferSize, vals);
    }
}

void InputSignalReader::start() {
    if(running)
        return;

    running = true;

    thread readThread(&InputSignalReader::readSamples, this);
    readThread.detach();
}

void InputSignalReader::stop() {
    running = false;
}

snd_pcm_uframes_t InputSignalReader::getBufferSize() {
    return bufferSize;
}