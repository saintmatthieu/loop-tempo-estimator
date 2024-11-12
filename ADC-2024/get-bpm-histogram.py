# A script that looks at the title of all the files found in the directory passed as an argument, and parses them for BPM information.

import matplotlib.pyplot as plt
import sys
import os
import re
from figure_utils import *

def get_bpm(filename):
    # Use the equivalent of the C++ regex `R"((?:.*(?:_|-|\s|\.|/|\\))?(\d+)(?:_|-|\s|\.)?bpm(?:(?:_|-|\s|\.).*)?)"`
    # to match the BPM in the filename.
    regex = re.compile(
        r"(?:.*(?:_|-|\s|\.|/|\\))?(\d+)(?:_|-|\s|\.)?bpm(?:(?:_|-|\s|\.).*)?",
        re.IGNORECASE
    )
    match = regex.match(filename)
    if match:
        bpm = int(match.group(1))
        return bpm
    else:
        return None

directory = os.path.join(os.path.dirname(__file__), "..", "tests",
                         "benchmarking", "dataset", "loops")

bpms = []

for filename in os.listdir(directory):
    bpm = get_bpm(filename)
    if bpm:
        bpms.append(bpm)
    else:
        assert False

# The bars should be slightly transparent
plt.hist(bpms, bins=range(55, 210, 10),
         color="white", edgecolor="grey", alpha=0.7)
plt.title("BPM histogram")
# Label font size should be rather big
plt.xlabel("BPM", color="white", fontsize=15)
plt.ylabel("Count", color="white", fontsize=15)

apply_style(plt)
save_figure(plt, "bpm_histogram.svg")

