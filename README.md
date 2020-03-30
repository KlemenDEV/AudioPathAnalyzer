# Audio path analyzer

This project aims to develop an audio analyzer for analyzing audio transport path quality using criteria such as:
* First-order frequency response
* Harmonic distortion properties (THD)
* (DC offset)

It is written for Linux ALSA audio subsystem, so ALSA-dev is required for building:

`apt install libasound2-dev`

# Usage

```
Allowed options:
  -h, --help                     Prints usage
  -f, --calibration arg          File containing calibration data or where to store it in
  -c, --docalibration            Run calibration and store it in -calibration <file>
  -v, --verifycalibration        Only verify if calibration file is valid for the given parameters
  --frequency_low arg (=20)      Sweep start frequency
  --frequency_high arg (=20000)  Sweep end frequency
  -s, --steps arg (=200)         Sweep frequency steps
```

# Concept

Frequency response is determined by:
1. Sweeping across the range of sound card using logarithmic scale
2. Sampling recorded input into a buffer
3. Windowing the samples and removing DC offset
4. FFT-ing the data
5. Detecting peaks
6. Correlating peaks to the harmonics of the measured frequency
7. Interpolating FFT bins using parabolic interpolation
8. Obtaining 0-th harmonic frequency and its amplitude
9. Calculating THD as additional data

Some tests of this concept can be found in the matlab folder.

# Cloning

To clone the submodules too, use:

`git clone --recursive https://github.com/KlemenDEV/AudioPathAnalyzer.git`
