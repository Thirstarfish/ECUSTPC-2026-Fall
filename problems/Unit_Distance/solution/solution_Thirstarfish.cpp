#include <bits/stdc++.h>
using namespace std;

void cold_beans()
{
    int n;
    cin >> n;
    for (int m = 0; m < n; m++)
    {
        double x = 0, y = 0;
        for (int i = 12; i >= 0; i--)
            if (m & 1 << i)
            {
                double dy = 1.0 / (1 << i);
                double dx = sqrt(1 - dy * dy);
                x += dx, y += dy;
            }
        cout << x << ' ' << y << '\n';
    }
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout << fixed << setprecision(10);
    int _;
    cin >> _;
    while (_--)
        cold_beans();
}