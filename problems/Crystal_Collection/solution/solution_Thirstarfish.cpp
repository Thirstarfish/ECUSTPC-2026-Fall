#include <bits/stdc++.h>
using namespace std;

void cold_beans()
{
    int n;
    cin >> n;
    int mn = INT_MAX, cnt = 0;
    long long sum = 0;
    for (int i = 1; i <= n; i++)
    {
        int a;
        cin >> a;
        if (a < 0)
            cnt++;
        sum += abs(a);
        mn = min(mn, abs(a));
    }
    if (cnt % 2 == n % 2)
        sum -= mn * 2;
    cout << sum << '\n';
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