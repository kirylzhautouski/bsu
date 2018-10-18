#include <fstream>
#include <iostream>

#include "Process.h"
#include "mpi.h"


class Program
{
public:
	static void Main()
	{
		auto process = MPI::Process();
		
		std::ifstream fin;
		std::ofstream fout;

		int l, **ab = nullptr, *x = nullptr;

		if (process.IsMaster())
		{
			fin.open("input/task2_input.txt");
			fout.open("output/task2.1_output.txt");
			
			fin >> l;

			ab = new int*[l];

			for (int i = 0; i < l; i++)
				ab[i] = new int[l + 1];

			for (int i = 0; i < l; i++)
			{
				for (int j = 0; j < l + 1; j++)
					fin >> ab[i][j];
			}
		}
		
		MPI_Bcast(&l, 1, MPI_INT, MPI::MasterRank, MPI_COMM_WORLD);

		x = new int[l];

		if (process.IsMaster())
		{
			for (int i = 0; i < l; i++)
				fin >> x[i];
		}
			
		MPI_Bcast(x, l, MPI_INT, MPI::MasterRank, MPI_COMM_WORLD);

		auto slice_size = l / process.ProcessCount();

		auto d = new int[slice_size];
		auto slice_ab = new int*[slice_size];

		for (int i = 0; i < slice_size; i++)
			slice_ab[i] = new int[l + 1];
		
		if (process.IsMaster())
		{
			SendData(process, ab, l, slice_size);
			
			for (int i = 0; i < slice_size; i++)
			{
				for (int j = 0; j < l + 1; j++)
				{
					slice_ab[i][j] = ab[i][j];
				}
			}
		}
		else
		{
			ReceiveData(slice_ab, l, slice_size);
		}

		for (int i = 0; i < slice_size; i++)
		{
			int b1 = 0;
			for (int j = 0; j < l; j++)
				b1 += slice_ab[i][j] * x[j];

			d[i] = abs(b1 - slice_ab[i][l]);
		}

		int *result_d = nullptr;
		if (process.IsMaster())
			result_d = new int[l];

		MPI_Gather(
			d,
			slice_size,
			MPI_INT,
			result_d,
			slice_size,
			MPI_INT,
			MPI::MasterRank,
			MPI_COMM_WORLD
		);

		for (int i = 0; i < slice_size; i++)
			delete[] slice_ab[i];

		delete[] slice_ab;
		delete[] d;
		delete[] x;

		if (process.IsMaster())
		{
			for (int i = 0; i < l; i++)
			{
				//std::cout << result_d[i] << " ";
				fout << result_d[i] << " ";
			}

			//std::cout << "\n\n\n";
			delete[] result_d;
			

			for (int i = 0; i < l; i++)
				delete[] ab[i];

			delete[] ab;

			fout.close();
			fin.close();
		}
	}
private:
	static void SendData(const MPI::Process& process, int** source, int l, int slice_size)
	{
		for (int rank = 1; rank < process.ProcessCount(); rank++)
		{
			for (int i = 0; i < slice_size; i++)
			{
				MPI_Send(source[i + rank * slice_size], l + 1, MPI_INT, rank, i, MPI_COMM_WORLD);
			}
		}
	}

	static void ReceiveData(int** destination, int l, int slice_size)
	{
		for (int i = 0; i < slice_size; i++)
		{
			MPI_Recv(destination[i], l + 1, MPI_INT, MPI::MasterRank, i, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		}
	}
};

int main()
{
	Program::Main();
	return 0;
}