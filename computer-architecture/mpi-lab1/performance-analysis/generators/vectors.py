#generates data for task 1.2
import sys
import random

def generate_vectors(l, path):
    with open(path, "w") as f:
        f.write(str(l) + "\n")

        for j in range(0, 2):
            for i in range(0, l):
                f.write(str(int(-50 + (random.random() * 35000) % 100)) + " ")
            
            if j == 0:
                f.write("\n")


if __name__ == "__main__":
    if (len(sys.argv) > 1):
        l = int(sys.argv[1])
    else:
        l = 16

    path = "..\\input\\task1.2_input.txt", "w"
    generate_vectors(l)