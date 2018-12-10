import sys

def dfs(start: int, adjacency_matrix: list, visited: map):
    visited[start] = True
    
    for i in range(0, 7):
        if adjacency_matrix[start][i] == 1 and not visited[i]:
            dfs(i, adjacency_matrix, visited)

with open("input.txt", "r") as input_file:
    n = int(input_file.readline())

    degrees = [0] * 7
    adjacency_matrix = [[0 for i in range(7)] for j in range(7)]

    for i in range(0, n):
        top, bottom = map(int, input_file.readline().split(" "))

        degrees[top] += 1
        degrees[bottom] += 1
        adjacency_matrix[top][bottom] = 1
        adjacency_matrix[bottom][top] = 1

    start = 0
    for i in range(7):
        if degrees[i] != 0:
            start = i
            break

    all_degrees_are_even = True
    for degree in degrees:
        if degree % 2 == 1:
            all_degrees_are_even = False
            break

    with open("output.txt", "w") as output_file: 
        if all_degrees_are_even:
            visited = [False] * 7
            dfs(start, adjacency_matrix, visited)

            for i in range(7):
                if degrees[i] != 0:
                    if not visited[i]:
                        output_file.write("No")
                        sys.exit(0)

            output_file.write("Yes")
        else:
            output_file.write("No")
    