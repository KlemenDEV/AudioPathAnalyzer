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

#ifndef AUDIOPATHANALYZER_SMOOTHING_TCC
#define AUDIOPATHANALYZER_SMOOTHING_TCC

#include <vector>
#include "Measurement.h"

using namespace std;

class Smoothing {

public:
    template<typename T>
    static vector<T> smooth(vector<T> &data) {
        vector<T> retval(data.size());

        for (int i = 0; i < data.size(); ++i) {
            if (i == 0) {
                retval[i] = (data[i] * 3 + data[i + 1] * 2 + data[i + 2]) / 6;
            } else if (i == 1) {
                retval[i] = (data[i - 1] * 2 + data[i] * 3 + data[i + 1] * 2 + data[i + 2]) / 8;
            } else if (i == data.size() - 2) {
                retval[i] = (data[i - 2] + data[i - 1] * 2 + data[i] * 3 + data[i + 1] * 2) / 8;
            } else if (i == data.size() - 1) {
                retval[i] = (data[i - 2] + data[i - 1] * 2 + data[i] * 3) / 6;
            } else {
                retval[i] = (data[i - 2] + data[i - 1] * 2 + data[i] * 3 + data[i + 1] * 2 + data[i + 2]) / 9;
            }
        }

        return retval;
    }

};


#endif