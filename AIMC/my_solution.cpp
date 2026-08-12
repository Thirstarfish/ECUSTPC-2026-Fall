#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'
#define vec vector
#define pb push_back
typedef array<int, 2> aii;
typedef array<int, 3> aiii;
const int inf = 0x3f3f3f3f3f3f3f3f;
const vec<vec<aiii>> dir = {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {-1, 0, 0}, {0, -1, 0}, {0, 0, -1}, {0, 0, 0}},
                            {{1, 1, 0}, {1, 0, 1}, {0, 1, 1}, {-1, -1, 0}, {-1, 0, -1}, {0, -1, -1}, {1, -1, 0}, {1, 0, -1}, {0, 1, -1}, {-1, 1, 0}, {-1, 0, 1}, {0, -1, 1}, {0, 0, 0}},
                            {{1, 1, 1}, {1, 1, -1}, {1, -1, 1}, {1, -1, -1}, {-1, 1, 1}, {-1, 1, -1}, {-1, -1, 1}, {-1, -1, -1}, {0, 0, 0}}};

struct dinic
{
    int n;
    vec<vec<int>> g;
    vec<aii> e;
    vec<int> dis, cur;

    dinic(int _n)
    {
        n = _n;
        g.resize(n + 1);
    }

    void add(int u, int v, int w)
    {
        g[u].pb(e.size());
        e.pb({v, w});
        g[v].pb(e.size());
        e.pb({u, 0});
    }

    bool bfs(int s, int t)
    {
        dis.assign(n + 1, -1);
        queue<int> q;
        dis[s] = 0;
        q.push(s);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (auto i : g[u])
            {
                auto &[v, c] = e[i];
                if (c > 0 && dis[v] == -1)
                {
                    dis[v] = dis[u] + 1;
                    if (v == t)
                        return true;
                    q.push(v);
                }
            }
        }
        return false;
    }

    int dfs(int u, int t, int f)
    {
        if (u == t)
            return f;
        int r = f;
        for (int &i = cur[u]; i < (int)(g[u].size()); i++)
        {
            int j = g[u][i];
            auto &[v, c] = e[j];
            if (c > 0 && dis[v] == dis[u] + 1)
            {
                int temp = dfs(v, t, min(r, c));
                e[j][1] -= temp;
                e[j ^ 1][1] += temp;
                r -= temp;
                if (r == 0)
                    return f;
            }
        }
        return f - r;
    }

    int work(int s, int t)
    {
        int ans = 0;
        while (bfs(s, t))
        {
            cur.assign(n + 1, 0);
            ans += dfs(s, t, inf);
        }
        return ans;
    }
};

void cold_beans()
{
    int n, m, h;
    cin >> n >> m >> h;
    vec a(8, vec(n + 1, vec(m + 1, vec<int>(h + 1))));
    for (int i = 0; i < 8; i++)
        for (int j = 1; j <= n; j++)
            for (int k = 1; k <= m; k++)
                for (int l = 1; l <= h; l++)
                    cin >> a[i][j][k][l];

    auto id = [&](int i, int j, int k) -> int
    { return (i - 1) * m * h + (j - 1) * h + k; };

    int S = 7 * n * m * h + 1, T = 7 * n * m * h + 2;
    dinic flow(T);

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            for (int k = 1; k <= h; k++)
            {
                flow.add(S, id(i, j, k), a[~(i + j + k) & 1][i][j][k]);
                flow.add(id(i, j, k), T, a[i + j + k & 1][i][j][k]);
            }

    for (int l = 0; l < 3; l++)
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                for (int k = 1; k <= h; k++)
                {
                    int ID = (2 * l + 1) * n * m * h + id(i, j, k);
                    flow.add(S, ID, a[(2 * l + 2) | (~(i + j + k) & 1)][i][j][k]);
                    flow.add(n * m * h + ID, T, a[(2 * l + 2) | ((i + j + k) & 1)][i][j][k]);
                    for (auto [u, v, w] : dir[l])
                    {
                        int ni = i + u, nj = j + v, nk = k + w;
                        if (ni < 1 || ni > n || nj < 1 || nj > m || nk < 1 || nk > h)
                            continue;
                        flow.add(ID, id(ni, nj, nk), inf);
                        flow.add(id(ni, nj, nk), n * m * h + ID, inf);
                    }
                }

    int sum = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 1; j <= n; j++)
            for (int k = 1; k <= m; k++)
                for (int l = 1; l <= h; l++)
                    sum += a[i][j][k][l];
    cout << sum - flow.work(S, T) << endl;
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cout << fixed << setprecision(10);
    int _ = 1;
    // cin >> _;
    while (_--)
        cold_beans();
    return 0;
}

/*
騒がしい日々に笑えない君に
思い付く限り眩しい明日を
*/
