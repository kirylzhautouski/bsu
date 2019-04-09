from random import random
from math import log, exp
from scipy.stats import logistic

from .utils import *

def logistic_distribution(n, a, b):
    result = [0] * n

    for i in range(n):
        r = random()

        result[i] = a - b * log((1 - r) / r)

    return result

def logistic_distribution_expected_value(a, b):
    return a

def logistic_distribution_variance(a, b):
    return 3.2899 * pow(b, 2)

def logistic_distribution_function(x, a, b):
    # return logistic.cdf(x, a, b)

    return 1 / (1 + exp(-(x - a) / b))
