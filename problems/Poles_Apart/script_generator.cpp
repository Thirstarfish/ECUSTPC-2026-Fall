#include <bits/stdc++.h>
using namespace std;

int main()
{
    mt19937 g(114514);
    freopen("script.txt", "w", stdout);

    auto seed = [&]()
    { return g() % INT_MAX; };

    auto emit = [&]()
    { cout << "generator " << seed() << " > $\n"; };

    for (int i = 1; i <= 20; i++)
        emit();
    return 0;
}
