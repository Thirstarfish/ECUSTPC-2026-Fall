#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const long long INF = 2000000000000000000LL;

// 严格向下取整的除法（处理负数时和 C++ 默认的向零取整不同）
long long floor_div(long long a, long long b) {
    long long res = a / b;
    long long rem = a % b;
    if (rem != 0 && ((a < 0) ^ (b < 0))) {
        res--;
    }
    return res;
}

// 严格向上取整的除法
long long ceil_div(long long a, long long b) {
    long long res = a / b;
    long long rem = a % b;
    if (rem != 0 && !((a < 0) ^ (b < 0))) {
        res++;
    }
    return res;
}

// 完全整型的平方根推导，利用 round + 微调避开所有精度误差
long long my_sqrt(long long D) {
    if (D < 0) return 0;
    long long root = round(sqrt(D));
    while (root * root > D) root--;
    while ((root + 1) * (root + 1) <= D) root++;
    return root;
}

struct Banned {
    long long l, r;
};

void solve() {
    int n;
    cin >> n;
    
    long long L_global = -INF;
    long long R_global = INF;
    vector<Banned> banned_intervals;
    bool impossible = false;

    for (int i = 0; i < n; i++) {
        long long a, b, c;
        cin >> a >> b >> c;
        if (impossible) continue;

        if (a < 0) {
            // 将 a < 0 的形式翻转为 A*x^2 + B*x + C < 0 (A > 0)
            long long A = -a, B = -b, C = -c;
            long long D = B * B - 4 * A * C;
            if (D <= 0) {
                impossible = true; // 无法小于0，不可能有解
            } else {
                long long S_floor = my_sqrt(D);
                bool is_perfect = (S_floor * S_floor == D);
                long long S_ceil = is_perfect ? S_floor : S_floor + 1;
                long long P = -B;
                long long Q = 2 * A;

                // 左边界：求 (P - sqrt(D)) / Q 的向上取整值，即大于它的最小整数
                long long floor_sub = floor_div(P - S_ceil, Q);
                long long L = floor_sub + 1; 

                // 右边界：求 (P + sqrt(D)) / Q 的向下取整值，即小于它的最大整数
                long long ceil_add;
                if (is_perfect) ceil_add = ceil_div(P + S_floor, Q);
                else ceil_add = floor_div(P + S_floor, Q) + 1;
                long long R = ceil_add - 1;

                L_global = max(L_global, L);
                R_global = min(R_global, R);
            }
        } else {
            // a > 0 时，无效区间为 A*x^2 + B*x + C <= 0
            long long A = a, B = b, C = c;
            long long D = B * B - 4 * A * C;
            if (D == 0) {
                if (B % (2 * A) == 0) {
                    long long x = -B / (2 * A);
                    banned_intervals.push_back({x, x}); // 仅仅只是顶点恰好等于0时不符
                }
            } else if (D > 0) {
                long long S_floor = my_sqrt(D);
                bool is_perfect = (S_floor * S_floor == D);
                long long S_ceil = is_perfect ? S_floor : S_floor + 1;
                long long P = -B;
                long long Q = 2 * A;

                // 左侧禁止边界（大于等于根的最小整数）
                long long ceil_sub;
                if (is_perfect) ceil_sub = ceil_div(P - S_floor, Q);
                else ceil_sub = floor_div(P - S_ceil, Q) + 1;
                long long L = ceil_sub;

                // 右侧禁止边界（小于等于根的最大整数）
                long long floor_add = floor_div(P + S_floor, Q);
                long long R = floor_add;

                if (L <= R) {
                    banned_intervals.push_back({L, R});
                }
            }
        }
    }

    // 1. 若 a<0 的不等式已经自身相互冲突
    if (impossible || L_global > R_global) {
        cout << "NO\n";
        return;
    }

    // 2. 若全都是 a>0 的条件。有限个禁止区间永远无法覆盖无限大的有效域，必存在解
    if (L_global == -INF) {
        cout << "YES\n";
        return;
    }

    // 3. 将对全局候选有效区域产生影响的禁止区间剥离重组
    vector<pair<long long, long long>> intervals;
    for (auto& b : banned_intervals) {
        long long l = max(b.l, L_global);
        long long r = min(b.r, R_global);
        if (l <= r) {
            intervals.push_back({l, r});
        }
    }

    // 将需要筛除的区间按左端点排序进行合并与验证
    sort(intervals.begin(), intervals.end());

    long long current_L = L_global;
    for (auto& iv : intervals) {
        if (iv.first > current_L) {
            cout << "YES\n"; // 找到了漏网的合法数字！
            return;
        }
        current_L = max(current_L, iv.second + 1);
    }

    if (current_L <= R_global) {
        cout << "YES\n"; // 遍历完禁止区间之后，右侧还剩合法区域
    } else {
        cout << "NO\n";  // 全程被无缝盖死
    }
}

int main() {
    // 基础防超时处理
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}