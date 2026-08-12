// gen_random.cpp — generic random generator.
// Usage:
//   gen_random.exe N M H maxV [-pattern uniform|binary|sparse|checker] [seed]
// Or with named args (testlib registerGen):
//   gen_random.exe N M H maxV pattern  (extra arg becomes RNG seed)
//
// Always writes to stdout in the canonical format used by the validator.

#define FOR_LINUX
#include "testlib.h"
#include <cstdlib>
#include <string>
#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#include <fcntl.h>
#endif

using namespace std;

int N, M, H;
long long maxV;
string pattern;

long long sample_value() {
    if (pattern == "uniform")  return rnd.next(0LL, maxV);
    if (pattern == "binary")   return rnd.next(0, 1) ? maxV : 0LL;
    if (pattern == "sparse")   return rnd.next(0, 9) == 0 ? rnd.next(1LL, maxV) : 0LL;
    if (pattern == "constant") return maxV;
    if (pattern == "checker") {
        // caller will overwrite this, just placeholder
        return rnd.next(0LL, maxV);
    }
    return rnd.next(0LL, maxV);
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);
#if defined(_WIN32) || defined(_WIN64)
    _setmode(_fileno(stdout), _O_BINARY);
#endif

    if (argc < 5) {
        fprintf(stderr,
                "Usage: %s N M H maxV [pattern=uniform] [extra-seed-bits...]\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    H = atoi(argv[3]);
    maxV = atoll(argv[4]);
    pattern = argc >= 6 ? argv[5] : "uniform";

    ensuref(1 <= N && N <= 20, "N out of range");
    ensuref(1 <= M && M <= 20, "M out of range");
    ensuref(1 <= H && H <= 20, "H out of range");
    ensuref((long long)N * M * H <= 8000, "N*M*H must be <= 8000");
    ensuref(0 <= maxV && maxV <= 1000000000LL, "maxV out of range");

    printf("%d %d %d\n", N, M, H);
    for (int t = 0; t < 8; ++t) {
        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < M; ++y) {
                for (int z = 0; z < H; ++z) {
                    long long v;
                    if (pattern == "checker") {
                        // Alternate between 0 and maxV based on parity (x+y+z+t).
                        v = ((x + y + z + t) & 1) ? maxV : 0LL;
                    } else {
                        v = sample_value();
                    }
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
