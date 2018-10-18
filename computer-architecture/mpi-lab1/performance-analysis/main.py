#performance analysis for task 1.2 and task 2.1
import subprocess
import sys
import os
import datetime
import time
from matplotlib import pyplot as plt

from generators.linear_system import generate_system
from generators.vectors import generate_vectors

def visualize(name, dimension, time_dict, plot_path):
    plt.plot(time_dict.keys(), time_dict.values(), color="green", marker="o", linestyle="solid")
    plt.title(name + "(dimension: " + str(dimension) + ")")
    plt.ylabel("Time, sec")
    plt.xlabel("Processes")

    if not os.path.exists(plot_path + "d" + str(dimension)):
        os.mkdir(plot_path + "d" + str(dimension))

    plt.savefig(plot_path + "d" + str(dimension) + "/" + str(datetime.datetime.today().strftime("%Y-%m-%d-%H.%M.%S")) + ".png")
    plt.show()

def run_command(process_count, program_path):
    command = "mpiexec -n " + str(process_count) + " " + program_path
    subprocess.call(command)

def run_test(dimension, program_path, input_file_path, output_file_path, plot_path, data_generator, name):
    data_generator(dimension, input_file_path)
    time_dict = {}
    
    for deg in range(0, 6):
        start_time = time.time()
        print(deg)
        run_command(2 ** deg, program_path)
        time_dict[2 ** deg] = time.time() - start_time

    message = "[" + str(dimension) + ", " + str(time_dict) + "]"
    print(message)
    with open(output_file_path, "a") as output_file:
        output_file.write(message)
        output_file.write("\n\n")

    visualize(name, dimension, time_dict, plot_path)

if __name__ == "__main__":
    if len(sys.argv) > 2:
        test_type = sys.argv[1]

        if test_type == "multiply":
            multiply_dimension = int(sys.argv[2])

            print("<---- Scalar multiply test (dimension:", str(multiply_dimension) + ") ---->")
            run_test(multiply_dimension, "../x64/Release/1.2_Scalar_Multiply_Modified.exe", "input/task1.2_input.txt",  "test_output/scalar_multiply_time.txt", "test_output/scalar_multiply_plots/", generate_vectors, "Scalar multiply")
        elif test_type == "discrepancy":
            system_dimension = int(sys.argv[2])

            print("<---- Discrepancy test (dimension:", str(system_dimension) + ") ---->")
            run_test(system_dimension, "../x64/Release/2.1_Discrepancy.exe", "input/task2_input.txt", "test_output/discrepancy_time.txt", "test_output/discrepancy_plots/", generate_system, "Discrepency")
        else:
            print("Invalid arguments")
    else:
        print("Invalid arguments")
    
    

    
    
