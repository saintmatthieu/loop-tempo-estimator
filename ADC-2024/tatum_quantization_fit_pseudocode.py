odf = [0, 1.3, 0, 0, 3.4, 0, ...]
odf_peak_indices = [1, 4, ...]
pv = [odf[i] for i in odf_peak_indices]  # peak values
tatum_count_hypotheses = [8, 12, 16, ...]
num_peaks = len(odf_peak_indices)


def get_peak_distance(peak_index, tatum_count):
    # range is [0, 1]
    peak_pos = peak_index / len(odf)
    # range is also [0, 1]
    nearest_tatum_pos = round(peak_pos * tatum_count) / tatum_count
    # max is 0.5 / tatum_count
    distance = abs(peak_pos - nearest_tatum_pos)
    # we normalize back to 1
    return 2 * distance * tatum_count

errors = []
for tatum_count in tatum_count_hypotheses:
    pd = [get_peak_distance(i, tatum_count) for i in odf_peak_indices]
    weighted_average = sum([pd[i] * pv[i] for i in range(num_peaks)]) / sum(pv)
    errors.append(weighted_average)

# Tatum count with least error
best_tatum_count = tatum_count_hypotheses[errors.index(min(errors))]
