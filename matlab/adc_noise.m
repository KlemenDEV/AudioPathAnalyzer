clear fft;

Fs = 44000;            % Sampling frequency                    
T = 1/Fs;             % Sampling period       
L = 1500;             % Length of signal
t = (0:L-1)*T;        % Time vector

resolution = 16;

S = sin(2*pi*5000*t);

% dac
S = imquantize(S, -1:(1/(2^resolution)):1);
S = S / 2^resolution - 1;

% adc
S = imquantize(S, -1:(1/(2^resolution)):1);
S = S / 2^resolution - 1;

X = S + randn(size(t));

Y = fft(X);

P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);

f = Fs*(0:(L/2))/L;

mean(P1)

plot(f,P1) 
title('Single-Sided Amplitude Spectrum of X(t)')
xlabel('f (Hz)')
ylabel('|P1(f)|')
