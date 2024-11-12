import matplotlib.pyplot as plt
from figure_utils import *

samples = dict()

samples["Wood Snare"] = {
    "wavFile": "C:/Users/saint/git/github/saintmatthieu/loop-tempo-estimator/tests/benchmarking/dataset/non-loops/Wood Snare (Sample 3)wav.mp3",
    "odfSr": 99.7089,
    "isSingleEvent": 1,
    "kurtosis": 3236.83,
    "odf": [0, 0, 60.5959, 70.7098, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0132551, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.000394374, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.00171733, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,],
}

samples["You won FX"] = {
    "wavFile": "C:/Users/saint/git/github/saintmatthieu/loop-tempo-estimator/tests/benchmarking/dataset/non-loops/win.mp3",
    "odfSr": 100,
    "isSingleEvent": 1,
    "kurtosis": 23.9323,
    "odf": [0, 0, 0, 0, 0, 0, 0, 0, 0.875891, 2.91419, 0, 0, 0, 0, 0, 0, 0, 3.37298, 0.14373, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.32105, 1.92147, 0, 0, 0, 0, 0, 0, 0, 3.39955, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3.02316, 0.638544, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.43056, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.00928068, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0740337, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.1359, 0, 0, 0, 0, 0, 0, 0.00671458, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0994443, 0.150686, 0.0498537, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.020595, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0.0105242, 0.00311206, 0.0203498, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.00707472, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.00436415, 0, 0, 0.000755576, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.00996544, 0.0116917, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,],
}

samples["Drumloop"] = {
    "wavFile": "C:/Users/saint/git/github/saintmatthieu/loop-tempo-estimator/tests/benchmarking/dataset/loops/[Drumloop] 120 bpm edm drum loop 010.mp3",
    "odfSr": 100.014,
    "isSingleEvent": 0,
    "kurtosis": 1.70426,
    "odf": [0, 0, 0, 0, 0, 28.6944, 4.54258, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42.1017, 34.6049, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38.7379, 31.0475, 0, 0, 0.368948, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36.5293, 31.5619, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28.6641, 5.03593, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42.242, 35.6182, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38.2427, 31.0483, 0, 0, 0.566748, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35.5198, 32.5113, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,]
}

plt.figure(1)
sample = samples["Wood Snare"]
odfSr = sample["odfSr"]
odf = sample["odf"]
N = len(odf)
# rotate odf by 50 samples
odf = odf[N-50:] + odf[:N-50]
t = [i/odfSr for i in range(len(odf))]
t1 = 53 / odfSr
t2 = t1 + len(odf) / odfSr / 2
plt.plot(t, odf)
# plot a green vertical line at t1 and t2
plt.axvline(t1, color="green", linestyle="--")
plt.axvline(t2, color="green", linestyle="--")
plt.xlabel("Time (s)")
plt.title("2 tatums per bar, error = 0.0")
apply_style(plt)
save_figure(plt, "single_event_perfect_fit.svg")

plt.figure(2)
p = 1
for sample in samples:
    odfSr = samples[sample]["odfSr"]
    isSingleEvent = samples[sample]["isSingleEvent"]
    kurtosis = samples[sample]["kurtosis"]
    odf = samples[sample]["odf"]

    t = [i/odfSr for i in range(len(odf))]
    # plot odf
    plt.subplot(len(samples), 1, p)
    plt.plot(t, odf)
    plt.xlim(0, t[-1])

    # Display the kurtosis to one decimal place in the top right corner
    plt.text(t[-1], max(odf),
             f"Kurtosis: {kurtosis:.1f} ", ha="right", va="top", color="white")
    # display the name of the sample as text box in the top middle
    plt.text(t[-1]/2, max(odf), sample, ha="center", va="top", color="white")

    plt.xticks([])
    plt.yticks([])

    apply_style(plt)
    p += 1

save_figure(plt, "is_single_event.svg")
# plt.show()
