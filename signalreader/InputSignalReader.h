#ifndef AUDIOPATHANALYZER_INPUTSIGNALREADER_H
#define AUDIOPATHANALYZER_INPUTSIGNALREADER_H

#include <cmath>
#include <climits>
#include <iostream>
#include <alsa/asoundlib.h>

#include <thread>
#include <functional>

#include "../conf.h"

using namespace std;

class InputSignalReader {

private:
    bool running = false;

    snd_pcm_uframes_t bufferSize;
    snd_pcm_t *pcm_handle;

    function<void(snd_pcm_uframes_t, float*)> callback;

    void readSamples();

public:
    InputSignalReader();

    void registerCallback(function<void(snd_pcm_uframes_t, float*)> callback);

    void start();
    void stop();

    snd_pcm_uframes_t getBufferSize();

};


#endif
