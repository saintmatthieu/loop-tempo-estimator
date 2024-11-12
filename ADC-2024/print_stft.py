import numpy as np
import matplotlib.pyplot as plt
from stft_log import stft, sampleRate, frameRate, rawOdf, odf, movingAverage
from figure_utils import *

# transpose stft so that it's rotated 90 degrees clockwise
stft = np.transpose(stft)
fftSize = 2 * (stft.shape[0] - 1)
fLim = 10000
K = int(np.ceil(fftSize * fLim / sampleRate))

numStfts = stft.shape[1]

N = numStfts / 8
n = np.arange(0, numStfts + N, N)
F = fLim / 5
f = np.arange(0, fLim + F, F)

plt.imshow(np.abs(stft), aspect='auto', cmap='inferno', origin='lower')

plt.gca().set_xticks([])
plt.gca().set_yticks([])
plt.xlim([100, 110])
plt.ylim([0, 30])
apply_style(plt)
save_figure(plt, "py_stft_zoomed.svg")

# add a green rectangle spanning the

# add time and frequency axes
plt.xlabel('Time (s)')
plt.ylabel('Frequency (kHz)')
plt.ylim(0, K)
plt.xticks(n, np.round(n / frameRate, 1))
plt.yticks(f * fftSize / sampleRate, [int(np.round(f / 1000)) for f in f])

# plt.show()
apply_style(plt)
save_figure(plt, "py_stft.svg")

# Add a layer to the same figure that plots the ODF, time-scaled so that it fits the STFT
n_odf = np.arange(0, len(odf)) / len(odf) * numStfts
odfScal = K / np.max(np.abs(rawOdf)) * 0.9
rawOdfLines = plt.plot(n_odf, np.array(rawOdf) *
                       odfScal, color='white', linewidth=1)
apply_style(plt)
save_figure(plt, "py_stft_with_rawOdf.svg")

movingAvgLines = plt.plot(n_odf, np.array(
    movingAverage) * odfScal, color='white', linewidth=3, alpha=0.8)
apply_style(plt)
save_figure(plt, "py_stft_with_movingAverage.svg")

for line in rawOdfLines:
    line.remove()
for line in movingAvgLines:
    line.remove()

plt.plot(n_odf, np.array(odf) * odfScal, color='white', linewidth=1)
apply_style(plt)
save_figure(plt, "py_stft_with_odf.svg")

tatumCount = 32
# 32 evenly spaced vertical lines
for i in range(tatumCount):
    plt.axvline(i * numStfts / tatumCount, color='green', linestyle='--', linewidth=1)
apply_style(plt)
save_figure(plt, "py_stft_with_tatum_grid.svg")

# print just the odf now:
plt.figure()
plt.plot(n_odf, np.array(odf) * odfScal, color='white', linewidth=1)
plt.xticks([])
plt.yticks([])
plt.gca().spines['right'].set_visible(False)
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['left'].set_visible(False)
plt.gca().spines['bottom'].set_visible(False)
apply_style(plt)
save_figure(plt, "py_odf.svg")
