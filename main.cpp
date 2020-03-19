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

#include <chrono>


int main() {
    DataAcquisition dataAcquisition;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    vector<Measurement> measurements = dataAcquisition.measure(200);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Measurement time = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;

    float max_a = 0;
    for (Measurement &m : measurements) {
        if (max_a < m.a)
            max_a = m.a;
    }

    for (Measurement &m : measurements) {
        cout << m.f << "," << 20 * log10(m.a / max_a) << endl;
    }
}