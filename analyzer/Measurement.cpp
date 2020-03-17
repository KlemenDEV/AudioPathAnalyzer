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

#include "Measurement.h"

Measurement::Measurement(float gen_f, kiss_fft_cpx *fft_out, size_t fft_out_size, float resolution) {
    float maxA = 0;
    int maxi = 0;

    float energy = 0;

    for (int i = 0; i < fft_out_size; i++) {
        float A = sqrt(fft_out[i].r * fft_out[i].r + fft_out[i].i * fft_out[i].i) / fft_out_size;

        energy += A;

        if (A > maxA) {
            maxA = A;
            maxi = i;
        }
    }

    // f: ((float) maxi * resolution)

    cout << gen_f << "," << maxA << endl;
}

Measurement Measurement::operator+ (const Measurement &first) {
    Measurement retval = first;

    retval.f += this->f;

    return retval;
}