clear all; close all; clc;

inFile = '/home/k-jax/Desktop/rawOut.csv';
f = fopen(inFile);

A = csvread(f);
bb = reshape(A,1,[]);
b = bb;%(end-200*4096:end);
NFFT = 2*4096;
t = [0:NFFT-1]/NFFT;
figure;
plot(t,20*log10(abs(fft(b,NFFT))));
figure;
stem(b);
fclose(f);
