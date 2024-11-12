import matplotlib.pyplot as plt
from figure_utils import *

audio_duration = 4
num_bars = 2
odf_peak_indices = [0, 17, 32, 50, 64, 69, 81, 96, 128, 145, 160, 178, 192, 197,
                    209, 224, 256, 273, 288, 306, 320, 352, 384, 401, 416, 434, 448, 453, 465, 480,]
odf = [25.655, 13.5228, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3.20466, 0.827149, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15.946, 8.06438, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2.06568, 0.54126, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8.69268, 66.6699, 15.2066, 0, 0, 0, 0.866541, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.85089, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31.2806, 12.4258, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32.473, 21.503, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3.21055, 0.972378, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18.3127, 9.87289, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2.11299, 3.03164, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.534349, 58.5189, 16.6307, 0, 0, 0, 2.2355, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.605455, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14.2423, 4.30755, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 27.7916, 19.0319, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.394675, 0.105963, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26.2863, 19.5643, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.56024, 3.00088, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.68536, 66.3529, 22.5585, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8.51002, 5.65104, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23.2697, 19.1935, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.06358, 2.74081, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22.8635, 8.34654, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.38917, 1.87906, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60.9875, 23.3613, 0, 0, 0, 2.3508, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.2826, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10.6984, 2.47446, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,]
peak_values = [odf[i] for i in odf_peak_indices]
odfSr = len(odf) / audio_duration

tatum_count_hypotheses = [24, 36, 32]
t = [i / odfSr for i in range(len(odf))]

# for each tatum count hypothesis, plot the tatums as vertical lines
for tatum_count in tatum_count_hypotheses:
    plt.figure(1)
    plt.plot(t, odf)

    tatums_per_second = tatum_count / audio_duration
    peak_distances = []
    for peak_index in odf_peak_indices:
        peak_pos = peak_index / len(odf)
        nearest_tatum_pos = int(peak_pos * tatum_count + 0.5) / tatum_count
        left = min(nearest_tatum_pos, peak_pos) * audio_duration
        right = max(nearest_tatum_pos, peak_pos) * audio_duration
        plt.fill([left, right, right, left], [0, 0, odf[peak_index],
                 odf[peak_index]], alpha=0.8, color="tab:red")
        peak_distances.append(
            2 * abs(peak_pos - nearest_tatum_pos) * tatum_count)
    # weighted average is inner product of peak_distances and odf peak values
    weighted_average = sum(
        [peak_distances[i] * peak_values[i] for i in range(len(odf_peak_indices))]) / sum(peak_values)

    for i in range(tatum_count):
        plt.axvline(x=i / tatums_per_second, color='g', linestyle='--')
    plt.xlabel("Time (s)", fontsize=15, color="white")
    apply_style(plt)
    if tatum_count == 24:
      # provisionally set xlim [16, 22] and save the figure - it's particularly informative
      # Get current xlim:
      xlim = plt.xlim()
      ylim = plt.ylim()
      plt.xlim([1.6, 2.2])
      plt.ylim([0, 32])
      save_figure(plt, "tatum_quantization_fit_24_zoomed.svg")
      plt.xlim(xlim)
      plt.ylim(ylim)
    plt.title(str(int(tatum_count / num_bars)) +
              " tatums per bar, error = " + str(round(weighted_average*100)/100), fontsize=15, color="white")
    # plt.show()
    save_figure(plt, "tatum_quantization_fit_" + str(tatum_count) + ".svg")
    plt.close(1)
