#include <fstream>
#include <vector>
#include <queue>

using namespace std;

void bfs(int start, int n, int& label, const vector<vector<int>>& v, vector<int>& labels, vector<char>& visited)
{
	queue<int> q;
	q.push(start);
	visited[start] = 1;
	labels[start] = label++;

	while (!q.empty())
	{
		int vertex = q.front();
		q.pop();

		for (int i = 0; i < n; i++)
		{
			if (!visited[i] && v[vertex][i])
			{
				q.push(i);
				visited[i] = 1;

				labels[i] = label++;
			}
		}
	}
}

int main()
{
	ifstream fin("input.txt");
	ofstream fout("output.txt");

	int n;
	fin >> n;

	vector<vector<int>> v(n, vector<int> (n));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fin >> v[i][j];
		}
	}

	int label = 1;

	vector<int> labels(n);
	vector<char> visited(n);
	
	for (int i = 0; i < n; i++)
	{
		if (!visited[i])
			bfs(i, n, label, v, labels, visited);
	}

	for (auto i : labels)
		fout << i << " ";

	fout.close();
	fin.close();
	return 0;
}