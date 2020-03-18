fft = [];

load ftt_441_49 % this data is for f_gen = 441.49 measured on 16bits/48kHz

res = 48000 / 2048;

stem((0:(size(fft,1)-1)) * res, fft)

hold on;

avg = mean(fft) / 4;

for i=2:(size(fft,1)-1)
    if(fft(i-1) <= fft(i) && fft(i+1) <= fft(i) && fft(i) > avg)
        stem((i-1) * res, fft(i), 'LineStyle', 'none', 'color', 'red')
    end
end

hold off;