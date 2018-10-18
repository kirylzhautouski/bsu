#include <fstream>
#include <iostream>

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

		int *first_vector = nullptr, *second_vector = nullptr,
			*slices = nullptr, *steps = nullptr, l = 0, all_slice_size = 0, rest_size = 0;
		if (process.IsMaster())
		{
			fin.open("input/task1.2_input.txt");
			fout.open("output/task1.2_output.txt");

			fin >> l;

			all_slice_size = l / process.ProcessCount();
			rest_size = l % process.ProcessCount();

			auto process_count = process.ProcessCount();

			first_vector = new int[l];
			second_vector = new int[l];
	
			ReadVector(fin, first_vector, l);
			ReadVector(fin, second_vector, l);

			slices = new int[process_count];
			steps = new int[process_count];
	
			all_slice_size = l / process_count;
			rest_size = l % process_count;

			for (int i = 0; i < process_count; i++)
			{
				slices[i] = all_slice_size;
				steps[i] = all_slice_size * i;
			}

			for (int i = 0; i < rest_size; i++)
			{
				slices[i]++;

				for (int j = i + 1; j < process_count; j++)
					steps[j]++;
			}
		}

		int process_slice;

		MPI_Scatter(slices, 1, MPI_INT, &process_slice, 1, MPI_INT, MPI::MasterRank, MPI_COMM_WORLD);

		auto first_buffer = new int[process_slice];
		auto second_buffer = new int[process_slice];

		Scatterv(first_vector, slices, steps, first_buffer, process_slice);
		Scatterv(second_vector, slices, steps, second_buffer, process_slice);
		
		long slice_sum = 0;

		for (int i = 0; i < process_slice; i++)
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
			std::cout << "Result S = " << result_sum << std::endl;

			delete[] first_vector;
			delete[] second_vector;

			delete[] slices;
			delete[] steps;

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

	static void Scatterv(int* source, int* slices, int* steps, int* buffer, int rec_count)
	{
		MPI_Scatterv(
			source,
			slices,
			steps,
			MPI_INT,
			buffer,
			rec_count,
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








