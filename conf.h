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

#ifndef AUDIOPATHANALYZER_CONF_H
#define AUDIOPATHANALYZER_CONF_H

#define SAMPLE_RATE 192000
#define SIGNAL_BITS SND_PCM_FORMAT_S24
#define SIGNAL_HALF_RANGE 8388607

#define MAX_ADC_V 1.767767

#define OUT_DEVICE "hw:0,0"
#define IN_DEVICE "hw:0,0"

#define MEAS_F_LOW 20
#define MEAS_F_HIGH 20000
#define MEAS_STEPS 200

#endif
