#include <bits/stdc++.h>
using namespace std;

// Maintain p[value] in an implicit splay. Range reversal implements op 2;
// subtree summaries keep the at-most-two deepest selected vertices. The
// resulting O((n + q) log n) bound is amortized over the operation sequence.
struct Summary {
    int mn = 0, mx = 0;
    int cnt = 0;
    int leaf[2] = {};
    bool bad = false;
};

struct Node {
    int ch[2] = {}, fa = 0, sz = 1, val = 0;
    int mn = 0, mx = 0, cnt = 0;
    int leaf[2] = {};
    bool bad = false, rev = false;
};

int n, LOGN;
vector<Node> tr;
vector<int> whereNode, depthNode, tinNode, toutNode;
vector<vector<int>> up;
int rootNode, timerDfs;

bool ancestor(int u, int v) {
    return tinNode[u] <= tinNode[v] && toutNode[v] <= toutNode[u];
}

int lca(int u, int v) {
    if (ancestor(u, v)) return u;
    if (ancestor(v, u)) return v;
    for (int k = LOGN - 1; k >= 0; --k)
        if (!ancestor(up[k][u], v)) u = up[k][u];
    return up[0][u];
}

Summary getSummary(int x) {
    Summary s;
    if (!x) return s;
    s.mn = tr[x].mn;
    s.mx = tr[x].mx;
    s.cnt = tr[x].cnt;
    s.leaf[0] = tr[x].leaf[0];
    s.leaf[1] = tr[x].leaf[1];
    s.bad = tr[x].bad;
    return s;
}

Summary combine(const Summary &a, const Summary &b) {
    if (!a.cnt) return b;
    if (!b.cnt) return a;
    Summary c;
    c.mn = tinNode[a.mn] < tinNode[b.mn] ? a.mn : b.mn;
    c.mx = tinNode[a.mx] > tinNode[b.mx] ? a.mx : b.mx;
    if (a.bad || b.bad) {
        c.bad = true;
        c.cnt = 3;
        return c;
    }
    int cand[4], z = 0;
    for (int i = 0; i < a.cnt; ++i) cand[z++] = a.leaf[i];
    for (int i = 0; i < b.cnt; ++i) cand[z++] = b.leaf[i];
    for (int i = 0; i < z; ++i) {
        bool hasDescendant = false;
        for (int j = 0; j < z; ++j)
            if (i != j && ancestor(cand[i], cand[j])) {
                hasDescendant = true;
                break;
            }
        if (!hasDescendant) {
            bool duplicate = false;
            for (int j = 0; j < c.cnt; ++j)
                if (c.leaf[j] == cand[i]) duplicate = true;
            if (!duplicate) {
                if (c.cnt < 2) c.leaf[c.cnt] = cand[i];
                ++c.cnt;
            }
        }
    }
    if (c.cnt > 2) c.bad = true, c.cnt = 3;
    return c;
}

void pull(int x) {
    Node &u = tr[x];
    u.sz = 1;
    Summary s;
    if (u.ch[0]) u.sz += tr[u.ch[0]].sz, s = combine(s, getSummary(u.ch[0]));
    Summary self;
    self.mn = self.mx = u.val;
    self.cnt = 1;
    self.leaf[0] = u.val;
    s = combine(s, self);
    if (u.ch[1]) u.sz += tr[u.ch[1]].sz, s = combine(s, getSummary(u.ch[1]));
    u.mn = s.mn;
    u.mx = s.mx;
    u.cnt = s.cnt;
    u.leaf[0] = s.leaf[0];
    u.leaf[1] = s.leaf[1];
    u.bad = s.bad;
}

void applyReverse(int x) {
    if (!x) return;
    swap(tr[x].ch[0], tr[x].ch[1]);
    tr[x].rev ^= 1;
}

void push(int x) {
    if (tr[x].rev) {
        applyReverse(tr[x].ch[0]);
        applyReverse(tr[x].ch[1]);
        tr[x].rev = false;
    }
}

void rotateNode(int x) {
    int p = tr[x].fa, g = tr[p].fa;
    int d = tr[p].ch[1] == x;
    int b = tr[x].ch[d ^ 1];
    if (g) tr[g].ch[tr[g].ch[1] == p] = x;
    tr[x].fa = g;
    tr[x].ch[d ^ 1] = p;
    tr[p].fa = x;
    tr[p].ch[d] = b;
    if (b) tr[b].fa = p;
    pull(p);
    pull(x);
}

void splay(int x, int goal = 0) {
    vector<int> stk;
    for (int y = x; y != goal; y = tr[y].fa) stk.push_back(y);
    if (goal) stk.push_back(goal);
    for (int i = (int)stk.size() - 1; i >= 0; --i) push(stk[i]);
    while (tr[x].fa != goal) {
        int p = tr[x].fa, g = tr[p].fa;
        if (g != goal) {
            bool zigzig = (tr[p].ch[1] == x) == (tr[g].ch[1] == p);
            rotateNode(zigzig ? p : x);
        }
        rotateNode(x);
    }
    if (!goal) rootNode = x;
}

int kth(int k) {
    int x = rootNode;
    while (true) {
        push(x);
        int leftSize = tr[x].ch[0] ? tr[tr[x].ch[0]].sz : 0;
        if (k == leftSize + 1) return x;
        if (k <= leftSize) x = tr[x].ch[0];
        else k -= leftSize + 1, x = tr[x].ch[1];
    }
}

pair<int, int> split(int rt, int k) {
    if (!k) return {0, rt};
    if (k == tr[rt].sz) return {rt, 0};
    rootNode = rt;
    int x = kth(k);
    splay(x);
    int b = tr[x].ch[1];
    tr[x].ch[1] = 0;
    tr[b].fa = 0;
    pull(x);
    return {x, b};
}

int join(int a, int b) {
    if (!a) return b;
    if (!b) return a;
    rootNode = a;
    int x = kth(tr[a].sz);
    splay(x);
    tr[x].ch[1] = b;
    tr[b].fa = x;
    pull(x);
    return x;
}

int build(int l, int r, const vector<int> &seq, int parent = 0) {
    if (l > r) return 0;
    int m = (l + r) / 2, x = m;
    tr[x].val = seq[m];
    whereNode[seq[m]] = x;
    tr[x].fa = parent;
    tr[x].ch[0] = build(l, m - 1, seq, x);
    tr[x].ch[1] = build(m + 1, r, seq, x);
    pull(x);
    return x;
}

bool query(int l, int r) {
    auto [a, bc] = split(rootNode, l - 1);
    auto [b, c] = split(bc, r - l + 1);
    const Node &s = tr[b];
    bool ok = !s.bad && s.cnt > 0;
    if (ok) {
        int f = lca(s.mn, s.mx);
        long long length;
        if (s.cnt == 1) {
            int x = s.leaf[0];
            length = depthNode[x] - depthNode[f] + 1LL;
        } else {
            int x = s.leaf[0], y = s.leaf[1], bxy = lca(x, y);
            length = depthNode[x] + depthNode[y] - depthNode[bxy] - depthNode[f] + 1LL;
        }
        ok = length == r - l + 1LL;
    }
    rootNode = join(a, join(b, c));
    return ok;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        int q;
        cin >> n >> q;
        vector<vector<int>> g(n + 1);
        for (int i = 1, u, v; i < n; ++i) {
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        LOGN = 1;
        while ((1 << LOGN) <= n) ++LOGN;
        up.assign(LOGN, vector<int>(n + 1));
        depthNode.assign(n + 1, 0);
        tinNode.assign(n + 1, 0);
        toutNode.assign(n + 1, 0);
        vector<int> parent(n + 1), it(n + 1), order;
        order.reserve(n);
        vector<int> st{1};
        parent[1] = 1;
        while (!st.empty()) {
            int u = st.back();
            if (!it[u]) {
                tinNode[u] = ++timerDfs;
                up[0][u] = parent[u];
                for (int k = 1; k < LOGN; ++k) up[k][u] = up[k - 1][up[k - 1][u]];
                order.push_back(u);
            }
            if (it[u] < (int)g[u].size()) {
                int v = g[u][it[u]++];
                if (v == parent[u]) continue;
                parent[v] = u;
                depthNode[v] = depthNode[u] + 1;
                st.push_back(v);
            } else {
                toutNode[u] = timerDfs;
                st.pop_back();
            }
        }
        timerDfs = 0;

        vector<int> valueAtVertex(n + 1);
        for (int u = 1; u <= n; ++u) cin >> valueAtVertex[u];
        vector<int> seq(n + 1);
        for (int u = 1; u <= n; ++u) seq[valueAtVertex[u]] = u;
        tr.assign(n + 1, Node{});
        whereNode.assign(n + 1, 0);
        rootNode = build(1, n, seq);

        while (q--) {
            int op, x, y;
            cin >> op >> x >> y;
            if (op == 1) {
                int a = whereNode[x], b = whereNode[y];
                if (a != b) {
                    splay(a);
                    splay(b);
                    swap(tr[a].val, tr[b].val);
                    whereNode[x] = b;
                    whereNode[y] = a;
                    splay(a);
                    pull(a);
                    splay(b);
                    pull(b);
                }
            } else if (op == 2) {
                auto [a, bc] = split(rootNode, x - 1);
                auto [b, c] = split(bc, y - x + 1);
                applyReverse(b);
                rootNode = join(a, join(b, c));
            } else {
                cout << (query(x, y) ? "YES\n" : "NO\n");
            }
        }
    }
    return 0;
}
