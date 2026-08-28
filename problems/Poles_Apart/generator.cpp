#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    registerGen(argc, argv, 1);
    cout << 0 << '\n';
    cout << 200000 << '\n';
    cout << rnd.next(0, 1000000000) << '\n';
    cout << 2000 << ' ' << 43600 << '\n';
    vector<bitset<2001>> vis(2001);
    for (int i = 0; i < 43600; i++)
    {
        int u = rnd.next(1, 2000);
        int v = rnd.next(1, 2000);
        while (vis[u][v] || u == v)
        {
            u = rnd.next(1, 2000);
            v = rnd.next(1, 2000);
        }
        vis[u][v] = vis[v][u] = 1;
        cout << u << ' ' << v << '\n';
    }
    return 0;
}