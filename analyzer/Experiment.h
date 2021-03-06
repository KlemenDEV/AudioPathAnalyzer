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

#ifndef AUDIOPATHANALYZER_EXPERIMENT_H
#define AUDIOPATHANALYZER_EXPERIMENT_H

#include <utility>
#include <fstream>

#include "csv.h"
#include "Measurement.h"

class Experiment {

public:
    vector<Measurement> takes;
    float dc_offset = 0;
    int invalidCount = 0;
    int measurementTime = 0;

    int steps;
    int min_f;
    int max_f;

    Experiment(vector<Measurement> takes, float dcOffset, int invalidCount, int measurementTime, int steps, int min_f,
               int max_f) :
            takes(std::move(takes)),
            dc_offset(dcOffset),
            invalidCount(invalidCount),
            measurementTime(measurementTime),
            steps(steps),
            min_f(min_f),
            max_f(max_f) {}

    Experiment() = default;

    static Experiment read(const std::string &fileName) {
        Experiment e;
        io::CSVReader<4> in(fileName);

        int valid;
        float f, a, thd_f;

        bool first = true;
        while (in.read_row(f, a, thd_f, valid)) {
            if (first) {
                e.invalidCount = (int) a;
                e.dc_offset = thd_f;
                first = false;
            } else {
                Measurement measurement;
                measurement.f = f;
                measurement.a = a;
                measurement.thd_f = thd_f;
                measurement.valid = valid == 1;
                e.takes.push_back(measurement);
            }
        }

        e.steps = e.takes.size();
        e.min_f = (int) e.takes[0].f;
        e.max_f = (int) e.takes[e.takes.size() - 1].f;

        return e;
    }

    static void write(Experiment &e, const std::string &fileName) {
        ofstream experimentFile(fileName, fstream::trunc);
        if (experimentFile.is_open()) {
            write(e, experimentFile);
            experimentFile.close();
        } else {
            cerr << "Failed to write experiment to the file!" << endl;
        }
    }

    static void write(Experiment &e, std::ostream &writestream = std::cout) {
        writestream << e.takes.size() << "," << e.invalidCount << "," << e.dc_offset << ",1" << endl;
        for (Measurement &m : e.takes) {
            writestream << m.f << "," << m.a << "," << m.thd_r << "," << m.valid << endl;
        }
    }

};


#endif
