/*
 * Copyright 2020 Klemen Pevec
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AUDIOPATHANALYZER_SINEWAVEGENERATOR_H
#define AUDIOPATHANALYZER_SINEWAVEGENERATOR_H

#include <cmath>
#include <climits>
#include <iostream>
#include <alsa/asoundlib.h>

#include <thread>

#include "../conf.h"
#include "../kissfft/kiss_fft.h"

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
