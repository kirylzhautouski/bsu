#include "OpenCL.h"
#include "DeviceEx.h"
#include "Matrix.h"
#include "KernelEx.h"

#include <iomanip>
#include <iostream>
#include <chrono>
#include <fstream>

using namespace std;
using namespace cl;

class Main
{
public:
	static void Run(int argc, char** argv)
	{
		if (argc < 5) 
		{
			PrintUsage();
		}
		else if (argc >= 5)
		{
			if (strcmp(argv[1], "-p") == 0)
			{
				if (argc == 7)
					ParallelMultiply(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5], argv[6]);
				else
					ParallelMultiply(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
			}
			else if (strcmp(argv[1], "-np") == 0)
			{
				NonParallelMultiply(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
			}
			else
			{
				PrintUsage();
			}
		}
	}

private:
	static void ParallelMultiply(int n, int m, int l, string kernel_file = 
		"..\\kernels\\matrix-multiplication-task2\\Kernel.cl", string kernel_name = "VectorMultiply")
	{
		try
		{
			auto device = DeviceEx::Find("Intel");
			RunOnDevice(device, n, m, l, kernel_file, kernel_name);
		}
		catch (const exception &e)
		{
			cerr << e.what() << endl;
		}
	}

	static void NonParallelMultiply(int n, int m, int l)
	{
		auto m1 = CreateMatrix(n, m);
		auto m2 = CreateMatrix(m, l);

		ofstream fout("..\\output\\matrix-multiplication-task2\\output.txt");
		ofstream time_history_out("..\\output\\matrix-multiplication-task2\\time_history.txt", ios::app);
		ofstream time_out("..\\output\\matrix-multiplication-task2\\time.txt", ios::app);

		Print(m1, fout);
		Print(m2, fout);

		auto start_time = chrono::high_resolution_clock::now();
		auto m3 = m1 * m2;
		auto finish_time = chrono::high_resolution_clock::now();

		auto duration = finish_time - start_time;

		time_out << "NonParallelMultiply(" << n << ", " << m << ", " << l << "): " << duration.count() / 1000000.0 << " ms" << endl;
		time_history_out << "NonParallelMultiply(" << n << ", " << m << ", " << l << "): " << duration.count() / 1000000.0 << " ms" << endl;
		
		Print(m3, fout);

		time_out.close();
		time_history_out.close();
		fout.close();
	}

	static void RunOnDevice(const Device& device, int n, int m, int l, string kernel_file, string kernel_name)
	{
		auto m1 = CreateMatrix(n, m);
		auto m2 = CreateMatrix(l, m);

		ofstream fout("..\\output\\matrix-multiplication-task2\\output.txt");
		ofstream time_history_out("..\\output\\matrix-multiplication-task2\\time_history.txt", ios::app);
		ofstream time_out("..\\output\\matrix-multiplication-task2\\time.txt", ios::app);

		Print(m1, fout);
		Print(m2, fout);

		m2 = m2.Transpose();
		auto context = DeviceEx::CreateContext(device);
		auto m1_buffer = CreateBuffer(context, m1, CL_MEM_READ_ONLY);
		auto m2_buffer = CreateBuffer(context, m2, CL_MEM_READ_ONLY);

		auto m3 = RunKernel(device, context, m1_buffer, m2_buffer, n, m, l, kernel_file, kernel_name, time_history_out, time_out);

		m1.Dispose();
		m2.Dispose();

		Print(m3, fout);

		m3.Dispose();

		time_out.close();
		time_history_out.close();
		fout.close();
	}

	static Matrix RunKernel(const Device& device, const Context& context, Buffer m1_buffer, 
		Buffer m2_transposed_buffer, int n, int m, int l, string kernel_file, string kernel_name, 
		ostream& time_history_out, ostream& time_out)
	{
		auto m3 = Matrix(n, l, new double[n * l]{});
		auto m3_buffer = CreateBuffer(context, m3, CL_MEM_READ_WRITE);

		auto kernel = KernelEx::BuildFromFile(device, context, kernel_file, kernel_name);
		kernel.setArg(0, m1_buffer);
		kernel.setArg(1, m2_transposed_buffer);
		kernel.setArg(2, m3_buffer);
		kernel.setArg(3, n);
		kernel.setArg(4, m);
		kernel.setArg(5, l);

		auto event = Event();
		auto queue = CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE);
		queue.enqueueNDRangeKernel(kernel, NullRange, NDRange(m3.GetRows(), m3.GetColumns()), NullRange, nullptr, &event);
		event.wait();
		queue.finish();

		auto start_time = event.getProfilingInfo<CL_PROFILING_COMMAND_START>();
		auto end_time = event.getProfilingInfo<CL_PROFILING_COMMAND_END>();
		time_out << "ParallelMultiply(" << kernel_name << ", " << n << ", " << m << ", " << l << "): " << fixed << setprecision(5) << (end_time - start_time) / 1000000.0 << " ms" << endl;
		time_history_out << "ParallelMultiply(" << kernel_name << ", " << n << ", " << m << ", " << l << "): " << fixed << setprecision(5) << (end_time - start_time) / 1000000.0 << " ms" << endl;

		ReadFromBuffer(queue, m3, m3_buffer);
		
		return m3;
	}

	static Matrix CreateMatrix(int n, int m)
	{
		auto size = n * m;
		auto matrix = new double[size];

		for (auto k = 0; k < size; ++k)
		{
			matrix[k] = 0.2 + k % m;
		}

		return Matrix(n, m, matrix);
	}

	static void Print(Matrix matrix, ostream& os)
	{
		for (auto i = 0; i < matrix.GetRows(); ++i)
		{
			for (auto j = 0; j < matrix.GetColumns(); ++j)
			{
				os << fixed << setprecision(2) << matrix.Get(i, j) << " ";
			}

			os << endl;
		}

		os << endl;
	}

	static Buffer CreateBuffer(const Context& context, Matrix matrix, int accessFlag)
	{
		return Buffer(context, accessFlag | CL_MEM_COPY_HOST_PTR, matrix.GetRows() * matrix.GetColumns() 
			* sizeof(double), matrix.Data);
	}

	static void ReadFromBuffer(const CommandQueue& queue, Matrix matrix, const Buffer& buffer)
	{
		queue.enqueueReadBuffer(buffer, CL_TRUE, 0, matrix.GetRows() * matrix.GetColumns() 
			* sizeof(double), matrix.Data);
	}

	static void PrintUsage()
	{
		cout << "usage: <-p/-np> <n> <m> <l> [if -p {[kernel file path] [kernel function]}]\n\tp - parallel\n\tnp - not parallel\n";
	}
};

int main(int argc, char** argv)
{
	// usage: <-p/-np> [kernel file path] [kernel function]
	//        p - parallel
	//		  np - not parallel
	//        if parallel then you can choose a file and a function
	//
	//	matrix-multiplication-task2.exe -np 8 8 8
	//	
	//
	Main::Run(argc, argv);
}