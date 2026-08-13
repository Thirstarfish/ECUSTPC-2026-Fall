#include <bits/stdc++.h>
using namespace std;

void cold_beans()
{
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &b : a)
        cin >> b;
    mt19937 g(114514);
    shuffle(a.begin(), a.end(), g);
    bitset<2000000> dp;
    dp[1000000] = 1;
    for (auto b : a)
        dp = (dp << b) | (dp >> b);
    cout << (dp[1000000] ? "YES" : "NO") << endl;
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