import sys

with open("output.txt", "w") as output_file:
    
    with open("input.txt", "r") as input_file:
        n = int(input_file.readline())
        binary_heap = list(map(int, input_file.read().split(" ")))

        for i in range(0, n):
            if 2 * i + 2 < n:
                if min(binary_heap[i], binary_heap[2 * i + 1], binary_heap[2 * i + 2]) != binary_heap[i]:
                    output_file.write("No")
                    sys.exit(0)
            elif 2 * i + 1 < n:
                if min(binary_heap[i], binary_heap[2 * i + 1]) != binary_heap[i]:
                    output_file.write("No")
                    sys.exit(0)

    output_file.write("Yes") 

   

   

