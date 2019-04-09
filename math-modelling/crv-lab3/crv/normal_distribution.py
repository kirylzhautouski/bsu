from random import random
from math import sqrt
from scipy.stats import norm

from .utils import *

def normal_distribution(n, m, s2, k):
    result = [0] * n

    for i in range(n):
        r = [random() for j in range(k)]

        result[i] = m + sqrt(s2) * sqrt(12 / k) * (sum(r) - k / 2)

    return result

def normal_distribution_expected_value(m, s2):
    return m

def normal_distribution_variance(m, s2):
    return s2

def normal_distribution_function(x, m, s2):
    return norm.cdf(x, m, s2)
