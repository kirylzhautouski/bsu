from random import random
from math import cos, sin

def f(x):
    return cos(x + sin(x))

def density_function(x, a, b):
    return 1 / (b - a) if a < x < b else 0

def integrate(f, a, b, n):
    x_seq = [a + (b - a) * random() for _ in range(n)]

    result_sum = 0
    for x in x_seq:
        result_sum += f(x)

    return ((b - a) / n) * result_sum 
