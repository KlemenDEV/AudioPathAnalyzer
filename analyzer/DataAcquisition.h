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

#ifndef AUDIOPATHANALYZER_DATAACQUISITION_H
#define AUDIOPATHANALYZER_DATAACQUISITION_H

#include <map>
#include <vector>

#include <SineWaveGenerator.h>
#include <InputSignalReader.h>
#include <kiss_fftr.h>

#include "Measurement.h"

using namespace std;

class DataAcquisition {

private:
    SineWaveGenerator gen;
    InputSignalReader reader;

    snd_pcm_uframes_t in_buffer_size;
    size_t fft_out_size;

    kiss_fftr_cfg fft;
    float resolution;

    int getLatencyInSamples();

public:
    DataAcquisition();

    map<int, Measurement> measure(int steps, int takes);
};


#endif
