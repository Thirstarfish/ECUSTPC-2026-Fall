#pragma GCC optimize("O3,unroll-loops") // 开启 O3 优化，对 bitset 移位操作有显著提升
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt") // 针对现代 CPU 指令集优化

#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

// 最大可能的总和为 5000 * 5000 = 25,000,000
// 目标值（target）最大为 12,500,000，稍微开大一点防止越界
const int MAX_TARGET = 12500005;

// 声明为全局变量，避免在栈上分配 1.5MB 导致栈溢出
bitset<MAX_TARGET> dp;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    long long total_sum = 0;
    
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        total_sum += a[i];
    }

    // 如果总和是奇数，直接返回 NO
    if (total_sum % 2 != 0) {
        cout << "NO\n";
        return;
    }

    int target = total_sum / 2;

    // 清空上一次测试用例残留的状态
    dp.reset();
    dp[0] = 1; // 初始状态：和为 0 始终可行

    // 核心动态规划逻辑
    for (int i = 0; i < n; ++i) {
        // bitset 的左移操作会自动处理底层的所有位运算
        dp |= (dp << a[i]); 
    }

    // 检查第 target 位是否为 1
    if (dp[target]) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

int main() {
    // 提升输入输出速度
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