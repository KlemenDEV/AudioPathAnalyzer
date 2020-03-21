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

#ifndef AUDIOPATHANALYZER_ANALYSISRESULT_H
#define AUDIOPATHANALYZER_ANALYSISRESULT_H

#include <utility>
#include <vector>

using namespace std;

class AnalysisResult {

public:
    float dc_offset;
    vector<float> frequencies;
    vector<float> amplitudes;
    vector<float> thd_r;
    vector<float> thd_n;
    vector<float> thd_f;

    AnalysisResult(float dcOffset, vector<float> frequencies, vector<float> amplitudes,
                   vector<float> thdR, vector<float> thdN, vector<float> thdF) :
            dc_offset(dcOffset),
            frequencies(std::move(frequencies)),
            amplitudes(std::move(amplitudes)),
            thd_r(std::move(thdR)),
            thd_n(std::move(thdN)),
            thd_f(std::move(thdF)) {}

};


#endif
