#ifndef AUDIOPATHANALYZER_SINEWAVEGENERATOR_H
#define AUDIOPATHANALYZER_SINEWAVEGENERATOR_H

#include <cmath>
#include <climits>
#include <iostream>
#include <alsa/asoundlib.h>

#include <thread>

#include "../conf.h"

#define PI_2 3.141592653589793238462643383279502884197169399375105820974944f * 2.f

using namespace std;

class SineWaveGenerator {

private:
    float f = 0;
    float phi = 0;
    bool running = false;

    snd_pcm_uframes_t bufferSize;
    snd_pcm_t *pcm_handle;

    void sendSamples();

public:
    SineWaveGenerator();

    void start();
    void stop();

    void setFrequency(float f);

};


#endif
