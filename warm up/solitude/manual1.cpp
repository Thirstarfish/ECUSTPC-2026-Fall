#include <bits/stdc++.h>
using namespace std;

void cold_beans()
{
    cout << 1 << endl;
    cout << 5000 << endl;
    for (int i = 1; i <= 2500; i++)
        cout << 4996 << ' ';
    for (int i = 1; i <= 2498; i++)
        cout << 5000 << ' ';
    cout << "1 1";
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