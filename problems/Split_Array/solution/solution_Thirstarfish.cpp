#include <bits/stdc++.h>
using namespace std;

void cold_beans()
{
    int n, m;
    cin >> n >> m;
    vector<int> a(n + 1), b(n + 1);
    for (int i = 1; i <= n; i++)
        cin >> a[i];
    for (int i = 1; i <= n; i++)
        cin >> b[i];

    auto check = [&](int mid) -> bool
    {
        vector<int> pre(n + 1), suf(n + 3);

        if (a[1] >= mid)
            pre[1]++;
        for (int i = 2; i <= n; i++)
            if (a[i] >= mid)
                pre[i] = pre[i - 2] + 1;
            else
                pre[i] = pre[i - 1];

        if (b[n] >= mid)
            suf[n]++;
        for (int i = n - 1; i >= 1; i--)
            if (b[i] >= mid)
                suf[i] = suf[i + 2] + 1;
            else
                suf[i] = suf[i + 1];
                
        int mx = suf[1];
        for (int i = 0; i <= n; i++)
            mx = max(mx, pre[i] + suf[i + 2]);
        if (mx >= m)
            return 1;
        else
            return 0;
    };

    int l = -1e9, r = 1e9, ans = -2e9;
    while (l <= r)
    {
        int mid = l + r >> 1;
        if (check(mid))
        {
            ans = mid;
            l = mid + 1;
        }
        else
            r = mid - 1;
    }
    cout << ans << '\n';
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