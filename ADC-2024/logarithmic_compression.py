import matplotlib.pyplot as plt
from figure_utils import *
import numpy as np

# plot log(1 + 100*sqrt(x)) for x in [0, 10]
x = np.linspace(0, 3, 100)
plt.plot(x, np.log(1 + 100 * np.sqrt(x)))
plt.grid(True)
plt.xlabel('in: normalized power spectrum')
plt.ylabel('out: compressed spectrum')
# remove all spines
plt.gca().spines['right'].set_visible(False)
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['left'].set_visible(False)
plt.gca().spines['bottom'].set_visible(False)

apply_style(plt)
save_figure(plt, "logarithmic_compression.svg")

# plt.show()