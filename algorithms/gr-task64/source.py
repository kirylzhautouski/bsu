with open("input.txt", "r") as input_file:
    n, m = map(int, input_file.readline().split(" "))
    
    adjacency_matrix = [[0 for i in range(0, n)] for i in range(0, n)]

    for i in range(0, m):
        u, w = map(int, input_file.readline().split(" "))
        adjacency_matrix[u - 1][w - 1], adjacency_matrix[w - 1][u - 1] = 1, 1

with open("output.txt", "w") as output_file:
    for i in range(0, n - 1):
        for j in range(0, n - 1):
            output_file.write(str(adjacency_matrix[i][j]) + " ")

        output_file.write(str(adjacency_matrix[i][n - 1]) + "\n")
    
    for j in range(0, n - 1):
            output_file.write(str(adjacency_matrix[n - 1][j]) + " ")

    output_file.write(str(adjacency_matrix[n - 1][n - 1]))
    
