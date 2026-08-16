#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <cstring>

using namespace std;

const long long MOD = 998244353;
const int MAX_STATES = 105;

// 用于矩阵快速幂的类封装
struct Matrix {
    long long mat[MAX_STATES][MAX_STATES];
    int size;
    Matrix(int n) {
        size = n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                mat[i][j] = 0;
            }
        }
    }
    Matrix operator*(const Matrix& other) const {
        Matrix res(size);
        for(int i = 0; i < size; ++i) {
            for(int k = 0; k < size; ++k) {
                if(mat[i][k] == 0) continue;
                for(int j = 0; j < size; ++j) {
                    res.mat[i][j] = (res.mat[i][j] + mat[i][k] * other.mat[k][j]) % MOD;
                }
            }
        }
        return res;
    }
};

int ch[MAX_STATES][26];
int fail[MAX_STATES];
bool bad[MAX_STATES];
int node_count;

// 初始化 AC 自动机节点
void init() {
    memset(ch, 0, sizeof(ch));
    memset(fail, 0, sizeof(fail));
    memset(bad, 0, sizeof(bad));
    node_count = 1;
}

// 插入禁止子串
void insert(const string& s) {
    int u = 0;
    for (char c : s) {
        int x = c - 'a';
        if (!ch[u][x]) ch[u][x] = node_count++;
        u = ch[u][x];
    }
    bad[u] = true; // 单词结尾即视为非法
}

// 建立 Fail 指针
void build_fail() {
    queue<int> q;
    for (int i = 0; i < 26; ++i) {
        if (ch[0][i]) {
            fail[ch[0][i]] = 0;
            q.push(ch[0][i]);
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        bad[u] |= bad[fail[u]]; // 继承 Fail 指针的非法状态 (包含非法后缀的串整体即为非法)
        for (int i = 0; i < 26; ++i) {
            if (ch[u][i]) {
                fail[ch[u][i]] = ch[fail[u]][i];
                q.push(ch[u][i]);
            } else {
                ch[u][i] = ch[fail[u]][i]; // 建立 Trie 图
            }
        }
    }
}

// 向量乘法替代矩阵快速幂 (提速)
// 【修改1】：由于 L <= 1e18，二进制拆分的上限需要从 32 提升到 62 (2^61 > 1e18)
void multiply_vec(vector<long long>& vec, long long d, const vector<Matrix>& M_pow, int V) {
    for(int j = 0; j < 62; ++j) { 
        if((d >> j) & 1) {
            vector<long long> next_vec(V, 0);
            for(int i = 0; i < V; ++i) {
                if(vec[i] == 0) continue;
                for(int k = 0; k < V; ++k) {
                    next_vec[k] = (next_vec[k] + vec[i] * M_pow[j].mat[i][k]) % MOD;
                }
            }
            vec = next_vec;
        }
    }
}

void solve() {
    long long L;
    while (cin >> L) {
        int K, N;
        cin >> K >> N;

        vector<pair<long long, char>> constraints(K);
        for(int i = 0; i < K; ++i) {
            cin >> constraints[i].first >> constraints[i].second;
        }
        sort(constraints.begin(), constraints.end()); // 按照限制的位置 p_i 从小到大排序

        init();

        for(int i = 0; i < N; ++i) {
            string s;
            cin >> s;
            insert(s);
        }

        build_fail();

        // 挑选出所有非法的状态进行隔离去重
        int map_to_valid[MAX_STATES];
        vector<int> valid_states;
        int V = 0;
        memset(map_to_valid, -1, sizeof(map_to_valid));

        for(int i = 0; i < node_count; ++i) {
            if(!bad[i]) {
                map_to_valid[i] = V++;
                valid_states.push_back(i);
            }
        }

        if (V == 0) {
            cout << 0 << "\n";
            continue;
        }

        // 构建无约束转移图的邻接矩阵 M
        Matrix M(V);
        for(int j = 0; j < V; ++j) {
            int u = valid_states[j];
            for(int c = 0; c < 26; ++c) {
                int v = ch[u][c];
                if(!bad[v]) {
                    M.mat[j][map_to_valid[v]]++;
                }
            }
        }

        // 提前预计算无约度矩阵 M 的 2^j 乘方，节省每次 K 操作时重复的快速幂时间
        // 【修改2】：倍增矩阵的预计算也要开到 62 位
        vector<Matrix> M_pow(62, Matrix(V));
        M_pow[0] = M;
        for(int j = 1; j < 62; ++j) {
            M_pow[j] = M_pow[j-1] * M_pow[j-1];
        }

        long long curr_p = 0;
        vector<long long> vec(V, 0); // 状态数组
        if (map_to_valid[0] != -1) {
            vec[map_to_valid[0]] = 1;
        }

        // 顺序对分段的独立部分应用约束状态转移
        // K=500 也会按照 O(K * V^2 * log L) 的极小常数平滑度过，不用做任何修改
        for(int i = 0; i < K; ++i) {
            long long p = constraints[i].first;
            char c = constraints[i].second;
            long long d = p - curr_p - 1;
            
            if (d > 0) { // 第一个是无约束跳跃段
                multiply_vec(vec, d, M_pow, V);
            }
            
            vector<long long> next_vec(V, 0);
            for(int j = 0; j < V; ++j) {
                if(vec[j] == 0) continue;
                int u = valid_states[j];
                int v = ch[u][c - 'a']; // 单行强制转移
                if(!bad[v]) {
                    next_vec[map_to_valid[v]] = (next_vec[map_to_valid[v]] + vec[j]) % MOD;
                }
            }
            vec = next_vec;
            curr_p = p;
        }

        // 处理全部预置约束结束后残余的字符串尾迹
        long long d = L - curr_p;
        if (d > 0) {
            multiply_vec(vec, d, M_pow, V);
        }

        long long ans = 0;
        for(int i = 0; i < V; ++i) {
            ans = (ans + vec[i]) % MOD;
        }
        cout << ans << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}