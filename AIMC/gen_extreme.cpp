// gen_extreme.cpp — biased generators for specific structures.
// Usage:
//   gen_extreme.exe N M H mode [maxV] [seed-bits...]
// Modes:
//   all-zero       all matrices identically 0
//   all-max        all matrices identically maxV
//   only-A1        only A_1 nonzero (random)
//   only-A2        only A_2 nonzero (random)
//   only-A34       only A_3, A_4 nonzero (test S_1 different-color cluster)
//   only-A56       only A_5, A_6 nonzero (test S_2 same-color cluster)
//   only-A78       only A_7, A_8 nonzero (test S_3 different-color vertex cluster)
//   binary         each cell independently 0 or maxV
//   contrast       A_3..A_8 large, A_1, A_2 zero (forces cluster decisions)

#define FOR_LINUX
#include "testlib.h"
#include <cstdlib>
#include <cstring>
#include <string>
#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#include <fcntl.h>
#endif

using namespace std;

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
#if defined(_WIN32) || defined(_WIN64)
    _setmode(_fileno(stdout), _O_BINARY);
#endif
    if (argc < 5) {
        fprintf(stderr, "Usage: %s N M H mode [maxV] [extra-seed]\n", argv[0]);
        return 1;
    }
    int N = atoi(argv[1]), M = atoi(argv[2]), H = atoi(argv[3]);
    string mode = argv[4];
    long long maxV = argc >= 6 ? atoll(argv[5]) : 1000000000LL;

    ensuref(1 <= N && N <= 20, "N out of range");
    ensuref(1 <= M && M <= 20, "M out of range");
    ensuref(1 <= H && H <= 20, "H out of range");
    ensuref((long long)N * M * H <= 8000, "N*M*H must be <= 8000");

    auto valFor = [&](int t, int /*x*/, int /*y*/, int /*z*/) -> long long {
        if (mode == "all-zero") return 0LL;
        if (mode == "all-max")  return maxV;
        if (mode == "only-A1") return t == 0 ? rnd.next(0LL, maxV) : 0LL;
        if (mode == "only-A2") return t == 1 ? rnd.next(0LL, maxV) : 0LL;
        if (mode == "only-A34") return (t == 2 || t == 3) ? rnd.next(0LL, maxV) : 0LL;
        if (mode == "only-A56") return (t == 4 || t == 5) ? rnd.next(0LL, maxV) : 0LL;
        if (mode == "only-A78") return (t == 6 || t == 7) ? rnd.next(0LL, maxV) : 0LL;
        if (mode == "binary")  return rnd.next(0, 1) ? maxV : 0LL;
        if (mode == "contrast") return t < 2 ? 0LL : rnd.next(0LL, maxV);
        return rnd.next(0LL, maxV);
    };

    printf("%d %d %d\n", N, M, H);
    for (int t = 0; t < 8; ++t) {
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < M; ++y) {
                for (int z = 0; z < H; ++z) {
                    long long v = valFor(t, x, y, z);
                    if (z) putchar(' ');
                    printf("%lld", v);
                }
                putchar('\n');
            }
            if (x + 1 < N) putchar('\n');
        }
        if (t + 1 < 8) putchar('\n');
    }
    fflush(stdout);
    _Exit(0);
}
