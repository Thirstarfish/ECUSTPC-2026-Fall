#include <bits/stdc++.h>
using namespace std;

void cold_beans()
{
    cout << 1 << endl;
    cout << 5000 << endl;
    cout << 2 << ' ' << 2 << ' ' << 4 << ' ' << 4 << ' ';
    for (int i = 6; i <= 4998; i += 2)
        if (i % 4)
            cout << i << ' ';
        else
            cout << i << ' ' << i << ' ' << i << ' ';
    cout << 5000 << ' ' << 5000 << ' ' << 5000;
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