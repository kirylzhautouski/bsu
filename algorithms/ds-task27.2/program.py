def dfs(start, adjacency_matrix, visited, labels_when_left, time):
    visited[start] = True
    time += 1

    for i in range(len(visited)):
        if not visited[i] and adjacency_matrix[start][i] == 1:
            time = dfs(i, adjacency_matrix, visited, labels_when_left, time)

    labels_when_left[start] = time
    time += 1
    return time

def dfs_find_scc(start, adjacency_matrix, visited, numbers_of_component, number_of_component):
    visited[start] = True
    numbers_of_component[start] = number_of_component

    for i in range(len(visited)):
        if not visited[i] and adjacency_matrix[i][start] == 1:
            dfs_find_scc(i, adjacency_matrix, visited, numbers_of_component, number_of_component)

def find_antibase(start, number_of_incomings, number_of_outcomings, scc_connections, number_of_components, labels):
    antibase = 0

    if labels[start] == False:
        if number_of_outcomings[start] == 0 and number_of_incomings[start] != 0: # if start is antibase
            antibase = start
            labels[start] = True
            return antibase

        labels[start] = True
        for i in range(number_of_components):
            if scc_connections[start][i]:
                antibase_i = find_antibase(i, number_of_incomings, number_of_outcomings, scc_connections, number_of_components, labels)

                antibase = antibase_i if antibase == 0 else antibase

    return antibase

def connect(sccs, processed_bases, processed_antibases, isolated, number_of_pairs, number_of_bases, number_of_antibases, number_of_isolated):
    new_arcs = list()

    if number_of_bases <= number_of_antibases and number_of_bases != 0:
        for i in range(1, number_of_pairs):
            new_arcs.append((sccs[processed_antibases[i - 1]][0], sccs[processed_bases[i]][0]))

        for i in range(number_of_pairs + 1, number_of_bases + 1):
            new_arcs.append((sccs[processed_antibases[i - 1]][0], sccs[processed_bases[i - 1]][0]))

        for i in range(number_of_bases + 1, number_of_antibases):
            new_arcs.append((sccs[processed_antibases[i - 1]][0], sccs[processed_antibases[i]][0]))

        if number_of_isolated == 0 and number_of_bases < number_of_antibases:
            new_arcs.append((sccs[processed_antibases[number_of_antibases - 1]][0], sccs[processed_bases[0]][0]))
        
        if number_of_isolated == 0 and number_of_bases == number_of_antibases:
            new_arcs.append((sccs[processed_antibases[number_of_pairs - 1]][0], sccs[processed_bases[0]][0]))

        if number_of_bases < number_of_antibases:
            new_arcs.append((sccs[processed_antibases[number_of_pairs - 1]][0], sccs[processed_antibases[number_of_bases]][0]))

    if number_of_bases > number_of_antibases and number_of_antibases != 0:
        for i in range(1, number_of_pairs):
            new_arcs.append((sccs[processed_antibases[i - 1]][0], sccs[processed_bases[i]][0]))

        for i in range(number_of_pairs + 1, number_of_antibases + 1):
            new_arcs.append((sccs[processed_antibases[i - 1]][0], sccs[processed_bases[i - 1]][0]))
        
        for i in range(number_of_antibases + 1, number_of_bases):
            new_arcs.append((sccs[processed_bases[i - 1]][0], sccs[processed_bases[i]][0]))

        if number_of_isolated == 0:
            new_arcs.append((sccs[processed_antibases[number_of_antibases - 1]][0], sccs[processed_bases[0]][0]))

        if number_of_isolated == 0 and number_of_bases == number_of_antibases:
            new_arcs.append((sccs[processed_antibases[number_of_pairs - 1]][0], sccs[processed_bases[0]][0]))

        new_arcs.append((sccs[processed_bases[number_of_pairs - 1]][0], sccs[processed_bases[number_of_antibases]][0]))

    if number_of_isolated != 0 and number_of_antibases != 0:
        for i in range(1, number_of_isolated):
            new_arcs.append((sccs[isolated[i - 1]][0], sccs[isolated[i]][0]))

        if number_of_bases == number_of_antibases:
            new_arcs.append((sccs[processed_antibases[number_of_pairs - 1]][0], sccs[isolated[0]][0]))
        else:
            new_arcs.append((sccs[processed_antibases[number_of_antibases - 1]][0], sccs[isolated[0]][0]))

        new_arcs.append((sccs[isolated[number_of_isolated - 1]][0], sccs[processed_bases[0]][0]))

    if number_of_isolated != 0 and number_of_antibases == 0:
        for i in range(1, number_of_isolated):
            new_arcs.append((sccs[isolated[i - 1]][0], sccs[isolated[i]][0]))

        new_arcs.append((sccs[isolated[number_of_isolated - 1]][0], sccs[isolated[0]][0]))

    return new_arcs


if __name__ == "__main__":
    with open("input.txt", "r") as input_file:
        number_of_schools = int(input_file.readline())

        adjacency_matrix = [[0 for j in range(number_of_schools)] for i in range(number_of_schools)]
        for i in range(number_of_schools):
            receivers = list(map(int, input_file.readline().split(" ")))

            for receiver in receivers[:len(receivers) - 1]:
                adjacency_matrix[i][receiver - 1] = 1

    # searching for strongly connected components

    visited = [False] * number_of_schools
    labels_when_left = [0] * number_of_schools
    
    time = 0
    for i in range(len(visited)):
        if not visited[i]:
            time = dfs(i, adjacency_matrix, visited, labels_when_left, time)

    numbers_of_component = [0] * number_of_schools
    visited = [False] * number_of_schools

    sorted_labels_when_left = sorted(labels_when_left, reverse=True)

    number_of_components = 0
    for i in sorted_labels_when_left:
        if not visited[labels_when_left.index(i)]:
            dfs_find_scc(labels_when_left.index(i), adjacency_matrix, visited, numbers_of_component, number_of_components)
            
            number_of_components += 1

    scc_connections = [[0 for i in range(number_of_components)] for j in range(number_of_components)]
    for i in range(len(adjacency_matrix)):
        for j in range(len(adjacency_matrix)):
            if numbers_of_component[i] != numbers_of_component[j] and adjacency_matrix[i][j] != 0:
                scc_connections[numbers_of_component[i]][numbers_of_component[j]] = 1

    number_of_incomings = [0] * number_of_components
    number_of_outcomings = [0] * number_of_components
    answer_a = 0 # number of components, that do not have incoming arcs from other components - answer to A
    for i in range(number_of_components):
        for j in range(number_of_components):
            if scc_connections[j][i] == 1:
                number_of_incomings[i] += 1    

            if scc_connections[i][j] == 1:
                number_of_outcomings[i] += 1

        if number_of_incomings[i] == 0:
            answer_a += 1    

    if number_of_components == 1:      
        with open("output.txt", "w") as output_file:
            output_file.write(str(answer_a) + '\n')
            output_file.write("0\n")
    else:
        sccs = [[] for i in range(number_of_components)]
        
        for vertice, component in enumerate(numbers_of_component):
            sccs[component].append(vertice)

        scc_types = [0] * number_of_components
        bases = list()
        isolated = list()

        for i in range(number_of_components):
            if number_of_outcomings[i] != 0 and number_of_incomings[i] == 0:
                bases.append(i)
                scc_types[i] = 1 # base
            elif number_of_incomings[i] != 0 and number_of_outcomings[i] == 0:
                scc_types[i] = 2 # antibase
            elif number_of_incomings[i] == 0 and number_of_outcomings[i] == 0:
                isolated.append(i)
                scc_types[i] = 3 # isolated

        labels = [0] * number_of_components

        number_of_pairs = 0
        processed_bases = list()
        processed_antibases = list()

        while len(bases) != 0:
            base = bases.pop()

            if labels[base] == 0:
                antibase = find_antibase(base, number_of_incomings, number_of_outcomings, scc_connections, number_of_components, labels)

                if antibase != 0:
                    processed_bases.append(base)
                    processed_antibases.append(antibase)

                    number_of_pairs += 1

        for i in range(number_of_components):
            if scc_types[i] == 1 and not i in processed_bases:
                processed_bases.append(i)

            if scc_types[i] == 2 and not i in processed_antibases:
                processed_antibases.append(i)

        new_arcs = connect(sccs, processed_bases, processed_antibases, isolated, number_of_pairs, len(processed_bases), len(processed_antibases), len(isolated))
        
        with open("output.txt", "w") as output_file:
            output_file.write(str(answer_a) + '\n')
            output_file.write(str(len(new_arcs)) + '\n')
        
            for arc in new_arcs:
                output_file.write(str(arc[0] + 1) + " " + str(arc[1] + 1) + "\n")
        