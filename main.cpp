#include "SineWaveGenerator.h"
#include "InputSignalReader.h"

#include "kiss_fftr.h"

int main() {
    SineWaveGenerator gen;

    InputSignalReader reader;

    snd_pcm_uframes_t in_buffer_size = reader.getBufferSize();

    size_t fft_out_size = in_buffer_size/2 + 1;

    kiss_fftr_cfg fft = kiss_fftr_alloc(in_buffer_size, 0, nullptr, nullptr);
    kiss_fft_cpx fft_out[fft_out_size];

    float resolution = (float) SAMPLE_RATE / (float) in_buffer_size;

    float f = 1;

    reader.registerCallback([&fft, &fft_out, resolution, &f, &gen](snd_pcm_uframes_t size, float *values) {
        kiss_fftr(fft, values, fft_out);

        float maxA = 0;
        int maxi = 0;
        for (int i = 0; i < size/2 + 1; i++) {
            float A = sqrt(fft_out[i].r*fft_out[i].r + fft_out[i].i*fft_out[i].i);
            if (A >= maxA) {
                maxA = A;
                maxi = i;
            }
        }

        f *= 1.1;
        gen.setFrequency(f);

        cout << ((float) maxi * resolution) << endl;
    });

    reader.start();
    gen.start();
}