import numpy as np
import matplotlib.pyplot as plt
from figure_utils import *
import os
from pydub import AudioSegment

# our signal is the first second of "./483494__josefpres__disco-guitar-loop-120-bpm.mp3"
path = os.path.join(os.path.dirname(__file__),
                    "483494__josefpres__disco-guitar-loop-120-bpm.mp3")
audio = AudioSegment.from_file(path)
x = np.array(audio.get_array_of_samples())
x = x[:44000]  # first second
# aggressively downsample, decimating by a factor of 100
x = x[::100]
x = x / np.max(np.abs(x)) * 0.9
N = len(x)
W = 5  # number of windows
H = N / W  # hop size
M = 230  # window size

plt.figure(figsize=(10, 6))

# keep only the waveform: remove x-axis, y-axis, labels, too, and top and right spines
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)
plt.gca().spines['left'].set_visible(False)
plt.gca().spines['bottom'].set_visible(False)
plt.gca().set_xticks([])
plt.gca().set_yticks([])

plt.xlim([-N/20, N*21/20])
plt.ylim([-1.1, 1.1])

plt.plot(x, color='white', zorder=2, alpha=0.9)
apply_style(plt)
save_figure(plt, "circular_stft_0.svg")

# plot W evenly-spaced vertical lines
colors = [plt.cm.viridis(i/W) for i in range(W)]

for i in range(W+1):
    plt.axvline(i*H, color=colors[i % W], linestyle='--', linewidth=1)
# Place "<- ~10ms ->" (but with proper arrow characters) as text box between 0th and 1st vertical lines
plt.text(H/2, -0.8, "\u219010ms\u2192", color='white', fontsize=20, ha='center')
apply_style(plt)
save_figure(plt, "circular_stft_1.svg")

# Draw W + 1 hanning windows of length M with hop size H, each time picking a new color, the last one with dashed lines
w = np.hanning(M)
grains = []
for i in range(W):
    end = min(i*H + M, N)
    K = int(end - i*H)
    plt.plot(np.arange(i*H, end), w[:K], color=colors[i], zorder=3, linewidth=4)
    if K < M:
        plt.plot(np.arange(0, M-K), w[K:], color=colors[i], zorder=3, linewidth=4)
    apply_style(plt)
    save_figure(plt, "circular_stft_{}.svg".format(i+2))

plt.figure(figsize=(10, 6))
