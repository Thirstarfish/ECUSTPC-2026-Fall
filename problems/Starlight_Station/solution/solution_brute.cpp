#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n, q;
        cin >> n >> q;
        vector<vector<int>> g(n + 1);
        for (int i = 1, u, v; i < n; ++i) {
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }

        int lg = 1;
        while ((1 << lg) <= n) ++lg;
        vector<int> parent(n + 1), depth(n + 1), tin(n + 1), tout(n + 1), it(n + 1);
        vector<vector<int>> up(lg, vector<int>(n + 1));
        int timer = 0;
        vector<int> st{1};
        parent[1] = 1;
        while (!st.empty()) {
            int u = st.back();
            if (it[u] == 0) {
                tin[u] = ++timer;
                up[0][u] = parent[u];
                for (int k = 1; k < lg; ++k) up[k][u] = up[k - 1][up[k - 1][u]];
            }
            if (it[u] < (int)g[u].size()) {
                int v = g[u][it[u]++];
                if (v == parent[u]) continue;
                parent[v] = u;
                depth[v] = depth[u] + 1;
                st.push_back(v);
            } else {
                tout[u] = timer;
                st.pop_back();
            }
        }

        auto isAncestor = [&](int u, int v) {
            return tin[u] <= tin[v] && tout[v] <= tout[u];
        };
        auto lca = [&](int u, int v) {
            if (isAncestor(u, v)) return u;
            if (isAncestor(v, u)) return v;
            for (int k = lg - 1; k >= 0; --k)
                if (!isAncestor(up[k][u], v)) u = up[k][u];
            return parent[u];
        };

        // p[value] is the tree vertex currently carrying that value.
        // Range reversal and query inspection are intentionally linear.
        vector<int> p(n + 1), value(n + 1);
        for (int u = 1; u <= n; ++u) {
            cin >> value[u];
            p[value[u]] = u;
        }

        while (q--) {
            int op, x, y;
            cin >> op >> x >> y;
            if (op == 1) {
                int vx = value[x], vy = value[y];
                swap(value[x], value[y]);
                swap(p[vx], p[vy]);
            } else if (op == 2) {
                reverse(p.begin() + x, p.begin() + y + 1);
                for (int k = x; k <= y; ++k) value[p[k]] = k;
            } else {
                vector<int> nodes;
                nodes.reserve(y - x + 1);
                for (int k = x; k <= y; ++k) nodes.push_back(p[k]);
                sort(nodes.begin(), nodes.end(), [&](int a, int b) {
                    return tin[a] < tin[b];
                });

                int f = lca(nodes.front(), nodes.back());
                vector<int> leaves;
                for (int i = 0; i < (int)nodes.size(); ++i) {
                    if (i + 1 == (int)nodes.size() || tin[nodes[i + 1]] > tout[nodes[i]])
                        leaves.push_back(nodes[i]);
                }

                bool ok = leaves.size() <= 2;
                long long covered = 0;
                if (ok && leaves.size() == 1) {
                    int a = leaves[0];
                    covered = depth[a] - depth[f] + 1LL;
                } else if (ok) {
                    int a = leaves[0], b = leaves[1], c = lca(a, b);
                    covered = depth[a] + depth[b] - depth[c] - depth[f] + 1LL;
                }
                ok = ok && covered == y - x + 1LL;
                cout << (ok ? "YES\n" : "NO\n");
            }
        }
    }
    return 0;
}
