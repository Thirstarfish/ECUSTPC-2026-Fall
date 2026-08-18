// Slightly modified copy of solution_Thirstarfish_opt.cpp, for offline testing.
//
// The core algorithm `Thirstarfish()` is copied verbatim.  The only changes are
// in the driver: instead of reading "send"/"receive" and printing the selected
// cycles, it (a) can generate a random graph matching the statement's
// convention, and (b) reports how many pairwise-triple-disjoint induced 4-cycles
// the greedy managed to select, so we can measure the success probability on
// random graphs.
//
// Usage:
//   prog file              -- read "n m" + m edges from stdin (graph.txt format)
//   prog [T [seed]]        -- generate T random graphs (default T=1000), report success rate
//
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
vec<array<aiii, 4>> tri;
vec<int> ans, idx;
int N = 200000, n, m;

void Thirstarfish()
{
    int cnt = 0;
    vec<vec<int>> gtemp(2001);
    vec<int> touched(2001);
    for (int u = 1; u <= n; u++)
    {
        for (auto x : g[u])
        {
            if (x <= u)
                continue;
            for (auto v : g[x])
            {
                if (v == u)
                    continue;
                if (gtemp[v].empty())
                    touched.pb(v);
                gtemp[v].pb(x);
            }
        }
        for (int v : touched)
        {
            if (v > u)
            {
                auto &gv = gtemp[v];
                int sz = (int)gv.size();
                for (int i = 0; i < sz; i++)
                    for (int j = i + 1; j < sz; j++)
                    {
                        if (vis[u][v] || vis[gv[i]][gv[j]])
                            continue;
                        a4i temp = {u, gv[i], gv[j], v};
                        sort(temp.begin(), temp.end());
                        mp2[temp] = {0, ++cnt};
                        rev.pb(temp);
                    }
            }
            gtemp[v].clear();
        }
        touched.clear();
    }

    tri.assign(cnt + 1, {});
    for (int i = 1; i <= cnt; i++)
    {
        auto &c4 = rev[i];
        tri[i][0] = {c4[0], c4[1], c4[2]};
        tri[i][1] = {c4[0], c4[1], c4[3]};
        tri[i][2] = {c4[0], c4[2], c4[3]};
        tri[i][3] = {c4[1], c4[2], c4[3]};
    }

    for (int i = 1; i <= cnt; i++)
        for (auto &t : tri[i])
            mp1[t][0]++;
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
        for (auto &t : tri[idx])
        {
            if (mp1[t][1])
                f = 0;
        }
        if (f)
        {
            ans[idx] = 1;
            for (auto &t : tri[idx])
                mp1[t][1] = idx;
        }
    }

    for (auto idx : order)
    {
        if (ans[idx])
            continue;
        int old = 0;
        bool f = 1;
        for (auto &t : tri[idx])
        {
            if (mp1[t][1])
                if (!old)
                    old = mp1[t][1];
                else
                {
                    f = 0;
                    break;
                }
        }
        if (f && old)
        {
            ans[old] = 0;
            for (auto &t : tri[old])
                mp1[t][1] = 0;
            ans[idx] = 1;
            for (auto &t : tri[idx])
                mp1[t][1] = idx;
        }
    }
    for (auto idx : order)
    {
        bool f = 1;
        for (auto &t : tri[idx])
        {
            if (mp1[t][1])
            {
                f = 0;
                break;
            }
        }
        if (f)
        {
            ans[idx] = 1;
            for (auto &t : tri[idx])
                mp1[t][1] = idx;
        }
    }
}

// ---- test-only additions below (NOT part of the original solution) ----

// Uniform random graph with `n` vertices and `m` edges, matching the statement:
// repeatedly pick a uniformly random pair of currently-unconnected vertices and
// add the edge (rejection sampling of a uniform random pair is equivalent).
void gen_graph(int n, int m, uint64_t seed)
{
    g.assign(n + 1, {});
    for (int i = 0; i <= n; i++)
        vis[i].reset();
    mt19937_64 rng(seed);
    int added = 0;
    while (added < m)
    {
        int u = (int)(rng() % n) + 1;
        int v = (int)(rng() % n) + 1;
        if (u == v)
            continue;
        if (u > v)
            swap(u, v);
        if (vis[u][v])
            continue;
        vis[u][v] = vis[v][u] = 1;
        g[u].pb(v);
        g[v].pb(u);
        ++added;
    }
}

void reset_globals()
{
    g.assign(n + 1, {});
    for (int i = 0; i <= n; i++)
        vis[i].reset();
    mp2.clear();
    mp1.clear();
    rev.assign(1, {0, 0, 0, 0});
    tri.clear();
    ans.clear();
    idx.clear();
}

// returns {total induced 4-cycles, selected pairwise-triple-disjoint cycles}
pair<long long, long long> run_trial()
{
    Thirstarfish();
    long long total = (long long)mp2.size();
    long long selected = 0;
    for (size_t i = 1; i < ans.size(); i++)
        if (ans[i])
            selected++;
    return {total, selected};
}

int main(int argc, char **argv)
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    n = 2000;
    m = 43600;
    N = 200000;

    if (argc >= 2 && string(argv[1]) == "file")
    {
        cin >> n >> m;
        reset_globals();
        for (int i = 0; i < m; i++)
        {
            int u, v;
            cin >> u >> v;
            g[u].pb(v);
            g[v].pb(u);
            vis[u][v] = vis[v][u] = 1;
        }
        auto [total, selected] = run_trial();
        cout << "n=" << n << " m=" << m
             << " total_induced_C4=" << total
             << " selected=" << selected
             << " need=" << N
             << (selected >= N ? "  OK" : "  FAIL") << endl;
        return 0;
    }

    int T = (argc >= 2 ? atoi(argv[1]) : 1000);
    uint64_t seed = (argc >= 3 ? strtoull(argv[2], nullptr, 10) : 12345ULL);

    long long ok = 0;
    long long tot_sum = 0, sel_sum = 0, sel_sq = 0;
    long long sel_min = LLONG_MAX, sel_max = 0;

    for (int t = 0; t < T; t++)
    {
        reset_globals();
        gen_graph(n, m, seed + t);
        auto [total, selected] = run_trial();
        bool good = selected >= N;
        if (good)
            ok++;
        tot_sum += total;
        sel_sum += selected;
        sel_sq += selected * selected;
        sel_min = min(sel_min, selected);
        sel_max = max(sel_max, selected);

        if (!good)
            cout << "trial " << t << ": total_C4=" << total
                 << " selected=" << selected << "  FAIL" << endl;

        if ((t + 1) % 50 == 0 || t + 1 == T)
            cout << "progress " << (t + 1) << "/" << T
                 << ": success=" << ok
                 << "  last selected=" << selected
                 << "  min=" << sel_min << "  max=" << sel_max << endl;
    }

    double mean = sel_sum / (double)T;
    double var = (sel_sq / (double)T) - mean * mean;
    double sd = sqrt(max(0.0, var));

    cout << "==== summary ====" << endl;
    cout << "trials=" << T << " success=" << ok
         << " (" << (100.0 * ok / T) << "%)" << endl;
    cout << "avg total_C4=" << (tot_sum / (double)T) << endl;
    cout << "selected: mean=" << mean << "  sd=" << sd
         << "  min=" << sel_min << "  max=" << sel_max << endl;
    cout << "margin above threshold: mean-200000=" << (mean - N)
         << "  (= " << ((mean - N) / sd) << " sigma)" << endl;
    return 0;
}
