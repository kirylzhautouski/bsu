#include <fstream>
#include <vector>

using namespace std;

int main()
{
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    int n;
    fin >> n;

    vector<int> tree(n, 0);
    for (int i = 0; i < n - 1; i++)
    {
        int u, v;
        fin >> u >> v;

        tree[v - 1] = u;
    }

    for (auto vertex : tree)
        fout << vertex << " ";

    fin.close();
    fout.close();

    return 0;
}