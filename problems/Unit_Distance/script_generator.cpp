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
    emit(3);
    for (int mode = 4; mode <= 11; ++mode)
        emit(mode);
    for (int i = 0; i < 4; ++i)
        emit(12);
    for (int i = 0; i < 6; ++i)
        emit(13);
    for (int i = 0; i < 8; ++i)
        emit(14);
    return 0;
}
