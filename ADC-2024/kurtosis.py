import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as stats
from figure_utils import *

# We are drawing 3 different PDFs of variance 1 to illustrate the difference in kurtosis.
x = np.linspace(-6, 6, 1000)

y = stats.laplace.pdf(x, 0, 1/np.sqrt(2))
plt.plot(x, y, linewidth=2, label="Laplace, k = 6")
y = stats.norm.pdf(x, 0, 1)
plt.plot(x, y, linewidth=2, label="Normal, k = 3")
y = stats.uniform.pdf(x, -np.sqrt(3), 2*np.sqrt(3))
plt.plot(x, y, linewidth=2, label="Uniform, k = 1.8")

ylim = plt.gca().get_ylim()
# plt.fill_between(x, 0, ylim[1], where=(x >= -1) & (x <= 1), alpha=0.1, zorder=0, color="black", label="variance")
# add a dashed vertical line at 0
plt.axvline(0, linestyle="--", color="black", zorder=0, alpha=0.5)

plt.ylim(0, ylim[1])
plt.xlim(-6, 6)
plt.xticks([])
plt.yticks([])
plt.legend()
plt.title("PDFs of equal variance", fontsize=30)

apply_style(plt)
save_figure(plt, "kurtosis.svg")
