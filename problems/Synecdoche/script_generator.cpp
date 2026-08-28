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

    emit(1);                              // sample
    for (int i = 0; i < 5; ++i) emit(2);  // tiny random (brute-forcible)
    for (int i = 0; i < 8; ++i) emit(3);  // small random (DP-oracle verifiable)
    for (int i = 0; i < 5; ++i) emit(4);  // medium random (L in [1e6, 1e12])
    for (int i = 0; i < 6; ++i) emit(5);  // max L = 1e18 random
    emit(6);                              // single forbidden character
    emit(7);                              // self-overlap "aa"
    emit(8);                              // forbidden "ab"
    emit(9);                              // duplicate forbidden strings
    for (int i = 0; i < 3; ++i) emit(10); // max L, max K = 500, no forbidden
    for (int i = 0; i < 2; ++i) emit(11); // boundary positions 1 and L
    emit(12);                             // planted zero (single char)
    emit(13);                             // planted zero (fail-chain pair)
    emit(14);                             // max N = 50, max total length = 100
    emit(15);                             // long forbidden string "a"*100
    emit(16);                             // hack: single 2^59-1 all-ones gap
    for (int i = 0; i < 3; ++i) emit(17); // hack: greedy pack of all-ones gaps
    for (int i = 0; i < 3; ++i) emit(18); // hack: K=500 with all gaps 2^50-1

    fflush(stdout);
    return 0;
}
