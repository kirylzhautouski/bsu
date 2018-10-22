with open("input.txt", "r") as input_file:
    n, m = map(int, input_file.readline().split(" "))

    adjacency_list = [[] for i in range(0, n)]

    for i in range(0, m):
        u, w = map(int, input_file.readline().split(" "))
        adjacency_list[u - 1].append(w)
        adjacency_list[w - 1].append(u)

with open("output.txt", "w") as output_file:
    for i in range(0, n):
        output_file.write(str(len(adjacency_list[i])) + " ")
        
        for j in range(0, len(adjacency_list[i])):
            output_file.write(str(adjacency_list[i][j]) + " ")
        
        output_file.write("\n")