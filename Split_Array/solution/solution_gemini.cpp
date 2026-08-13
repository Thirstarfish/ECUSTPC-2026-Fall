#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

const int INF = 1e8; // 用一个足够大但不至于溢出的负数代替负无穷

bool check(long long x, int n, int m, const vector<long long> &a, const vector<long long> &b)
{
    // dpA[i][0/1] 记录前缀 a[0...i-1]
    vector<vector<int>> dpA(n + 1, vector<int>(2, -INF));
    dpA[0][0] = 0;

    for (int i = 1; i <= n; i++)
    {
        dpA[i][0] = max(dpA[i - 1][0], dpA[i - 1][1]);
        if (a[i - 1] >= x)
        {
            dpA[i][1] = dpA[i - 1][0] + 1;
        }
        else
        {
            dpA[i][1] = -INF;
        }
    }

    // dpB[i][0/1] 记录后缀 b[i-1...n-1]
    vector<vector<int>> dpB(n + 2, vector<int>(2, -INF));
    dpB[n + 1][0] = 0;

    for (int i = n; i >= 1; i--)
    {
        dpB[i][0] = max(dpB[i + 1][0], dpB[i + 1][1]);
        if (b[i - 1] >= x)
        {
            dpB[i][1] = dpB[i + 1][0] + 1;
        }
        else
        {
            dpB[i][1] = -INF;
        }
    }

    // 枚举分割点 k (a贡献前 k 个，b贡献后 n-k 个)
    for (int k = 0; k <= n; k++)
    {
        // 方案1：不选分割点左侧的元素 (a的第k个)
        int op1 = dpA[k][0] + max(dpB[k + 1][0], dpB[k + 1][1]);
        // 方案2：选了分割点左侧的元素，则强制不能选右侧第一个元素 (b的第k+1个)
        int op2 = dpA[k][1] + dpB[k + 1][0];

        if (max(op1, op2) >= m)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T;
    cin >> T;
    while (T--)
    {
        int n, m;
        if (!(cin >> n >> m))
            return 0;

        // 根据抽屉原理，长度为n的数组最多选出 (n+1)/2 个不相邻元素
        if (m > (n + 1) / 2)
        {
            cout << "Impossible" << "\n";
            return 0;
        }

        vector<long long> a(n), b(n);
        for (int i = 0; i < n; i++)
            cin >> a[i];
        for (int i = 0; i < n; i++)
            cin >> b[i];

        // 二分答案的范围：最小值到最大值
        long long left = -1e9, right = 1e9;
        long long ans = -1e9;

        while (left <= right)
        {
            // 处理负数时向负无穷方向取整的安全写法
            long long mid = left + (right - left) / 2;
            if (mid < 0 && (right - left) % 2 != 0)
            {
                mid--;
            }
            // 如果标准的 left + (right - left) / 2 你更习惯，可以直接写，但在存在负数情况下，
            // 对于二分答案最好能确保严格向下取整。直接用 mid = left + (right - left) / 2 也可以运作。

            if (check(mid, n, m, a, b))
            {
                ans = mid;
                left = mid + 1; // 尝试寻找更大的第 m 大值
            }
            else
            {
                right = mid - 1;
            }
        }

        cout << ans << "\n";
    }
    return 0;
}