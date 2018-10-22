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

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int x;
            fin >> x;

            if (x)
                tree[j] = i + 1;
        }
    }

    for (auto vertex : tree)
        fout << vertex << " ";

    fin.close();
    fout.close();

    return 0;
}