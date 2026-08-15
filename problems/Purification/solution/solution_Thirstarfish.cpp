#include <bits/stdc++.h>
using namespace std;

void cold_beans()
{
    int n;
    cin >> n;
    unordered_map<int, vector<array<int, 2>>> r, c;
    vector<vector<int>> g(n + 1);
    vector<int> deg(n + 1);
    for (int i = 1; i <= n; i++)
    {
        int x, y;
        cin >> x >> y;
        r[x].push_back({y, i});
        c[y].push_back({x, i});
    }
    for (auto &[_, v] : r)
        if (v.size() > 3)
        {
            cout << "NO" << endl;
            return;
        }
        else if (v.size() == 3)
        {
            ranges::sort(v);
            g[v[1][1]].push_back(v[0][1]);
            g[v[1][1]].push_back(v[2][1]);
            deg[v[0][1]]++;
            deg[v[2][1]]++;
        }
    for (auto &[_, v] : c)
        if (v.size() > 3)
        {
            cout << "NO" << endl;
            return;
        }
        else if (v.size() == 3)
        {
            ranges::sort(v);
            g[v[1][1]].push_back(v[0][1]);
            g[v[1][1]].push_back(v[2][1]);
            deg[v[0][1]]++;
            deg[v[2][1]]++;
        }

    int cnt = 0;
    queue<int> q;
    for (int i = 1; i <= n; i++)
        if (!deg[i])
            q.push(i);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        cnt++;
        for (auto v : g[u])
        {
            deg[v]--;
            if (!deg[v])
                q.push(v);
        }
    }
    cout << (cnt == n ? "YES" : "NO") << '\n';
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