#include <fstream>
#include <queue>

using namespace std;

struct Vertex
{
    int n;
    long long weight;
};

struct Comparator
{
    bool operator() (const Vertex& lhs, const Vertex& rhs)
    {
        return lhs.weight > rhs.weight;
    }
};

int main()
{
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    int n, m;
    fin >> n >> m;

    vector<vector<pair<int, int>>> g(n, vector<pair<int, int>>());
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        fin >> u >> v >> w;
        g[u - 1].push_back({ v - 1, w });
        g[v - 1].push_back({ u - 1, w });
    }

    priority_queue<Vertex, vector<Vertex>, Comparator> q;
    vector<char> visited(n, 0);

    q.push(Vertex{ 0, 0 });
    while (!q.empty())
    {
        auto v = q.top();
        q.pop();

        if (v.n == n - 1)
        {
            fout << v.weight;
            break;
        }

        if (!visited[v.n])
        {
            visited[v.n] = 1;

            for (const auto& i : g[v.n])
            {
                if (!visited[i.first])
                    q.push(Vertex{ i.first, v.weight + i.second });
            }
        }
    }

    fout.close();
    fin.close();

    return 0;
}