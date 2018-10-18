#include <fstream>
#include <iostream>
#include <ctime>

#include <mpi.h>
#include "Process.h"

class Program
{
public:
	static void Main()
	{
		auto process = MPI::Process();

		std::ifstream fin;
		std::ofstream fout;
		
		int *first_vector = nullptr, *second_vector = nullptr, l = 16;

		if (process.IsMaster())
		{
			fin.open("input/task1.1_input.txt");
			fout.open("output/task1.1_output.txt");

			fin >> l;

			first_vector = new int[l];
			second_vector = new int[l];

			ReadVector(fin, first_vector, l);
			ReadVector(fin, second_vector, l);
		}

		auto slice_size = l / process.ProcessCount();

		auto first_buffer = new int[slice_size];
		auto second_buffer = new int[slice_size];
		Scatter(first_vector, slice_size, first_buffer);
		Scatter(second_vector, slice_size, second_buffer);

		long slice_sum = 0;
		for (int i = 0; i < slice_size; i++)
			slice_sum += second_buffer[i] * first_buffer[i];

		long result_sum = 0;
		MPI_Reduce(
			&slice_sum,
			&result_sum,
			1,
			MPI_LONG,
			MPI_SUM,
			MPI::MasterRank,
			MPI_COMM_WORLD
		);

		if (process.IsMaster())
		{
			fout << result_sum;

			delete[] first_vector;
			delete[] second_vector;

			fin.close();
			fout.close();
		}

		delete[] first_buffer;
		delete[] second_buffer;
	}

private:
	static void ReadVector(std::istream& stream, int* vector, int l)
	{
		for (int i = 0; i < l; i++)
			stream >> vector[i];
	}

	static void Scatter(int* source, int slice_size, int* buffer)
	{
		MPI_Scatter(
			source,
			slice_size,
			MPI_INT,
			buffer,
			slice_size,
			MPI_INT,
			MPI::MasterRank,
			MPI_COMM_WORLD
		);
	}
};


int main()
{
	Program::Main();
	return 0;
}









