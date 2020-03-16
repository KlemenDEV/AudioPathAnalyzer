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
    this->resolution = (float) SAMPLE_RATE / (float) in_buffer_size;

    this->fft = kiss_fftr_alloc(in_buffer_size, 0, nullptr, nullptr);
}

int DataAcquisition::getLatencyInSamples() {
    int samplesCount = 0;
    reader.registerCallback(
            [this, &samplesCount](snd_pcm_uframes_t size, const int16_t *values) {
                if (samplesCount == 0)
                    gen.setFrequency(4000);

                unsigned long ampl = 0;
                for (int i = 0; i < size; i++) {
                    if (i % 2 == 0)
                        ampl += abs(values[i]) / size;
                }

                samplesCount++;

                if (ampl > 1000) {
                    reader.stop();
                    gen.stop();
                }
            });

    reader.start();
    gen.start();

    return samplesCount;
}

map<int, Measurement> DataAcquisition::measure(int steps) {
    map<int, Measurement> measurements;

    kiss_fft_scalar fft_in[in_buffer_size];
    kiss_fft_cpx fft_out[fft_out_size];

    float f_list[steps];

    double max_f_log = log((float) SAMPLE_RATE / 2.f) / log(10);

    double d_gen = max_f_log / (double) steps;

    for (int i = 0; i < steps; i++) {
        f_list[i] = pow(10, d_gen * (float) i);
    }

    int meas_idx = 0;

    int latency = this->getLatencyInSamples();

    reader.registerCallback(
            [this, &fft_out, &fft_in, &meas_idx, steps, &f_list, latency](snd_pcm_uframes_t size,
                                                                          const int16_t *values) {
                gen.setFrequency(f_list[meas_idx]);

                // de-interleave channels
                for (int i = 0; i < size; i++) {
                    if (i % 2 == 0)
                        fft_in[i / 2] = values[i];
                }

                kiss_fftr(fft, fft_in, fft_out);

                float maxA = 0;
                int maxi = 0;

                float energySum = 0;

                for (int i = 0; i < fft_out_size; i++) {
                    float A = sqrt(fft_out[i].r * fft_out[i].r + fft_out[i].i * fft_out[i].i) / fft_out_size;

                    energySum += A;

                    if (A >= maxA) {
                        maxA = A;
                        maxi = i;
                    }
                }

                if (meas_idx - latency > 0) {
                    cout << f_list[meas_idx - latency] << " : " << ((float) maxi * resolution) << endl;
                }

                meas_idx++;
                if (meas_idx >= steps) {
                    reader.stop();
                    gen.stop();
                }
            });

    reader.start();
    gen.start();

    return measurements;
}