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

    cout << "FFT frequency resolution: " << this->resolution << endl;

    this->fft = kiss_fftr_alloc(in_buffer_size, 0, nullptr, nullptr);
}

vector<Measurement> DataAcquisition::measure(int steps) {
    vector<Measurement> measurements;

    kiss_fft_scalar fft_in[in_buffer_size];
    kiss_fft_cpx fft_out[fft_out_size];

    int latency = this->getLatencyInSamples();
    cout << "Measured latency: " << latency << endl;

    float f_list[steps];

    double min_f_log = log10(MEAS_F_LOW);
    double max_f_log = log10(MEAS_F_HIGH);
    double d_gen = (max_f_log - min_f_log) / (double) (steps - 1);
    for (int i = 0; i < steps; i++) {
        f_list[i] = pow(10, min_f_log + d_gen * i);
    }

    int meas_idx = 0;
    int take_idx = 0;
    int takes = latency + 1;

    reader.registerCallback([&](
            snd_pcm_uframes_t size, const int16_t *values) {
        if (meas_idx < steps && take_idx == 0)
            gen.setFrequency(f_list[meas_idx]);

        take_idx++;

        // we do not measure "latency" steps
        if (take_idx > latency) {
            float dc_offset = 0;

            // de-interleave channels, apply Hann window and get dc_offset estimate
            for (int i = 0; i < size; i++) {
                if (i % 2 == 0) {
                    int idx = i / 2;
                    float multiplier = 0.5f * (1 - cos(PI_2 * (float) idx / (float) in_buffer_size));
                    fft_in[idx] = multiplier * (float) values[i];
                    dc_offset += fft_in[idx];
                }
            }

            dc_offset /= (float) in_buffer_size;

            // remove dc offset
            for (int i = 0; i < in_buffer_size; i++) {
                fft_in[i] -= dc_offset;
            }

            kiss_fftr(fft, fft_in, fft_out);

            Measurement measurement(f_list[meas_idx], fft_out, fft_out_size, resolution);
            measurements.push_back(measurement);
        }

        if (take_idx >= takes) {
            meas_idx++;
            take_idx = 0; // reset takes counter, next f

            // we need to wait "latency" more steps to capture all measurements
            if (meas_idx >= steps) {
                reader.stop();
                gen.stop();
            }
        }
    });

    reader.start();
    gen.start();

    return measurements;
}

int DataAcquisition::getLatencyInSamples() {
    int samplesCount = 0;
    reader.registerCallback([this, &samplesCount](snd_pcm_uframes_t size, const int16_t *values) {
        if (samplesCount == 0)
            gen.setFrequency(3000);

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