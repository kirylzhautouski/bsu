from drv import *

p1 = 0.2
p2 = 0.6

n = 1000

def print_results(seq, p, expected_value_function, variance_function, probability_function):
    print("\t Несмещенная оценка мат. ожидания: " + str(count_expected_value(seq)))
    print("\t Истинное мат. ожидание: " + str(expected_value_function(p)))
    print("\t Несмещенная оценка дисперсии: " + str(count_variance(seq)))
    print("\t Истинная дисперсия: " + str(variance_function(p)))
    print("\t xi2: " + str(pirson_xi2(seq, p, probability_function)))
    print()

if __name__ == "__main__":
    bernoulli_seq = bernoulli_distribution_sequence(p1, n)
    geometric_seq_bernoulli = geometric_distribution_from_bernoulli(p2, n)
    geometric_seq = geometric_distribution(p2, n)

    print("Bi(1, {0}):".format(p1))
    print_results(bernoulli_seq, p1, bernoulli_expected_value, bernoulli_variance,\
         bernoulli_probability_function)

    print("G({0}) from Bernoulli:".format(p2))
    print_results(geometric_seq_bernoulli, p2, geometric_expected_value, geometric_variance,\
         geometric_probability_function)

    print("G({0}):".format(p2))
    print_results(geometric_seq, p2, geometric_expected_value, geometric_variance,\
         geometric_probability_function)
