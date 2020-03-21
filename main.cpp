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

    Experiment calibration = dataAcquisition.measure(MEAS_STEPS);

    cout << "Press [Enter] to start audio path measurement";
    getchar();

    Experiment measurement = dataAcquisition.measure(MEAS_STEPS);

    // obtain maximal amplitude
    float max_a = 0;
    for (auto &c : calibration.takes) {
        if (max_a < c.a)
            max_a = c.a;
    }

    vector<float> amplitudes(calibration.takes.size());
    vector<float> frequencies(calibration.takes.size());

    for (int i = 0; i < amplitudes.size(); ++i) {
        if (calibration.takes[i].a != 0)
            calibration.takes[i].a = 20 * log10(calibration.takes[i].a / max_a);

        if (measurement.takes[i].a != 0)
            measurement.takes[i].a = 20 * log10(measurement.takes[i].a / max_a);

        amplitudes[i] = measurement.takes[i].a - calibration.takes[i].a;
        frequencies[i] = measurement.takes[i].f;
    }

    amplitudes = Smoothing::smooth(amplitudes);

    for (int i = 0; i < amplitudes.size(); ++i) {
        cout << frequencies[i] << ", " << amplitudes[i] << endl;
    }
}