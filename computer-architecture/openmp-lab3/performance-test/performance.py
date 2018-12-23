import datetime
import subprocess
import os
from matplotlib import pyplot as plt

def run(exe_path: str, number_of_threads: int, n: int, a: int, b: int):
    subprocess.call(exe_path + " " + str(number_of_threads) + " " + str(a) + " " + str(b) + " " + str(n))

def visualize(a: int, b: int, n: int, numbers_of_threads: list, time_results: list, plot_path: str):
    plt.plot(numbers_of_threads, time_results, color="green", marker="o", linestyle="solid")
    plt.title("n=" + str(n) + ", a=" + str(a) + ", b=" + str(b))
    plt.ylabel("Time, ms")

    if not os.path.exists(plot_path + "n" + str(n) + "-"  + str(a) + "-" + str(b)):
        os.mkdir(plot_path + "n" + str(n) + "-"  + str(a) + "-" + str(b))

    plt.savefig(plot_path + "n" + str(n) + "-"  + str(a) + "-" + str(b) + "\\" + str(datetime.datetime.today().strftime("%Y-%m-%d-%H.%M.%S")) + ".png")
    plt.show()

def test_function(function_name: str, a: int, b: int, n: int):
    numbers_of_threads = [1, 2, 3, 4, 8, 16, 32, 64, 128]

    with open("..\\time-results\\time_results.txt", "r+") as time_results_data:
        time_results = list()
        
        time_results_data.truncate(0)
        for number_of_threads in numbers_of_threads:
            run("..\\cmake-build-release\\" + function_name + ".exe", number_of_threads, n, a, b)
        
        for number_of_threads in numbers_of_threads:
            time_results.append(float(time_results_data.readline().split(" ")[-2]))

        visualize(a, b, n, numbers_of_threads, time_results, "performance-results\\" + function_name + "\\")

if __name__ == "__main__":
    test_function("find_max_min_task1", 1, 21, 20000000)