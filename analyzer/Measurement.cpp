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
#include "../conf.h"

Measurement::Measurement(float gen_f, kiss_fft_cpx *fft_out, size_t fft_out_size, float resolution) {
    this->f = gen_f;

    // calculate amplitude spectrum
    float energy = 0;
    float aspectr[fft_out_size + 1]; // +1 to make the peak detection work for the max f too
    for (int i = 0; i < fft_out_size; i++) {
        aspectr[i] = sqrt(fft_out[i].r * fft_out[i].r + fft_out[i].i * fft_out[i].i) / fft_out_size;
        energy += aspectr[i];
    }

    // extract harmonics
    float harmonicsSquareSum = 0;
    float signalSquareSum = 0;
    int fundamentalIdx = 0;

    int harmonics_count = (int) (SAMPLE_RATE / gen_f);
    for (int hi = 1; hi <= harmonics_count; hi++) { // iterate through harmonic multiplies
        float fh = (float) hi * gen_f;
        int idx = (int) (fh / resolution);

        for (int j = -1; j <= 1; j++) { // search for peak in [idx - 1, idx + 1] range
            int i = idx + j;

            // bounds check
            if (i < 1) i = 1;
            if (i > fft_out_size - 1) i = (int) fft_out_size - 1;

            if (aspectr[i - 1] <= aspectr[i] && aspectr[i + 1] <= aspectr[i]) { // make sure it is peak
                peak_t peak = fft_interpolate_peak(i, resolution, aspectr[i - 1], aspectr[i], aspectr[i + 1]);

                signalSquareSum += peak.a * peak.a;

                if (hi == 1) {
                    this->valid = true;
                    this->a = peak.a;
                    fundamentalIdx = i;
                } else {
                    harmonicsSquareSum += peak.a * peak.a;
                }

                break; // peak was found, to the next harmonic
            }
        }
    }

    if (!this->valid) { // if we did not find main f peak, we "fallback" to direct bin value
        fundamentalIdx = (int) round(gen_f / resolution);
        this->a = aspectr[fundamentalIdx];
    }

    float noiseAndHarmonicsSum = energy - aspectr[fundamentalIdx];
    if (fundamentalIdx > 0)
        noiseAndHarmonicsSum -= aspectr[fundamentalIdx - 1];
    if (fundamentalIdx < fft_out_size - 1)
        noiseAndHarmonicsSum -= aspectr[fundamentalIdx + 1];
    this->thd_n = noiseAndHarmonicsSum / (energy - noiseAndHarmonicsSum);

    harmonicsSquareSum = sqrt(harmonicsSquareSum);
    signalSquareSum = sqrt(signalSquareSum);
    this->thd_f = harmonicsSquareSum / this->a;
    this->thd_r = harmonicsSquareSum / signalSquareSum;
}

peak_t Measurement::fft_interpolate_peak(int bin_idx, float resolution, float a, float b, float c) {
    float p = 0.5f * (a - c) / (a - 2 * b + c);
    float f = ((float) bin_idx + p) * resolution;
    float amplitude = b - 0.25f * (a - c) * p;
    return {f, amplitude};
}