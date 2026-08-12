#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
 
// 使用 gp_hash_table 替代 unordered_map 和 unordered_set
template<typename K, typename V>
using hash_map = gp_hash_table<K, V>;
 
using hash_set = gp_hash_table<int, null_type>;
 
struct C4 {
    int v[4];
    bool operator<(const C4 &o) const {
        for (int i = 0; i < 4; i++)
            if (v[i] != o.v[i]) return v[i] < o.v[i];
        return false;
    }
};
 
int V, E;
vector<vector<int>> adj;
vector<hash_set> S;
 
void readGraph() {
    cin >> V >> E;
    adj.assign(V + 1, {});
    S.assign(V + 1, {});
    for (int i = 0; i < E; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        S[u].insert(v);
        S[v].insert(u);
    }
    for (int v = 1; v <= V; v++)
        sort(adj[v].begin(), adj[v].end());
}
 
// 快速枚举所有诱导 C4
vector<C4> enumerateC4() {
    vector<C4> out;
    hash_map<long long, vector<int>> cn;
 
    // 对每对邻居，记录它们的公共邻居
    for (int x = 1; x <= V; x++) {
        auto &nb = adj[x];
        for (size_t i = 0; i < nb.size(); i++) {
            for (size_t j = i + 1; j < nb.size(); j++) {
                int a = nb[i], c = nb[j];
                if (a > c) swap(a, c);
                cn[(long long)a * (V + 1) + c].push_back(x);
            }
        }
    }
 
    // 对每对非相邻顶点，检查公共邻居形成的 C4
    for (auto &kv : cn) {
        long long key = kv.first;
        int a = (int)(key / (V + 1)), c = (int)(key % (V + 1));
        if (S[a].find(c) != S[a].end()) continue;
 
        auto &mid = kv.second;
        int sz = (int)mid.size();
        for (int i = 0; i < sz; i++) {
            for (int j = i + 1; j < sz; j++) {
                int b = mid[i], d = mid[j];
                if (S[b].find(d) != S[b].end()) continue;
 
                int b2 = min(b, d), d2 = max(b, d);
                if (make_pair(a, c) > make_pair(b2, d2)) continue;
 
                C4 q;
                q.v[0] = a; q.v[1] = c; q.v[2] = b2; q.v[3] = d2;
                sort(q.v, q.v + 4);
                out.push_back(q);
            }
        }
    }
 
    return out;
}
 
// 计算三元组的哈希值
inline long long tripleKey(int a, int b, int c) {
    return ((long long)a * (V + 1) + b) * (V + 1) + c;
}
 
// 获取 C4 的 4 个三元组（使用数组避免 vector 分配）
inline void getTriples(const C4 &c, long long t[4]) {
    t[0] = tripleKey(c.v[0], c.v[1], c.v[2]);
    t[1] = tripleKey(c.v[0], c.v[1], c.v[3]);
    t[2] = tripleKey(c.v[0], c.v[2], c.v[3]);
    t[3] = tripleKey(c.v[1], c.v[2], c.v[3]);
}
 
// 贪心打包 + 简单增广
vector<C4> greedyPacking(const vector<C4> &all) {
    int N = all.size();
 
    // 建立三元组 → C4 索引的映射（使用 pbds）
    hash_map<long long, vector<int>> triple2c4;
 
    vector<array<long long, 4>> c4Triples(N);
 
    for (int i = 0; i < N; i++) {
        long long t[4];
        getTriples(all[i], t);
        for (int j = 0; j < 4; j++) {
            c4Triples[i][j] = t[j];
            triple2c4[t[j]].push_back(i);
        }
    }
 
    // 计算每个 C4 的冲突度
    vector<int> conflict(N, 0);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 4; j++) {
            conflict[i] += triple2c4[c4Triples[i][j]].size() - 1;
        }
    }
 
    // 按冲突度升序排序
    vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        if (conflict[a] != conflict[b]) return conflict[a] < conflict[b];
        return a < b;
    });
 
    // 贪心选择（使用 pbds）
    hash_map<long long, int> owner;
 
    vector<char> selected(N, 0);
 
    auto isFree = [&](int idx) {
        for (int j = 0; j < 4; j++) {
            if (owner.find(c4Triples[idx][j]) != owner.end())
                return false;
        }
        return true;
    };
 
    auto select = [&](int idx) {
        selected[idx] = 1;
        for (int j = 0; j < 4; j++) {
            owner[c4Triples[idx][j]] = idx;
        }
    };
 
    auto deselect = [&](int idx) {
        selected[idx] = 0;
        for (int j = 0; j < 4; j++) {
            owner.erase(c4Triples[idx][j]);
        }
    };
 
    // 初始贪心
    for (int idx : order) {
        if (isFree(idx)) {
            select(idx);
        }
    }
 
    // 增广：限制轮数为 3 轮（进一步减少）
    for (int round = 0; round < 3; round++) {
        bool changed = false;
 
        // 替换轮
        for (int idx : order) {
            if (selected[idx]) continue;
 
            int conflictWith = -1;
            bool multiConflict = false;
 
            for (int j = 0; j < 4; j++) {
                auto it = owner.find(c4Triples[idx][j]);
                if (it != owner.end()) {
                    if (conflictWith == -1) {
                        conflictWith = it->second;
                    } else if (conflictWith != it->second) {
                        multiConflict = true;
                        break;
                    }
                }
            }
 
            if (conflictWith != -1 && !multiConflict) {
                deselect(conflictWith);
                select(idx);
                changed = true;
            }
        }
 
        // 添加新的无冲突 C4
        for (int idx : order) {
            if (!selected[idx] && isFree(idx)) {
                select(idx);
                changed = true;
            }
        }
 
        if (!changed) break;
    }
 
    // 收集结果
    vector<C4> result;
    result.reserve(N / 2);
    for (int i = 0; i < N; i++) {
        if (selected[i]) {
            result.push_back(all[i]);
        }
    }
 
    return result;
}
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    string cmd;
    cin >> cmd;
 
    if (cmd == "send") {
        int n;
        cin >> n;
        readGraph();
 
        auto allC4 = enumerateC4();
        auto packed = greedyPacking(allC4);
 
        int output = min((int)packed.size(), n);
        for (int i = 0; i < output; i++) {
            cout << packed[i].v[0] << ' ' << packed[i].v[1] << ' '
                 << packed[i].v[2] << ' ' << packed[i].v[3] << '\n';
        }
 
        // 如果不够，重复最后一个
        for (int i = output; i < n; i++) {
            auto &c = packed[output > 0 ? output - 1 : 0];
            cout << c.v[0] << ' ' << c.v[1] << ' '
                 << c.v[2] << ' ' << c.v[3] << '\n';
        }
 
    } else { // receive
        readGraph();
 
        int m;
        cin >> m;
        vector<array<int, 3>> queries(m);
        for (int i = 0; i < m; i++) {
            cin >> queries[i][0] >> queries[i][1] >> queries[i][2];
        }
 
        auto allC4 = enumerateC4();
        auto packed = greedyPacking(allC4);
 
        // 建立三元组 → ID 的映射（使用 pbds）
        hash_map<long long, int> triple2id;
 
        for (int i = 0; i < (int)packed.size(); i++) {
            long long t[4];
            getTriples(packed[i], t);
            for (int j = 0; j < 4; j++) {
                triple2id[t[j]] = i + 1;
            }
        }
 
        // 回答查询
        for (int i = 0; i < m; i++) {
            int a = queries[i][0], b = queries[i][1], c = queries[i][2];
            int x = min({a, b, c});
            int z = max({a, b, c});
            int y = a + b + c - x - z;
 
            long long key = tripleKey(x, y, z);
            auto it = triple2id.find(key);
            cout << (it == triple2id.end() ? 1 : it->second) << '\n';
        }
    }
 
    return 0;
}
