#include <bits/stdc++.h>
using namespace std;

void cold_beans()
{
    cout << 1 << endl;
    cout << 5000 << endl;
    for (int i = 1; i <= 5000; i++)
        cout << i << " \n"[i == 5000];
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    int _ = 1;
    // cin >> _;
    while (_--)
        cold_beans();
}