#include <fstream>
#include <vector>
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

		//screening_numbers - простые числа из [2, sqrt(n)], которыми потом 
		//просеиваем все промежутки из (sqrt(n), n]
		unsigned *screening_numbers = nullptr, *numbers = nullptr,
			max = 0, l = 0, all_slice_size = 0, rest_size = 0;
		int *slices = nullptr, *steps = nullptr;
		int *primes = nullptr;
		unsigned screening_numbers_size = 0;
		
		if (process.IsMaster())
		{
			fin.open("input/task3_input.txt");
			fout.open("output/task3_output.txt");

			fin >> l;

			numbers = new unsigned[l];
			for (unsigned i = 0; i < l; i++)
			{
				fin >> numbers[i];

				if (numbers[i] > max)
					max = numbers[i];
			}

			auto process_count = process.ProcessCount();
			steps = new int[process_count];
			slices = new int[process_count];

			all_slice_size = (max - static_cast<int>(sqrt(max))) / process_count;
			rest_size = (max - static_cast<int>(sqrt(max))) % process_count;

			for (int i = 0; i < process_count; i++)
			{
				slices[i] = all_slice_size;
				steps[i] = static_cast<int>(sqrt(max)) + 1 + all_slice_size * i;
			}

			for (unsigned i = 0; i < rest_size; i++)
			{
				slices[i]++;

				for (int j = i + 1; j < process_count; j++)
					steps[j]++;
			}

			primes = new int[max + 1];
			for (unsigned i = 0; i < max + 1; i++)
				primes[i] = 1;
			primes[0] = primes[1] = 0;

			screening_numbers_size = FindPrimes(2, static_cast<int>(sqrt(max)), primes);
		}

		MPI_Bcast(&screening_numbers_size, 1, MPI_UNSIGNED, MPI::MasterRank, MPI_COMM_WORLD);
		screening_numbers = new unsigned[screening_numbers_size];
		
		if (process.IsMaster())
			FindScreeningNumbers(screening_numbers);
		
		MPI_Bcast(screening_numbers, screening_numbers_size, MPI_UNSIGNED, MPI::MasterRank, MPI_COMM_WORLD);
		
		unsigned process_slice = 0, process_step = 0;
		MPI_Scatter(slices, 1, MPI_UNSIGNED, &process_slice, 1, MPI_UNSIGNED, 
			MPI::MasterRank, MPI_COMM_WORLD);
		MPI_Scatter(steps, 1, MPI_UNSIGNED, &process_step, 1, MPI_UNSIGNED,
			MPI::MasterRank, MPI_COMM_WORLD);
		
		auto slice_primes = new int[process_slice];
		for (unsigned i = 0; i < process_slice; i++)
			slice_primes[i] = 1;
		
		for (unsigned i = 0; i < screening_numbers_size; i++)
		{
			auto first_number_in_slice = (process_step / screening_numbers[i]) * screening_numbers[i];
			if (first_number_in_slice < process_step)
				first_number_in_slice += screening_numbers[i];

			if (first_number_in_slice <= process_step + process_slice)
			{
				for (unsigned j = first_number_in_slice;
					j <= process_step + process_slice - 1; j += screening_numbers[i])
				{
					slice_primes[j - process_step] = 0;
				}
			}
		}
		
		MPI_Gatherv(slice_primes, process_slice, MPI_INT, primes, slices, steps, 
			MPI_INT, MPI::MasterRank, MPI_COMM_WORLD);

		delete[] screening_numbers;
		delete[] slice_primes;
		if (process.IsMaster())
		{
			for (unsigned i = 0; i < l; i++)
			{
				fout << numbers[i] << ": " << primes[numbers[i]] << std::endl;
				std::cout << numbers[i] << ": " << primes[numbers[i]] << std::endl;	
			}
			
			delete[] slices;
			delete[] steps;
			delete[] numbers;
			delete[] primes;
			
			fout.close();
			fin.close();
		}
		
	}
private:
	static std::vector<unsigned> interval_prime_numbers;

	static unsigned FindPrimes(unsigned n1, unsigned n2, int* primes)
	{
		unsigned j = 0;

		for (unsigned i = n1; i * 1ll* i <= n2; i++)
		{
			if (primes[i] == 1)
			{
				if (i * 1ll * i <= n2)
				{
					for (unsigned j = i * i; j <= n2; j += i)
						primes[j] = 0;
				}
				j = i + 1;
				interval_prime_numbers.push_back(i);
			}
		}

		for (; j < n2 + 1; j++)
			if (primes[j] == 1)
				interval_prime_numbers.push_back(j);

		return (unsigned)interval_prime_numbers.size();
	}

	static void FindScreeningNumbers(unsigned* numbers)
	{
		auto size = (unsigned)interval_prime_numbers.size();
		for (unsigned i = 0; i < size; i++)
		{
			numbers[i] = Program::interval_prime_numbers[i];
		}
	}
};

std::vector<unsigned> Program::interval_prime_numbers = {};

int main()
{
	Program::Main();
	return 0;
}