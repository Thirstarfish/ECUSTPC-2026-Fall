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

    emit(1);                        // sample
    emit(2);                        // tiny exhaustive
    emit(3);                        // line structures
    for (int i = 0; i < 3; ++i) emit(4); // cycles
    emit(5);                        // planted yes
    for (int i = 0; i < 3; ++i) emit(6); // small random
    for (int i = 0; i < 2; ++i) emit(7); // large random
    emit(8);                        // max T
    emit(9);                        // max line
    emit(10);                       // max DAG
    fflush(stdout);
    return 0;
}
