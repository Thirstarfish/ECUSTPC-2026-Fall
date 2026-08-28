#include <bits/extc++.h>
#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
#define vec vector
#define pb push_back
typedef array<int, 2> aii;
typedef array<int, 3> aiii;
typedef array<int, 4> a4i;
const vec<aiii> dir = {{0, 1, 2}, {0, 1, 3}, {0, 2, 3}, {1, 2, 3}};

struct chash
{
    static uint64_t u64(uint64_t x)
    {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const
    {
        static const uint64_t rg = chrono::steady_clock::now().time_since_epoch().count();
        return u64(x + rg);
    }

    template <typename Range>
    size_t operator()(const Range &arr) const
    {
        static const uint64_t rg = chrono::steady_clock::now().time_since_epoch().count();
        uint64_t hash = 0;
        for (auto x : arr)
            hash ^= u64(x + rg) + 0x9e3779b97f4a7c15 + (hash << 6) + (hash >> 2);
        return hash;
    }
};

vec<vec<int>> g;
bitset<2001> vis[2001];
__gnu_pbds::gp_hash_table<a4i, aii, chash> mp2; 
__gnu_pbds::gp_hash_table<aiii, aii, chash> mp1;
vec<a4i> rev{{0, 0, 0, 0}};
vec<int> ans, idx;
int N = 200000, n, m;

void Thirstarfish()
{
    int cnt = 0;
    vec<vec<int>> gtemp(2001);
    for (int u = 1; u <= n; u++)
    {
        for (auto x : g[u])
            for (auto v : g[x])
            {
                if (v == u)
                    continue;
                gtemp[v].pb(x);
            }
        for (int v = 1; v <= n; v++)
        {
            if (v == u)
                continue;
            for (int i = 0; i < gtemp[v].size(); i++)
                for (int j = i + 1; j < gtemp[v].size(); j++)
                {
                    a4i temp = {u, gtemp[v][i], gtemp[v][j], v};
                    if (vis[u][v] || vis[gtemp[v][i]][gtemp[v][j]])
                        continue;
                    sort(temp.begin(), temp.end());
                    mp2[temp] = {0, ++cnt};
                    rev.pb(temp);
                }
        }
        for (int v = 1; v <= n; v++)
            gtemp[v].clear();
    }

    for (auto &[a, _] : mp2)
        for (auto d : dir)
            mp1[{a[d[0]], a[d[1]], a[d[2]]}][0]++;
    for (auto &[a, _] : mp2)
        for (auto d : dir)
            _[0] += mp1[{a[d[0]], a[d[1]], a[d[2]]}][0];

    vec<int> order(cnt);
    iota(order.begin(), order.end(), 1);
    sort(order.begin(), order.end(), [&](int i, int j)
         { return mp2[rev[i]][0] < mp2[rev[j]][0]; });

    ans.assign(cnt + 1, 0);

    for (auto idx : order)
    {
        bool f = 1;
        for (auto d : dir)
        {
            aiii temp = {rev[idx][d[0]], rev[idx][d[1]], rev[idx][d[2]]};
            if (mp1[temp][1])
                f = 0;
        }
        if (f)
        {
            ans[idx] = 1;
            for (auto d : dir)
            {
                aiii temp = {rev[idx][d[0]], rev[idx][d[1]], rev[idx][d[2]]};
                mp1[temp][1] = idx;
            }
        }
    }

    for (auto idx : order)
    {
        if (ans[idx])
            continue;
        int old = 0;
        bool f = 1;
        for (auto d : dir)
        {
            aiii temp = {rev[idx][d[0]], rev[idx][d[1]], rev[idx][d[2]]};
            if (mp1[temp][1])
                if (!old)
                    old = mp1[temp][1];
                else
                {
                    f = 0;
                    break;
                }
        }
        if (f && old)
        {
            ans[old] = 0;
            for (auto d : dir)
            {
                aiii temp = {rev[old][d[0]], rev[old][d[1]], rev[old][d[2]]};
                mp1[temp][1] = 0;
            }
            ans[idx] = 1;
            for (auto d : dir)
            {
                aiii temp = {rev[idx][d[0]], rev[idx][d[1]], rev[idx][d[2]]};
                mp1[temp][1] = idx;
            }
        }
    }
    for (auto idx : order)
    {
        bool f = 1;
        for (auto d : dir)
        {
            aiii temp = {rev[idx][d[0]], rev[idx][d[1]], rev[idx][d[2]]};
            if (mp1[temp][1])
            {
                f = 0;
                break;
            }
        }
        if (f)
        {
            ans[idx] = 1;
            for (auto d : dir)
            {
                aiii temp = {rev[idx][d[0]], rev[idx][d[1]], rev[idx][d[2]]};
                mp1[temp][1] = idx;
            }
        }
    }

    idx.assign(N + 1, 0);
    int tot = 0, cur = 1;
    while (tot < N)
    {
        if (ans[cur])
        {
            idx[++tot] = cur;
            ans[cur] = tot;
        }
        ++cur;
    }
}

void Liuguang_Ji()
{
    cin >> N >> n >> m;
    g.assign(n + 1, {});
    for (int i = 1; i <= m; i++)
    {
        int u, v;
        cin >> u >> v;
        g[u].pb(v);
        g[v].pb(u);
        vis[u][v] = vis[v][u] = 1;
    }
    Thirstarfish();
    for (int i = 1; i <= N; i++)
    {
        for (auto b : rev[idx[i]])
            cout << b << ' ';
        cout << endl;
    }
}

void AngleHai()
{
    cin >> n >> m;
    g.assign(n + 1, {});
    for (int i = 1; i <= m; i++)
    {
        int u, v;
        cin >> u >> v;
        g[u].pb(v);
        g[v].pb(u);
        vis[u][v] = vis[v][u] = 1;
    }
    Thirstarfish();
    int q;
    cin >> q;
    while (q--)
    {
        aiii temp;
        for (auto &b : temp)
            cin >> b;
        sort(temp.begin(), temp.end());
        cout << ans[mp1[temp][1]] << endl;
    }
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string s;
    cin >> s;
    int _ = 1;
    // cin >> _; 
    if (s == "send")
        while (_--)
            Liuguang_Ji();
    else
        while (_--)
            AngleHai();
    return 0;
}

