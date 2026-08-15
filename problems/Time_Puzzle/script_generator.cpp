#include <bits/stdc++.h>
using namespace std;

int main()
{
    mt19937 g(114514);
    freopen("script.txt", "w", stdout);

    auto seed = [&]() {
        return g() % (INT_MAX - 1u) + 1u;
    };
    auto emit = [&](int mode) {
        cout << "generator " << mode << ' ' << seed() << " > $\n";
    };

    emit(1);
    emit(2);
    for (int i = 0; i < 3; ++i) emit(3);
    emit(4);
    for (int i = 0; i < 3; ++i) emit(5);
    for (int i = 0; i < 2; ++i) emit(6);
    fflush(stdout);
    return 0;
}
