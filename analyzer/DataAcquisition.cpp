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

#include "DataAcquisition.h"

DataAcquisition::DataAcquisition() {
    this->in_buffer_size = reader.getBufferSize() / 2; // we only use one channel
    this->fft_out_size = in_buffer_size / 2 + 1;
    this->fft = kiss_fftr_alloc(in_buffer_size, 0, nullptr, nullptr);
    this->resolution = (float) SAMPLE_RATE / (float) in_buffer_size;
}

map<int, Measurement> DataAcquisition::measure() {
    kiss_fft_scalar fft_in[in_buffer_size];
    kiss_fft_cpx fft_out[fft_out_size];

    reader.registerCallback([this, &fft_out, &fft_in](snd_pcm_uframes_t size, const int16_t *values) {
        // de-interleave channels
        for (int i = 0; i < size; i++) {
            if (i % 2 == 0)
                fft_in[i / 2] = values[i];
        }

        kiss_fftr(fft, fft_in, fft_out);

        float maxA = 0;
        int maxi = 0;
        for (int i = 0; i < size / 2 + 1; i++) {
            float A = sqrt(fft_out[i].r * fft_out[i].r + fft_out[i].i * fft_out[i].i);
            if (A >= maxA) {
                maxA = A;
                maxi = i;
            }
        }

        //cout << ((float) maxi * resolution) << endl;
    });

    reader.start();
    gen.start();
}