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

#include "popl.hpp"
#include "Analyzer.h"

#include <filesystem>

using namespace std;
using namespace popl;

int main(int argc, char **argv) {
    OptionParser op("Allowed options");

    auto help_option = op.add<Switch>("h", "help", "Prints usage");

    auto calibration_option = op.add<Value<string>>("f", "calibration",
                                                    "File containing calibration data or where to store it in");
    auto docalibration_option = op.add<Switch>("c", "docalibration",
                                               "Run calibration and store it in -calibration <file>");

    auto f_low = op.add<Value<int>>("", "frequency_low", "Sweep start frequency", MEAS_F_LOW);
    auto f_high = op.add<Value<int>>("", "frequency_high", "Sweep end frequency", MEAS_F_HIGH);
    auto steps = op.add<Value<int>>("s", "steps", "Sweep frequency steps", MEAS_STEPS);

    op.parse(argc, argv);

    if (help_option->is_set()) {
        cout << op.help(Attribute::expert) << endl;
        return 0;
    }

    Experiment calibration;
    DataAcquisition dataAcquisition;

    if (docalibration_option->is_set()) {
        if (calibration_option->is_set()) {
            cout << "Calibration will be performed" << endl;
            calibration = dataAcquisition.measure(f_low->value(), f_high->value(), steps->value());
            Experiment::write(calibration, calibration_option->value());
        } else {
            cout << "Specify calibration output file" << endl << endl;
            cout << op.help(Attribute::expert) << endl;
        }
        return 0;
    }

    std::string calibrationFileName;

    if (calibration_option->is_set()) {
        calibrationFileName = calibration_option->value();
    }

    if (!calibrationFileName.empty() && filesystem::exists(calibrationFileName)) {
        calibration = Experiment::read(calibrationFileName);
    } else {
        cout << "Press [Enter] to start calibration";
        getchar();

        calibration = dataAcquisition.measure(f_low->value(), f_high->value(), steps->value());

        cout << "Press [Enter] to start audio path analyzer";
        getchar();
    }

    Analyzer::analyzePath(dataAcquisition, calibration, f_low->value(),
                          f_high->value(), steps->value());
}