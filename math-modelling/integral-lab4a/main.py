import matplotlib.pyplot as plt
import scipy.integrate
import math

from datetime import datetime
from monte_carlo import *

def draw_integral_result_plot(n, scipy_result, monte_carlo_results):
    plt.plot((n[0], n[len(n) - 1]), (scipy_result, scipy_result), n, monte_carlo_results)

    plt.title("integral result")

    plt.xlabel("number of iterations")
    plt.ylabel("intergal result")

    plot_name = "integral result\integral_result_plot(" + datetime.now().strftime("%m-%d-%H-%M-%S") + ")"
    plt.savefig(plot_name)

    plt.show()


def draw_accuracy_plot(n, differences):
    plt.plot(n, differences)

    plt.title("accuracy")

    plt.xlabel("number of iterations")
    plt.ylabel("difference")

    plot_name = "accuracy\\accuracy_plot(" + datetime.now().strftime("%m-%d-%H-%M-%S") + ")"
    plt.savefig(plot_name)

    plt.show()

if __name__ == "__main__":
    scipy_result, _ = scipy.integrate.quad(f, 0, (5 * math.pi) / 7)
    print("scipy:")
    print(scipy_result)

    number_of_iterations = (10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000)

    monte_carlo_results = [integrate(f, 0, (5 * math.pi) / 7, n) for n in number_of_iterations]

    print()
    print("monte-carlo:")
    print(monte_carlo_results[len(number_of_iterations) - 1])

    draw_integral_result_plot(number_of_iterations, scipy_result, monte_carlo_results)
    draw_accuracy_plot(number_of_iterations, [abs(monte_carlo_result - scipy_result) \
        for monte_carlo_result in monte_carlo_results])
