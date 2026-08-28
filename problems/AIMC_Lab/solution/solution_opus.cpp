#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Dinic {
    struct Edge { int to, rev; ll cap; };
    int n;
    vector<vector<Edge>> g;
    vector<int> level, iter;

    void init(int n_) { n = n_; g.assign(n, {}); }

    void add_edge(int u, int v, ll cap) {
        g[u].push_back({v, (int)g[v].size(), cap});
        g[v].push_back({u, (int)g[u].size() - 1, 0});
    }

    bool bfs(int s, int t) {
        level.assign(n, -1);
        queue<int> q;
        level[s] = 0; q.push(s);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (auto& e : g[v])
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
        }
        return level[t] >= 0;
    }

    ll dfs(int v, int t, ll up) {
        if (v == t) return up;
        for (int& i = iter[v]; i < (int)g[v].size(); ++i) {
            auto& e = g[v][i];
            if (e.cap > 0 && level[e.to] == level[v] + 1) {
                ll d = dfs(e.to, t, min(up, e.cap));
                if (d > 0) { e.cap -= d; g[e.to][e.rev].cap += d; return d; }
            }
        }
        return 0;
    }

    ll max_flow(int s, int t) {
        ll flow = 0, d;
        while (bfs(s, t)) {
            iter.assign(n, 0);
            while ((d = dfs(s, t, (ll)4e18)) > 0) flow += d;
        }
        return flow;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, H;
    cin >> N >> M >> H;
    int V = N * M * H;

    vector<vector<ll>> A(8, vector<ll>(V));
    ll baseline = 0;
    for (int t = 0; t < 8; ++t)
        for (int i = 0; i < V; ++i) { cin >> A[t][i]; baseline += A[t][i]; }

    // Node layout: [0, V) = cells, [V, 7V) = aux nodes (6 per cell), 7V = S, 7V+1 = T
    int S = 7 * V, T = S + 1;
    Dinic din; din.init(T + 1);

    auto cid = [&](int x, int y, int z) { return (x * M + y) * H + z; };
    // aux(u, k, s): auxiliary node for cell u, cluster k in {0,1,2}, side s in {0=z0, 1=z1}
    auto aux = [&](int u, int k, int s) { return V + 6 * u + 2 * k + s; };

    const ll INF = (ll)4e18;

    for (int x = 0; x < N; ++x)
    for (int y = 0; y < M; ++y)
    for (int z = 0; z < H; ++z) {
        int u = cid(x, y, z);
        int p = (x + y + z) & 1;   // parity: determines which A-index maps to z=0 vs z=1

        // Unary edges: A[p] = "z=0 reward", A[p^1] = "z=1 reward"
        din.add_edge(S, u,    A[p    ][u]);
        din.add_edge(u, T,    A[p ^ 1][u]);

        // Cluster edges for k=0,1,2 (S1/S2/S3); A indices: 2+2k and 2+2k+1
        for (int k = 0; k < 3; ++k) {
            int b = 2 + 2 * k;
            din.add_edge(S,         aux(u, k, 0), A[b ^ p      ][u]);
            din.add_edge(aux(u,k,1), T,           A[b ^ p ^ 1  ][u]);
            din.add_edge(aux(u, k, 0), u, INF);
            din.add_edge(u, aux(u, k, 1), INF);
        }

        // Neighbour edges: enumerate all 26 neighbours, classify by squared distance
        for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy)
        for (int dz = -1; dz <= 1; ++dz) {
            int d2 = dx*dx + dy*dy + dz*dz;
            if (d2 < 1 || d2 > 3) continue;
            int nx = x+dx, ny = y+dy, nz = z+dz;
            if (nx < 0 || nx >= N || ny < 0 || ny >= M || nz < 0 || nz >= H) continue;
            int v = cid(nx, ny, nz), k = d2 - 1;
            din.add_edge(aux(u, k, 0), v, INF);
            din.add_edge(v, aux(u, k, 1), INF);
        }
    }

    cout << baseline - din.max_flow(S, T) << "\n";
}
