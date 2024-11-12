# Read features_30_sec.csv (same directory as this script) and plot a histogram of the BPM values.

import matplotlib.pyplot as plt
import pandas as pd
import os
from figure_utils import *
import numpy as np

# open the file
df = pd.read_csv(os.path.join(
    os.path.dirname(__file__), "features_30_sec.csv"))

# get the BPM values
bpms = df["tempo"].dropna()

# The bars should be slightly transparent and normalized
plt.hist(bpms, bins=range(55, 215, 10),
         color="white", edgecolor="grey", alpha=0.5, density=True)
plt.title("BPM histogram")
# ticks should be every 30
plt.xticks(range(60, 210, 30))

# fit a normal distribution in the data
mean = bpms.mean()
std = bpms.std()
xmin, xmax = plt.xlim()
x = np.linspace(0, 300, 100)
p = np.exp(-0.5 * ((x - mean) / std) ** 2) / (std * np.sqrt(2 * np.pi))
plt.plot(x, p, 'w', linewidth=2, alpha=0.7)
# restore the x limits
plt.xlim(xmin, xmax)

# hide the y-axis, it's not important
plt.gca().axes.get_yaxis().set_visible(False)

# Add some text box in the top right corner with the mean
plt.text(0.95, 0.95, f"Mean: {mean:.1f}", color='white', ha='right', va='top', transform=plt.gca().transAxes)
plt.title("BPM histogram (GTZAN dataset)", color='white')


apply_style(plt)

# plt.show()
save_figure(plt, "gtzan_bpm_histogram.svg")
