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

Experiment Analyzer::analyzePath(DataAcquisition &dataAcquisition, Experiment &calibration,
                                 int lowf, int highf, int steps) {
    Experiment measurement = dataAcquisition.measure(lowf, highf, steps);

    cout << "Analysis complete" << endl;

    float dc_offset = measurement.dc_offset - calibration.dc_offset;

    cout << "DC offset: " << dc_offset << " V" << endl;

    // obtain maximal amplitude
    float max_a = 0;
    for (auto &c : calibration.takes) {
        if (max_a < c.a)
            max_a = c.a;
    }

    for (int i = 0; i < measurement.takes.size(); ++i) {
        if (calibration.takes[i].a != 0)
            calibration.takes[i].a = 20 * log10(calibration.takes[i].a / max_a);
        
        if (measurement.takes[i].a != 0)
            measurement.takes[i].a = 20 * log10(measurement.takes[i].a / max_a);

        measurement.takes[i].a = measurement.takes[i].a - calibration.takes[i].a;
    }

    measurement.takes = Smoothing::smooth(measurement.takes, &Measurement::a);

    measurement.dc_offset = dc_offset;

    return measurement;
}
