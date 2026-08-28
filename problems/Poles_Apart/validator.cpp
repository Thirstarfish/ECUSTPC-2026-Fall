#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    registerValidation(argc, argv);

    int run = inf.readInt(0, 0, "run");
    inf.readEoln();
    int n = inf.readInt(200000, 200000, "n");
    inf.readEoln();
    int seed = inf.readInt(0, 1000000000, "seed");
    inf.readEoln();
    int V = inf.readInt(2000, 2000, "V");
    inf.readSpace();
    int E = inf.readInt(43600, 43600, "E");
    inf.readEoln();
    vector g(V + 1, vector<char>(V + 1));
    for (int i = 0; i < E; i++)
    {
        int u = inf.readInt(1, V, "u");
        inf.readSpace();
        int v = inf.readInt(1, V, "v");
        inf.readEoln();
        ensuref(g[u][v] == 0 && g[v][u] == 0, "Duplicate edges detected at edge %d", i + 1);
        ensuref(u != v, "Self-loop detected at edge %d", i + 1);
        g[u][v] = g[v][u] = 1;
    }

    inf.readEof();

    return 0;
}
