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

#include <Analyzer.h>

int main() {
    DataAcquisition dataAcquisition;

    cout << "Press [Enter] to start calibration";
    getchar();

    Experiment calibration = dataAcquisition.measure(MEAS_STEPS);

    cout << "Press [Enter] to start audio path analyzer";
    getchar();

    Analyzer::analyzePath(dataAcquisition, calibration);
}