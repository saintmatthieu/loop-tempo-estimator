import matplotlib.pyplot as plt
from figure_utils import *

possible_num_bars = range(2, 5)
possible_tatums_per_bar = [1, 2, 4, 8, 12, 16, 24]
audio_file_duration = 5
min_tatums_per_minute = 12.5
max_tatums_per_minute = 700

tatum_count_set = set()
for num_bars in possible_num_bars:
    for tatums_per_bar in possible_tatums_per_bar:
        tatum_count_set.add(num_bars * tatums_per_bar)
num_colors = len(tatum_count_set)
colorMap = [plt.cm.get_cmap("tab20")(i % 20) for i in range(num_colors)]
tatum_counts = list(tatum_count_set)


def make_figure(bar_large_tatum_counts):
    plt.figure(figsize=(10, 6))
    for i, num_bars in enumerate(possible_num_bars):
        for j, tatums_per_bar in enumerate(possible_tatums_per_bar):
            tatum_count = num_bars * tatums_per_bar
            squareColor = colorMap[tatum_counts.index(tatum_count)]
            plt.fill([j, j + 1, j + 1, j],
                     [i, i, i + 1, i + 1], color=squareColor)
            tatums_per_minute = 60 * tatum_count / audio_file_duration
            if bar_large_tatum_counts and (tatums_per_minute < min_tatums_per_minute or tatums_per_minute > max_tatums_per_minute):
                plt.plot([j, j + 1], [i, i + 1], color="black", lw=4)
                plt.plot([j, j + 1], [i + 1, i], color="black", lw=4)
            plt.text(j + 0.5, i + 0.5, str(tatum_count),
                     color="white", ha='center', va='center', fontsize=15)

    plt.xlim([0, len(possible_tatums_per_bar)])
    plt.ylim([0, len(possible_num_bars)])
    # x- and y-ticks are centered on the squares
    plt.xticks([i + 0.5 for i in range(len(possible_tatums_per_bar))],
               possible_tatums_per_bar)
    plt.yticks([i + 0.5 for i in range(len(possible_num_bars))],
               possible_num_bars)
    plt.xlabel("Tatum-per-bar hypotheses", color="white", fontsize=15)
    plt.ylabel("Number-of-bar hypotheses", color="white", fontsize=15)
    plt.tight_layout()

    apply_style(plt)
    save_figure(
        plt, "possible_tatum_counts.svg" if not bar_large_tatum_counts else "possible_tatum_counts_barred.svg")


make_figure(False)
make_figure(True)
