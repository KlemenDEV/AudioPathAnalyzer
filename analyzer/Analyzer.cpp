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

#include "Analyzer.h"

AnalysisResult Analyzer::analyzePath(DataAcquisition &dataAcquisition, Experiment &calibration,
                                     int lowf, int highf, int steps) {
    Experiment measurement = dataAcquisition.measure(lowf, highf, steps);

    cout << "Analysis results below" << endl;
    cout << "======================" << endl << endl;

    float dc_offset = measurement.dc_offset - calibration.dc_offset;
    vector<float> frequencies(calibration.takes.size());
    vector<float> amplitudes(calibration.takes.size());
    vector<float> thd_r(calibration.takes.size());
    vector<float> thd_n(calibration.takes.size());
    vector<float> thd_f(calibration.takes.size());

    cout << "DC offset: " << dc_offset << " V" << endl;

    // obtain maximal amplitude
    float max_a = 0;
    for (auto &c : calibration.takes) {
        if (max_a < c.a)
            max_a = c.a;
    }

    for (int i = 0; i < amplitudes.size(); ++i) {
        if (calibration.takes[i].a != 0)
            calibration.takes[i].a = 20 * log10(calibration.takes[i].a / max_a);
        if (measurement.takes[i].a != 0)
            measurement.takes[i].a = 20 * log10(measurement.takes[i].a / max_a);
        amplitudes[i] = measurement.takes[i].a - calibration.takes[i].a;

        thd_r[i] = measurement.takes[i].thd_r - calibration.takes[i].thd_r;
        thd_n[i] = measurement.takes[i].thd_n - calibration.takes[i].thd_n;
        thd_f[i] = measurement.takes[i].thd_f - calibration.takes[i].thd_f;

        frequencies[i] = measurement.takes[i].f;
    }

    amplitudes = Smoothing::smooth(amplitudes);

    cout << "------ Amplitude data" << endl;
    for (int i = 0; i < amplitudes.size(); ++i) {
        cout << frequencies[i] << ", " << amplitudes[i] << endl;
    }

    AnalysisResult result(dc_offset, frequencies, amplitudes, thd_r, thd_n, thd_f);
    return result;
}
