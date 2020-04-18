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

    function<void(snd_pcm_uframes_t, int32_t*)> callback;

    void readSamples();

public:
    InputSignalReader();

    void registerCallback(function<void(snd_pcm_uframes_t, int32_t*)> callback);

    void start();
    void stop();

    snd_pcm_uframes_t getBufferSize();

};


#endif
