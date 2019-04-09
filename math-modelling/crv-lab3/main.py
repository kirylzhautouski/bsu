from crv import *

from math import sqrt

n = 1000

# normal dist
m = 0
s2 = 1

k = 12

# logistic dist
a = 2 # also for laplace dist
b = 3

def print_results(distribution_name, expected_value, real_expected_value, variance, real_variance, \
        sqrtn_dn, xi2):
    print(distribution_name)
    print("\t Несмещенная оценка мат. ожидания: " + str(expected_value))
    print("\t Истинное мат. ожидание: " + str(real_expected_value))
    print("\t Несмещенная оценка дисперсии: " + str(variance))
    print("\t Истинная дисперсия: " + str(real_variance))
    print("\t sqrt(n)Dn: " + str(sqrtn_dn))
    print("\t xi2: " + str(xi2))
    print()

if __name__ == "__main__":
    normal_distribution_seq = normal_distribution(n, m, s2, k)

    print_results("N(0, 1):", count_expected_value(normal_distribution_seq), \
         normal_distribution_expected_value(m, s2), count_variance(normal_distribution_seq), \
         normal_distribution_variance(m, s2), sqrt(n) * kolmogorov_d(normal_distribution_seq, \
         normal_distribution_function, m, s2), \
         pirson_xi2(normal_distribution_seq, 10, normal_distribution_function, m, s2))

    logistic_distribution_seq = logistic_distribution(n, a, b)
    print_results("LG(2, 3):", count_expected_value(logistic_distribution_seq), \
         logistic_distribution_expected_value(a, b), count_variance(logistic_distribution_seq), \
         logistic_distribution_variance(a, b), sqrt(n) * kolmogorov_d(logistic_distribution_seq, \
         logistic_distribution_function, a, b), \
         pirson_xi2(logistic_distribution_seq, 10, logistic_distribution_function, a, b))  

    laplace_distribution_seq = laplace_distribution(n, a, b)
    print_results("L(2, 3):", count_expected_value(laplace_distribution_seq), \
         laplace_distribution_expected_value(a, b), count_variance(laplace_distribution_seq), \
         laplace_distribution_variance(a, b), sqrt(n) * kolmogorov_d(laplace_distribution_seq, \
         laplace_distribution_function, a, b), \
         pirson_xi2(laplace_distribution_seq, 10, laplace_distribution_function, a, b))    