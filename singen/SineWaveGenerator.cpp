#include "SineWaveGenerator.h"

#include "../kissfft/kiss_fft.h"

SineWaveGenerator::SineWaveGenerator() {
    pcm_handle = nullptr;

    snd_pcm_hw_params_t *hwparams;  // hardware information

    snd_pcm_hw_params_alloca(&hwparams);

    int open_err = snd_pcm_open(&pcm_handle, "plughw:0,0", SND_PCM_STREAM_PLAYBACK, 0);
    if(open_err < 0) {
        cerr << snd_strerror(open_err) << endl;
    }

    snd_pcm_prepare(pcm_handle);
    snd_pcm_drop(pcm_handle);

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

    cout << "Suggested out buffer size: " << bufferSize << endl;

    if (snd_pcm_hw_params_set_buffer_size_near(pcm_handle, hwparams, &bufferSize) < 0)
        cerr << "Failed to set buffer size" << endl;

    cout << "Actual out buffer size: " << bufferSize << endl;

    snd_pcm_hw_params(pcm_handle, hwparams);
}

void SineWaveGenerator::sendSamples() {
    float vals[bufferSize];

    snd_pcm_writei(pcm_handle, vals, bufferSize);

    while (running) {
        float delta = (2.f * 3.141592653589793238462643383279502884197169399375105820974944f * f) / (float) SAMPLE_RATE;

        for (int i = 0; i < bufferSize; i++) {
            vals[i] = cos(phi);
            phi += delta;
        }

        snd_pcm_sframes_t frames = snd_pcm_writei(pcm_handle, vals, bufferSize);

        if (frames < 0) {
            cerr << "Failed to write, recovering" << endl;
            snd_pcm_recover(pcm_handle, frames, 0);
        }

    }
}

void SineWaveGenerator::start() {
    if (running)
        return;

    f = 440;
    running = true;

    thread sineThread(&SineWaveGenerator::sendSamples, this);
    sineThread.join();
}

void SineWaveGenerator::stop() {
    running = false;
}

void SineWaveGenerator::setFrequency(float f) {
    this->f = f;
}