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

#ifndef AUDIOPATHANALYZER_MEASUREMENT_H
#define AUDIOPATHANALYZER_MEASUREMENT_H

#include <iostream>

#include <kiss_fft.h>

using namespace std;

class Measurement {

public:
    float f = 0;
    float energy = 0;
    float f_amplitude = 0;

    Measurement(float gen_f, kiss_fft_cpx *fft_out, size_t fft_out_size, float resolution);

    // copy constructor
    Measurement(const Measurement &m) {
        f = m.f;
        energy = m.energy;
        f_amplitude = m.f_amplitude;
    }

    Measurement() = default;

    Measurement operator+ (const Measurement & first);

    Measurement operator/ (int n);

};


#endif
