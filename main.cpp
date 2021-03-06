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

#define RETVAL_PARAMETER_MISMATCH -1
#define RETVAL_FILE_NOT_FOUND -2

#define IS_CALIBRATION_INVALID (calibration.min_f != f_low->value() || calibration.max_f != f_high->value() || calibration.steps != steps->value())

using namespace std;
using namespace popl;

int main(int argc, char **argv) {
    OptionParser op("Allowed options");

    auto help_option = op.add<Switch>("h", "help", "Prints usage");

    auto calibration_option = op.add<Value<string>>("f", "calibration",
                                                    "File containing calibration data or where to store it in");
    auto docalibration_option = op.add<Switch>("c", "docalibration",
                                               "Run calibration and store it in -calibration <file>");
    auto verify_calibration_option = op.add<Switch>("v", "verifycalibration",
                                                    "Only verify if calibration file is valid for the given parameters");

    auto f_low = op.add<Value<int>>("", "frequency_low", "Sweep start frequency", MEAS_F_LOW);
    auto f_high = op.add<Value<int>>("", "frequency_high", "Sweep end frequency", MEAS_F_HIGH);
    auto steps = op.add<Value<int>>("s", "steps", "Sweep frequency steps", MEAS_STEPS);

    auto no_smooth = op.add<Switch>("n", "nosmooth",
                                    "Disable frequency response smoothing");

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

    if (verify_calibration_option->is_set()) {
        if (!calibrationFileName.empty() && filesystem::exists(calibrationFileName)) {
            calibration = Experiment::read(calibrationFileName);
            if (IS_CALIBRATION_INVALID) {
                cout << "VERIFY: parameter_mismatch" << endl;
                return RETVAL_PARAMETER_MISMATCH;
            }
            cout << "VERIFY: valid" << endl;
            return 0;
        } else {
            cout << "VERIFY: file_not_found" << endl;
            return RETVAL_FILE_NOT_FOUND;
        }
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

    if (IS_CALIBRATION_INVALID) {
        cerr << "Calibration data not compatible with specified measurement parameters!" << endl;
        return RETVAL_PARAMETER_MISMATCH;
    }

    Experiment experiment = Analyzer::analyzePath(dataAcquisition, calibration, f_low->value(),
                                                  f_high->value(), steps->value(), !no_smooth->is_set());

    cout << "#CSV" << endl;
    Experiment::write(experiment, cout);
}