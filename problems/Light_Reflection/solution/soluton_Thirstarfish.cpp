#include <bits/stdc++.h>
using namespace std;

void cold_beans()
{
    int n, m;
    cin >> n >> m;
    vector a(n + 1, vector<char>(m + 1));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            cin >> a[i][j];
    char d1, d2;
    int p1, p2;
    cin >> d1 >> p1 >> d2 >> p2;

    vector<vector<int>> g(n * m * 4 + 1);

    auto id = [&](int i, int j, int d) -> int
    { return d * n * m + (i - 1) * m + j; };

    auto add = [&](int u, int v) -> void
    {
        g[u].push_back(v);
        g[v].push_back(u);
    };

    int S, T;
    if (d1 == 'U')
        S = id(1, p1, 0);
    else if (d1 == 'R')
        S = id(p1, m, 1);
    else if (d1 == 'D')
        S = id(n, p1, 2);
    else
        S = id(p1, 1, 3);

    if (d2 == 'U')
        T = id(1, p2, 0);
    else if (d2 == 'R')
        T = id(p2, m, 1);
    else if (d2 == 'D')
        T = id(n, p2, 2);
    else
        T = id(p2, 1, 3);

    //   0
    // 3 x 1
    //   2

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m - 1; j++)
            add(id(i, j, 1), id(i, j + 1, 3));
    for (int i = 1; i <= n - 1; i++)
        for (int j = 1; j <= m; j++)
            add(id(i, j, 2), id(i + 1, j, 0));

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (a[i][j] == '/')
            {
                add(id(i, j, 0), id(i, j, 3));
                add(id(i, j, 1), id(i, j, 2));
            }
            else if (a[i][j] == '\\')
            {
                add(id(i, j, 0), id(i, j, 1));
                add(id(i, j, 2), id(i, j, 3));
            }
            else
            {
                add(id(i, j, 0), id(i, j, 3));
                add(id(i, j, 1), id(i, j, 2));
                add(id(i, j, 0), id(i, j, 1));
                add(id(i, j, 2), id(i, j, 3));
                add(id(i, j, 0), id(i, j, 2));
                add(id(i, j, 1), id(i, j, 3));
            }

    vector<int> from(n * m * 4 + 1);
    queue<int> q;
    q.push(S);
    from[S] = INT_MAX;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (auto v : g[u])
        {
            if (from[v])
                continue;
            from[v] = u;
            q.push(v);
        }
    }
    if (!from[T])
    {
        cout << "IMPOSSIBLE" << endl;
        return;
    }

    auto rev = [&](int u) -> array<int, 3>
    {
        array<int, 3> ret;
        ret[2] = (u - 1) / (n * m);
        u -= ret[2] * n * m;
        ret[0] = (u - 1) / m + 1;
        ret[1] = u - (ret[0] - 1) * m;
        return ret;
    };

    int cur = T;
    while (1)
    {
        if (from[cur] == INT_MAX)
            break;
        auto old = rev(from[cur]), now = rev(cur);
        if (old[0] == now[0] && old[1] == now[1])
        {
            if (old[2] + now[2] == 3)
                a[old[0]][old[1]] = '/';
            else if (abs(old[2] - now[2]) == 2)
                a[old[0]][old[1]] = '.';
            else
                a[old[0]][old[1]] = '\\';
        }
        cur = from[cur];
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
            cout << a[i][j];
        cout << '\n';
    }
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    int _;
    cin >> _;
    while (_--)
        cold_beans();
}