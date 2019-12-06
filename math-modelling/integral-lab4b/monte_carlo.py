from random import random
from random import uniform
from math import sqrt

import scipy.integrate as sp_integrate

eps = 0.007

def f(x, y):
    return 1 / (x ** 2 + y ** 4)

def get_y_ranges(square_radius):
    def y_ranges(x):
        y = (square_radius - x ** 2) ** 0.5
        return [-y, y]

    return y_ranges

def scipy_integrate(f, first_square_radius, second_square_radius):
    first_integral, _ = sp_integrate.nquad(f, [get_y_ranges(second_square_radius), \
        [-sqrt(second_square_radius), -eps]])
    second_integral, _ = sp_integrate.nquad(f, [get_y_ranges(second_square_radius), \
        [eps, sqrt(second_square_radius)]])
    third_integral, _ = sp_integrate.nquad(f, [get_y_ranges(first_square_radius), \
        [-sqrt(first_square_radius), -eps]])
    fourth_integral, _ = sp_integrate.nquad(f, [get_y_ranges(first_square_radius), \
        [eps, sqrt(first_square_radius)]])

    return first_integral + second_integral - third_integral - fourth_integral

def belongs_to(first_square_radius, second_square_radius, x, y):
    return first_square_radius <= x ** 2 + y ** 2 < second_square_radius

def integrate(f, first_square_radius, second_square_radius, number_of_iterations, belongs_to):
    a = sqrt(second_square_radius)

    count = 0
    result_sum = 0
    for _ in range(number_of_iterations):
        x, y = uniform(-a, a), uniform(-a, a)

        if belongs_to(first_square_radius, second_square_radius, x, y):
            result_sum += f(x, y)
            count += 1

    return ((2 * a) ** 2 / number_of_iterations) * result_sum
