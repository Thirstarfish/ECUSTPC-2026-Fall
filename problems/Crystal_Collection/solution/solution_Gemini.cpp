#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

void solve() {
    int n;
    cin >> n;
    
    long long sum_abs = 0;
    long long min_abs = 2e9; // 初始为一个比可能的最大绝对值 10^9 还要大的数
    int count_neg = 0;
    bool has_zero = false;
    
    for (int i = 0; i < n; ++i) {
        long long x;
        cin >> x;
        
        sum_abs += abs(x);
        min_abs = min(min_abs, abs(x));
        
        if (x < 0) {
            count_neg++;
        } else if (x == 0) {
            has_zero = true;
        }
    }
    
    // 如果存在 0 或者负数的奇偶性恰好满足条件，即可全部转为绝对值
    if (has_zero || (count_neg % 2 == (n - 1) % 2)) {
        cout << sum_abs << "\n";
    } else {
        // 否则必须牺牲绝对值最小的一个数字
        cout << sum_abs - 2 * min_abs << "\n";
    }
}

int main() {
    // 优化输入输出流速度
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