from random import random
from math import exp, log
from scipy.stats import laplace

from .utils import *

def laplace_distribution(n, a, b):
    result = [0] * n

    ri = random()
    for i in range(n):
        ri1 = random()

        result[i] = a + (1 / b) * log(ri / ri1)

        ri = ri1

    return result

def laplace_distribution_expected_value(a, b):
    return a

def laplace_distribution_variance(a, b):
    return 2 / pow(b, 2)

def laplace_distribution_function(x, a, b):
    # return laplace.cdf(x, a, b)

    if x <= a:
        return 0.5 * exp(b * (x - a))
    else:
        return 1 - 0.5 * exp(-b * (x - a))
