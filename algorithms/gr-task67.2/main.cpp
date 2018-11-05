#include <fstream>
#include <vector>

using namespace std;

void dfs(size_t v, const vector<vector<int>>& g, vector<char>& visited, vector<int>& labels, int& time)
{
    visited[v] = 1;
    labels[v] = time++;

    for (size_t i = 0; i < g.size(); i++)
    {
        if (g[v][i] && !visited[i])
            dfs(i, g, visited, labels, time);
    }
}

int main()
{
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    size_t n;
    fin >> n;

    vector<vector<int>> g(n, vector<int>(n, 0));

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            fin >> g[i][j];
        }
    }

    vector<char> visited(n, 0);
    vector<int> labels(n, 0);
    int time = 1;

    for (size_t i = 0; i < n; i++)
    {
        if (!visited[i])
        {
            dfs(i, g, visited, labels, time);
        }
    }

    for (auto i : labels)
        fout << i << " ";

    fout.close();
    fin.close();

    return 0;
}