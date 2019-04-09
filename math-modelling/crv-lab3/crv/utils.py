from math import fabs

def count_expected_value(sample):
    return sum(sample) / len(sample)

def count_variance(sample):
    expected_value = count_expected_value(sample)

    sample_sum = 0
    for x in sample:
        sample_sum += (x - expected_value) ** 2

    return sample_sum / (len(sample) - 1)

def kolmogorov_d(sample, distribution_function, *args):
    sorted_sample = sorted(sample)
    d = 0

    n = len(sample)
    for i in range(n):
        tmp = fabs((i + 1) / n - distribution_function(sorted_sample[i], *args))

        if d < tmp:
            d = tmp

    return d    

def pirson_xi2(sample, number_of_segments, distribution_function, *args):
    xi2 = 0
    n = len(sample)

    x_min = min(sample)
    x_max = max(sample)

    delta = (x_max - x_min) / number_of_segments

    count = [0] * (number_of_segments + 1)
    for i in range(n):
        count[int(((sample[i] - x_min) / (x_max - x_min)) * number_of_segments)] += 1

    for i in range(1, number_of_segments + 1):
        p = distribution_function(x_min + i * delta, *args) - \
            distribution_function(x_min + (i - 1) * delta, *args)
        xi2 += (count[i] - n * p) / (n * p)

    return xi2