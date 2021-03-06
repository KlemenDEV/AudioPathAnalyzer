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

#ifndef AUDIOPATHANALYZER_ANALYZER_H
#define AUDIOPATHANALYZER_ANALYZER_H

#include "DataAcquisition.h"
#include "Smoothing.tcc"

class Analyzer {

public:
    static Experiment
    analyzePath(DataAcquisition &dataAcquisition, Experiment &calibration, int lowf, int highf, int steps, bool smooth);

};


#endif
