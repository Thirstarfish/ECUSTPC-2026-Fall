#include <bits/stdc++.h>
using namespace std;

mt19937 g(114514);

int main()
{
    freopen("script.txt", "w", stdout);
    for (int j = 1; j <= 12; j++)
        for (int i = 1; i <= 3; i++)
            cout << "generator " << j << ' ' << g() % INT_MAX << " > $\n";
    for (int i = 1; i <= 5; i++)
        cout << "generator " << 4 << ' ' << g() % INT_MAX << " > $\n";
    return 0;
}