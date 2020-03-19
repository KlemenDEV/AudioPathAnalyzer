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
    this->f = gen_f;

    // calculate amplitude spectrum
    float energy = 0;
    float aspectr[fft_out_size + 1];
    for (int i = 0; i < fft_out_size; i++) {
        aspectr[i] = sqrt(fft_out[i].r * fft_out[i].r + fft_out[i].i * fft_out[i].i) / fft_out_size;
        energy += aspectr[i];
    }

    aspectr[fft_out_size] = 0; // to make the peak detection work for the max f too

    // determine spectrum peaks
    float max_f = 0;
    vector<peak_t> peaks;
    float limit = (energy / fft_out_size) / 32.f;
    for (int i = 1; i < fft_out_size; i++) {
        if (aspectr[i - 1] <= aspectr[i] && aspectr[i + 1] <= aspectr[i]) { // peak
            if (aspectr[i] > limit) { // major peaks only
                peak_t peak = fft_interpolate_peak(i, resolution, aspectr[i - 1], aspectr[i], aspectr[i + 1]);
                peaks.push_back(peak);

                if (peak.f > max_f)
                    max_f = peak.f;
            }
        }
    }

    // now we try to match peaks with multiplies of gen_f
    vector<peak_t> harmonics;
    for (int i = 1; i <= (int) ceil(max_f / gen_f); i++) {
        float hf = gen_f * (float) i;
        for (peak_t &p : peaks) {
            if (abs(p.f - hf) < resolution) {
                p.f = hf;
                harmonics.push_back(p);
                break;
            }
        }
    }

    float harmonicsSquareSum = 0;
    float signalSquareSum = 0;

    // calculate some signal statistics
    bool first = true;
    for (peak_t &hp : harmonics) {
        signalSquareSum += hp.a * hp.a;
        if (!first)
            harmonicsSquareSum += hp.a * hp.a;
        else
            first = false;
    }

    harmonicsSquareSum = sqrt(harmonicsSquareSum);
    signalSquareSum = sqrt(signalSquareSum);

    this->a = harmonics[0].a;
    this->thd_f = harmonicsSquareSum / harmonics[0].a;
    this->thd_r = harmonicsSquareSum / signalSquareSum;
}

peak_t Measurement::fft_interpolate_peak(int bin_idx, float resolution, float a, float b, float c) {
    float p = 0.5f * (a - c) / (a - 2 * b + c);
    float f = ((float) bin_idx + p) * resolution;
    float amplitude = b - 0.25f * (a - c) * p;
    return {f, amplitude};
}