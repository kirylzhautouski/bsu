#include <fstream>
#include <algorithm>
#include <limits>

using namespace std;

int main()
{
	ifstream fin("input.txt");
	ofstream fout("output.txt");

	size_t n;
	fin >> n;

	int **table = new int *[n];
	for (int i = 0; i < n; i++)
	{
		table[i] = new int[n];

		for (int j = 0; j < n; j++)
			table[i][j] = 0;
	}

	int *cell_for_card = new int[n];
	for (int i = 0; i < n; i++)
	{
		int card;
		fin >> card;

		cell_for_card[card - 1] = i + 1;
	}

	for (int m = 1; m < n; m++)
	{
		for (int i = 0; i < n - m; i++)
		{
			int j = i + m;
			table[i][j] = numeric_limits<int>::max();

			for (int q = i + 1; q <= j; q++)
				table[i][j] = min(table[i][q - 1] + table[q][j] + abs(cell_for_card[q - 1] - cell_for_card[j]),
					table[i][j]);
		}
	}

	fout << table[0][n - 1];

	for (int i = 0; i < n; i++)
		delete[] table[i];

	delete[] table;

	fin.close();
	fout.close();

	return 0;
}