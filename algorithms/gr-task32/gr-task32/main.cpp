#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Arc
{
	int from;
	int to;
	double weight;

	Arc(int from, int to, double weight) : from(from), to(to), weight(weight) {}
};

bool HasNegativeCycle(int n, const vector<Arc>& arcs)
{
	vector<double> d(n, numeric_limits<int>::max());

	d[0] = 0;
	for (int i = 0; i < n - 1; i++)
	{
		for (const auto& arc : arcs)
		{
			d[arc.to] = min(d[arc.to], d[arc.from] + arc.weight);
		}
	}

	for (const auto& arc : arcs)
	{
		if (d[arc.to] > d[arc.from] + arc.weight)
			return true;
	}

	return false;
}

double FindDistance(const pair<int, int>& first_point, const pair<int, int>& second_point)
{
	return sqrt((first_point.first - second_point.first) * (first_point.first - second_point.first) + 
				(first_point.second - second_point.second) * (first_point.second - second_point.second));
}

int main()
{
	ifstream fin("input.txt");
	ofstream fout("output.txt");

	unsigned n;
	fin >> n;

	vector<pair<int, int>> point_coordinates(2 * n);
	for (unsigned i = 0; i < 2 * n; i++)
	{
		int x, y;
		fin >> x >> y;

		point_coordinates[i] = { x, y };
	}

	vector<vector<char>> matching(n, vector<char>(n, 0));
	vector<Arc> arcs;
	for (unsigned i = 0; i < n; i++)
	{
		int from, to;
		fin >> from >> to;

		matching[from - 1][to - n - 1] = 1;
		arcs.emplace_back(from - 1, to - 1, 
			-FindDistance(point_coordinates[from - 1], point_coordinates[to - 1]));
	}

	for (unsigned i = 0; i < n; i++)
	{
		for (unsigned j = 0; j < n; j++)
		{
			if (matching[i][j] == 0)
			{
				arcs.emplace_back(j + n, i, FindDistance(point_coordinates[i], point_coordinates[j + n]));
			}
		}
	}

	if (HasNegativeCycle(2 * n, arcs))
		fout << "No";
	else
		fout << "Yes";

	fout.close();
	fin.close();
}