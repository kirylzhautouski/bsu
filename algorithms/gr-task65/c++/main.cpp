#include <fstream>
#include <vector>

using namespace std;

int main()
{
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    int n, m;
    fin >> n >> m;

    vector<vector<int>> adjacency_list(n);

    for (int i = 0; i < m; i++)
    {
        int u, w;
        fin >> u >> w;

        adjacency_list[u - 1].push_back(w);
        adjacency_list[w - 1].push_back(u);
    }

    for (int i = 0; i < n; i++)
    {
        fout << adjacency_list[i].size() << " ";

        for (int j = 0; j < adjacency_list[i].size(); j++)
            fout << adjacency_list[i][j] << " ";

        fout << endl;
    }

    fout.close();
    fin.close();

    return 0;
}