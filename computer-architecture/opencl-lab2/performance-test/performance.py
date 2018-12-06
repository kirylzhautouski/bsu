import subprocess
import datetime
import os
from matplotlib import pyplot as plt

from kernelgenerator.generate_kernel import generate_kernel

def visualize(n: int, m: int, l: int, testing_functions: list, time_results: list, plot_path):
    plt.plot(testing_functions, time_results, color="green", marker="o", linestyle="solid")
    plt.title("n=" + str(n) + ", m=" + str(m) + ", l=" + str(l))
    plt.ylabel("Time, ms")

    if not os.path.exists(plot_path + "d" + str(n) + "-"  + str(m) + "-" + str(l)):
        os.mkdir(plot_path + "d" + str(n) + "-"  + str(m) + "-" + str(l))

    plt.savefig(plot_path + "d" + str(n) + "-"  + str(m) + "-" + str(l) + "\\" + str(datetime.datetime.today().strftime("%Y-%m-%d-%H.%M.%S")) + ".png")
    plt.show()

def test_scalar_vector_nonparallel(n: int, m: int, l: int):
    subprocess.call("..\Intel Release\matrix-multiplication-task2.exe -np " + str(n) + " " + str(m) + " " + str(l))
    subprocess.call("..\Intel Release\matrix-multiplication-task2.exe -p " + str(n) + " " + str(m) + " " + str(l) + " ..\kernels\matrix-multiplication-task2\Kernel.cl ScalarMultiply")
    subprocess.call("..\Intel Release\matrix-multiplication-task2.exe -p " + str(n) + " " + str(m) + " " + str(l) + " ..\kernels\matrix-multiplication-task2\Kernel.cl VectorMultiply")

    with open("..\output\matrix-multiplication-task2\\time.txt", "r+") as time_info:
        lines = time_info.readlines()

        time_results = list()
        for line in lines:
            print(line)

            time_results.append(line.split(" ")[-2])

        visualize(n, m, l, ["non parallel", "scalar", "vector"], list(map(float, time_results)), "performance-results\\np-scalar-vector\\")
        time_info.truncate(0)
        

def test_different_vector_sizes(n: int, m: int, l: int):
    vector_sizes = [2, 3, 4, 8, 16]

    for vector_size in vector_sizes:
        generate_kernel(vector_size, "..\kernels\matrix-multiplication-task2\GeneratedKernel.cl")
        subprocess.call("..\Intel Release\matrix-multiplication-task2.exe -p " + str(n) + " " + str(m) + " " + str(l) + " ..\kernels\matrix-multiplication-task2\GeneratedKernel.cl VectorMultiply" + str(vector_size))

    with open("..\output\matrix-multiplication-task2\\time.txt", "r+") as time_info:
        lines = time_info.readlines()

        time_results = list()
        for line in lines:
            print(line)

            time_results.append(line.split(" ")[-2])

        visualize(n, m, l, vector_sizes, list(map(float, time_results)), "performance-results\\diff-vector-sizes\\")
        time_info.truncate(0)    
    

if __name__ == "__main__":
    test_scalar_vector_nonparallel(128, 128, 128)
    #test_different_vector_sizes(128, 128, 128)