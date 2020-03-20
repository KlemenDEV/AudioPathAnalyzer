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

#include "analyzer/DataAcquisition.h"
#include "analyzer/Smoothing.tcc"

int main() {
    DataAcquisition dataAcquisition;

    cout << "Press [Enter] to start calibration";
    getchar();

    vector<Measurement> calibrations = dataAcquisition.measure(MEAS_STEPS);

    cout << "Press [Enter] to start audio path measurement";
    getchar();

    vector<Measurement> measurements = dataAcquisition.measure(MEAS_STEPS);

    // obtain maximal amplitude and remove calibration amount from measurements
    float max_a = 0;
    for (auto & calibration : calibrations) {
        if (max_a < calibration.a)
            max_a = calibration.a;
    }

    vector<float> amplitudes(calibrations.size());
    vector<float> frequencies(calibrations.size());

    for (int i = 0; i < calibrations.size(); ++i) {
        if (calibrations[i].a != 0)
            calibrations[i].a = 20 * log10(calibrations[i].a / max_a);

        if (measurements[i].a != 0)
            measurements[i].a = 20 * log10(measurements[i].a / max_a);

        amplitudes[i] = measurements[i].a - calibrations[i].a;
        frequencies[i] = measurements[i].f;
    }

    amplitudes = Smoothing::smooth(amplitudes);

    for (int i = 0; i < amplitudes.size(); ++i) {
        cout << frequencies[i] << ", " << amplitudes[i] << endl;
    }
}