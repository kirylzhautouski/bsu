with open("input.txt", 'r') as input_file:
    x = str(bin(int(input_file.readline())))[2:]
    
    x = x[::-1] 
    with open("output.txt", 'w') as output_file:
        for i in range(0, len(x)):
            if x[i] == '1':
                output_file.write(str(i) + "\n")
