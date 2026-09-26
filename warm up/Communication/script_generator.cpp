#include <bits/stdc++.h>
using namespace std;

int main() {
    mt19937 g(114514);
    freopen("script.txt", "w", stdout);
    auto seed = [&]() { return (int)(g() % INT_MAX); };
    for (int i = 0; i < 8; ++i)
        cout << "generator 2 " << seed() << " > $\n";
    for (int i = 0; i < 12; ++i)
        cout << "generator 1 " << seed() << " > $\n";
    return 0;
}
