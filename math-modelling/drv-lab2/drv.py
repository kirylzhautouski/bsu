from random import random
from math import log

def bernoulli_distribution(p):
    return 1 if random() < p else 0

def bernoulli_distribution_sequence(p, n):
    return [bernoulli_distribution(p) for _ in range(n)]

def geometric_distribution(p, n):
    return [int(log(random()) / log(1 - p)) for _ in range(n)]

def geometric_distribution_from_bernoulli(p, n):
    result_seq = [0] * n
    
    counter = 0
    for i in range(n):
        while bernoulli_distribution(p) != 1:
            counter += 1

        result_seq[i] = counter
        counter = 0

    return result_seq

def count_expected_value(sample):
    return sum(sample) / len(sample)

def count_variance(sample):
    expected_value = count_expected_value(sample)

    sample_sum = 0
    for x in sample:
        sample_sum += (x - expected_value) ** 2

    return sample_sum / (len(sample) - 1)

def bernoulli_probability_function(x, p):
    return (p ** x) * ((1  - p) ** (1 - x))

def bernoulli_expected_value(p):
    return p

def bernoulli_variance(p):
    return p * (1 - p)

def geometric_probability_function(x, p):
    return p * ((1 - p) ** x)

def geometric_expected_value(p):
    return (1 - p) / p

def geometric_variance(p):
    return (1 - p) / (p ** 2)

def pirson_xi2(sample, p, probability_function):
    k = max(sample) + 1
    counts = [0]  * k

    xi2 = 0

    for x in sample:
        counts[x] += 1

    for i in range(k):
        xi2 += ((counts[i] - len(sample) * probability_function(i, p)) ** 2) / (len(sample) * \
        probability_function(i, p))

    return xi2