#generates data for tasks 2.1, 2.2
import random
import sys

def generate_system(l, path):   
    with open(path, "w") as f:
        f.write(str(l) + "\n")

        for i in range(1, l + 1):
            for j in range(1, l + 2):
                f.write(str(int(-50 + (random.random() * 35000) % 100)) + " ")
            f.write("\n")

        for i in range(1, l + 1):
            f.write(str(int(-50 + (random.random() * 35000) % 100)) + " ")

if __name__ == "__main__":
    if (len(sys.argv) > 1):
        l = int(sys.argv[1])
    else:
        l = 16

    path = "..\\input\\task2_input.txt"
    generate_system(l, path)

        
            