#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from matplotlib import pyplot as plt
import scipy.fftpack

origLeft, dftLeft, origRight, dftRight = [], [], [], []
freq_buckets = []
size = 0
sample_rate = 44100
num_samples = 5000

# generate bucket for frequencies
for x in range(int(-num_samples/2), int(num_samples/2)):
    freq_buckets.append(x * sample_rate / num_samples)

for line in open('C:\\Users\\fahas\\Desktop\\Masterarbeit\\cuda\\Master_parallel_fourier_audio\\out\\dftplots2.m', 'r'):

    try:
        values = [float(s) for s in line.split(' ')]
        origLeft.append(values[0])
        dftLeft.append(values[1])
        origRight.append(values[2])
        dftRight.append(values[3])
        size += 1
    except ValueError:
        break

dftLeft[0] = 0
dftRight[0] = 0

maxLeft = max(dftLeft)
maxIndex = dftLeft.index(max(dftLeft))
freq = maxIndex * sample_rate / num_samples

maxRight = max(dftRight)
maxIndexR = dftRight.index(max(dftRight))
freqR = maxIndexR * sample_rate / num_samples


fs_rate = 44100
secs = size / float(fs_rate)
Ts = 1.0/fs_rate  # sampling interval in time
t = scipy.arange(0, secs, Ts) # time vector as scipy arange field / numpy.ndarray


posL = dftLeft[0:num_samples//2]
negL = dftLeft[num_samples//2:num_samples]
posR = dftRight[0:num_samples//2]
negR = dftRight[num_samples//2:num_samples]
outLeft = []
outRight = []

# for x in range(num_samples//2 - 1, -1, -1):
for x in range(0, num_samples//2, 1):
    outRight.append(negR[x])
    outLeft.append(negL[x])

for x in range(0, num_samples//2, 1):
    outRight.append(posR[x])
    outLeft.append(posL[x])


plt.subplot(511)
p1 = plt.plot(t, origLeft, "g")  # plotting the signal
p1 = plt.plot(t, origRight, "r")
plt.xlabel('x')
plt.ylabel('original signal')

plt.subplot(512)
p2 = plt.stem(freq_buckets, outRight)  # plotting the signal
plt.xlabel('magn')
plt.ylabel('right freq')

plt.subplot(513)
p5 = plt.stem(freq_buckets, outLeft)
plt.xlabel('frequency')
plt.ylabel('magn')

plt.subplot(514)
p3 = plt.plot(dftLeft, "r")
plt.xlabel('x')
plt.ylabel('leftdft')

plt.subplot(515)
p4 = plt.plot(dftRight, "b") # plotting the complete fft spectrum
plt.xlabel('x')
plt.ylabel('rightdft')




# scipy.fftpack.fftshift()


plt.show()
