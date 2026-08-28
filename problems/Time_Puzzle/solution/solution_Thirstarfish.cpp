#include <bits/stdc++.h>
using namespace std;

void cold_beans()
{
    int a, b;
    cin >> a >> b;
    a += b / 60;
    b %= 60;
    a %= 24;
    cout << a << ' ' << b << '\n';
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