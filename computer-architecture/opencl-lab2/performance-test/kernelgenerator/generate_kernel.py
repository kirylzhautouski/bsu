import sys

def generate_kernel(n: int, kernel_file_path: str):
    zero_vector = str()
    v1 = str()
    v2 = str()
    sum = str()

    vector_components = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"]
    for i in range(n - 1):
        zero_vector += "0, "
        v1 += "m1[m1Base + " + (str(i) + " + " if i != 0 else "") + str(n) + " * k], "
        v2 += "m2[m2Base + " + (str(i) + " + " if i != 0 else "") + str(n) + " * k], "
        sum += "sum" + str(n) + ".s" + vector_components[i] + " + "

    zero_vector += "0"
    v1 += "m1[m1Base + " + str(n - 1) + " + " + str(n) + " * k]"
    v2 += "m2[m2Base + " + str(n - 1) + " + " + str(n) + " * k]"
    sum += "sum" + str(n) + ".s" + vector_components[n - 1]

    kernel = """__kernel void VectorMultiply{0}(__global double* m1, __global double* m2, __global double* m3, int n, int m, int l)
{{
    int i = get_global_id(0);
	int j = get_global_id(1);

	double{0} sum{0} = (double{0})({1});
	int m1Base = i * m;
	int m2Base = j * m;
	int bound{0} = m / {0};

	for (int k = 0; k < bound{0}; ++k)
    {{
	    double{0} v1 = (double{0})({2});
	    double{0} v2 = (double{0})({3});
        sum{0} += v1 * v2;
    }}
    
    double sum = 0;
	m1Base += bound{0} * {0};
	m2Base += bound{0} * {0};
	int bound = m - bound{0} * {0};

	for (int k = 0; k < bound; ++k)
    {{
        sum += m1[m1Base + k] * m2[m2Base + k];
    }}

	m3[i * l + j] = {4} + sum;
}}""".format(n, zero_vector, v1, v2, sum)
 
    with open(kernel_file_path, "w") as kernel_file:
        kernel_file.write(kernel)


if __name__ == "__main__":
    # python generate_kernel.py 16 ..\..\kernels\matrix-multiplication-task2\GeneratedKernel.cl
    if len(sys.argv) != 3:
        print("usage: <n> <kernel_file_path>, n = 2, 3, 4, 8, 16")
    else:
        if int(sys.argv[1]) in [2, 3, 4, 8, 16]:
            generate_kernel(int(sys.argv[1]), sys.argv[2])
        else:
            print("n != 2, 3, 4, 8, 16")    