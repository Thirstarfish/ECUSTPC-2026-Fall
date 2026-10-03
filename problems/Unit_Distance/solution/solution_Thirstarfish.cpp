#include <bits/stdc++.h>
using namespace std;

void solve()
{
    int n;
    cin >> n;
    for (int m = 0; m < n; m++)
    {
        long double x = 0, y = 0, dy;
        for (int i = 12; i >= 0; i--)
            if (m & 1 << i)
                dy = 1.0l / (1 << i), y += dy, x += sqrt(1.0l - dy * dy);
        cout << (long long)(x * 1e13) << ' ' << (long long)(y * 1e13) << '\n';
    }
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    int _;
    cin >> _;
    while (_--)
        solve();
}