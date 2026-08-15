#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// Usage: generator <mode> <seed> > output.in

namespace
{
const long long LIM = 1000000000LL; // 1e9
const int MAX_T = 100000;

vector<pair<long long, long long>> tests;

void addTest(long long A, long long B)
{
    ensuref(0 <= A && A <= LIM, "A=%lld is out of range", A);
    ensuref(0 <= B && B <= LIM, "B=%lld is out of range", B);
    ensuref((int)tests.size() < MAX_T, "T exceeds 100000");
    tests.push_back({A, B});
}

void printTests()
{
    cout << tests.size() << '\n';
    for (const auto &p : tests)
        cout << p.first << ' ' << p.second << '\n';
}

void modeSample()
{
    addTest(7, 110);
    addTest(25, 30);
    addTest(0, 0);
    addTest(23, 59);
    addTest(100, 200);
}

// Exhaustively cover the wrap-around bands around 24 (hours) and 60 (minutes).
void modeTinyExhaustive()
{
    for (long long A = 0; A <= 50; ++A)
        for (long long B = 0; B <= 130; ++B)
            addTest(A, B);
}

void modeSmallRandom()
{
    const int T = 10000;
    for (int i = 0; i < T; ++i)
        addTest(rnd.next(0, 200), rnd.next(0, 200));
}

void modeBoundary()
{
    // Exact boundary pairs straddling 23/24 and 59/60, plus coefficient extremes.
    long long As[] = {0, 1, 22, 23, 24, 25, 47, 48, 49, LIM, LIM - 1, 0, 24, 23, LIM};
    long long Bs[] = {0, 1, 58, 59, 60, 61, 119, 120, 121, 0, LIM, LIM - 1, LIM, 60, 60};
    for (int i = 0; i < 15; ++i)
        addTest(As[i], Bs[i]);

    // One coordinate pinned to an extreme, the other random.
    for (int i = 0; i < 200; ++i)
        addTest(rnd.next(0, 1) ? 0 : LIM, rnd.next(0LL, LIM));
    for (int i = 0; i < 200; ++i)
        addTest(rnd.next(0LL, LIM), rnd.next(0, 1) ? 0 : LIM);
}

void modeLargeRandom()
{
    const int T = 10000;
    for (int i = 0; i < T; ++i)
        addTest(rnd.next(0LL, LIM), rnd.next(0LL, LIM));
}

void modeMaxT()
{
    const int T = MAX_T;
    for (int i = 0; i < T; ++i)
        addTest(rnd.next(0LL, LIM), rnd.next(0LL, LIM));
}
} // namespace

int main(int argc, char *argv[])
{
    registerGen(argc, argv, 1);
    int mode = opt<int>(1);

    if (mode == 1) modeSample();
    else if (mode == 2) modeTinyExhaustive();
    else if (mode == 3) modeSmallRandom();
    else if (mode == 4) modeBoundary();
    else if (mode == 5) modeLargeRandom();
    else if (mode == 6) modeMaxT();
    else ensuref(false, "unknown mode: %d", mode);

    printTests();
    return 0;
}
