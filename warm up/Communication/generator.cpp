#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    int mode = opt<int>(1);
    long long x;
    if (mode == 1) {
        x = rnd.next(0LL, 1000000000000000000LL);
    } else if (mode == 2) {
        static const long long edge[] = {0, 1, 25, 26, 26 * 26 - 1,
            1000000000000000000LL - 1, 1000000000000000000LL};
        x = edge[rnd.next(0, (int)(sizeof(edge) / sizeof(edge[0])) - 1)];
    } else if (mode == 3) {
        x = rnd.next(0LL, 1000000000000000000LL);
        if (rnd.next(0, 1)) x = 1000000000000000000LL - x;
    } else {
        ensuref(false, "unknown generator mode %d", mode);
    }
    cout << 0 << '\n' << x << '\n';
}
