#include <bits/stdc++.h>
using namespace std;
#define int long long
typedef long double ld;
const ld eps = 1e-8;

void cold_beans()
{
    int n;
    cin >> n;
    vector<int> a(n + 1), b(n + 1), c(n + 1);
    for (int i = 1; i <= n; i++)
        cin >> a[i] >> b[i] >> c[i];
    ld l1 = -1e18, l2 = 1e18;
    vector<array<int, 2>> bl;
    for (int i = 1; i <= n; i++)
    {
        int delta = b[i] * b[i] - a[i] * c[i] * 4;
        if (delta < 0)
            if (a[i] > 0)
                continue;
            else
            {
                cout << "NO" << endl;
                return;
            }
        ld x1 = 1.0l * (-b[i] - sqrt(delta)) / a[i] / 2;
        ld x2 = 1.0l * (-b[i] + sqrt(delta)) / a[i] / 2;
        if (a[i] > 0)
        {
            int l = ceil(x1 - eps), r = floor(x2 + eps);
            bl.push_back({l, r});
        }
        else
        {
            l1 = max(x2, l1);
            l2 = min(x1, l2);
        }
    }
    ranges::sort(bl);
    int a1 = ceil(l1 + eps), a2 = floor(l2 - eps);
    int cur = a1;
    for (auto [l, r] : bl)
    {
        if (l > cur)
            break;
        cur = max(cur, r + 1);
    }
    if (cur <= a2)
        cout << "YES" << endl;
    else
        cout << "NO" << endl;
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    int _;
    cin >> _;
    while (_--)
        cold_beans();
}