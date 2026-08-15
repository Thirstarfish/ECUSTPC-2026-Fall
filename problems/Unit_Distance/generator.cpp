#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// Usage: generator <mode> <seed> > output.in

namespace
{
const int MAX_TESTS = 100;
const int MIN_N = 2;
const int MAX_N = 5000;
const int MAX_SUM_N = 5000;

vector<int> tests;
int totalN = 0;

void addTest(int n)
{
    ensuref(MIN_N <= n && n <= MAX_N, "n is out of range: %d", n);
    ensuref((int)tests.size() < MAX_TESTS, "T exceeds %d", MAX_TESTS);
    totalN += n;
    ensuref(totalN <= MAX_SUM_N, "sum of n exceeds %d", MAX_SUM_N);
    tests.push_back(n);
}

void printTests()
{
    cout << tests.size() << '\n';
    for (int n : tests)
        cout << n << '\n';
}

void modeHandmade()
{
    const int values[] = {
        2, 3, 4, 5, 7, 8, 9,
        15, 16, 17, 31, 32, 33,
        63, 64, 65, 100,
        127, 128, 129,
        255, 256, 257,
        511, 512, 513, 999};
    for (int n : values)
        addTest(n);
}

void modeAllSmall()
{
    for (int n = 2; n <= 99; ++n)
        addTest(n);
}

void modeAround1024()
{
    for (int n : {928, 1000, 1023, 1024, 1025})
        addTest(n);
}

void modeLargeSingle(int n)
{
    addTest(n);
}

void distributeRemainder(vector<int> &values, int remainder)
{
    while (remainder-- > 0)
        ++values[rnd.next(0, (int)values.size() - 1)];
}

void modeMaximumTestCount()
{
    vector<int> values(MAX_TESTS, MIN_N);
    distributeRemainder(values, MAX_SUM_N - MAX_TESTS * MIN_N);
    shuffle(values.begin(), values.end());
    for (int n : values)
        addTest(n);
}

void modeRandomAggregate()
{
    int T = rnd.next(1, MAX_TESTS);
    int targetSum = rnd.next(T * MIN_N, MAX_SUM_N);
    vector<int> values(T, MIN_N);
    distributeRemainder(values, targetSum - T * MIN_N);
    shuffle(values.begin(), values.end());
    for (int n : values)
        addTest(n);
}

void modeRandomSingle()
{
    addTest(rnd.next(MIN_N, MAX_N));
}
} // namespace

int main(int argc, char *argv[])
{
    registerGen(argc, argv, 1);
    int mode = opt<int>(1);

    if (mode == 1) modeHandmade();
    else if (mode == 2) modeAllSmall();
    else if (mode == 3) modeAround1024();
    else if (mode == 4) modeLargeSingle(2047);
    else if (mode == 5) modeLargeSingle(2048);
    else if (mode == 6) modeLargeSingle(2049);
    else if (mode == 7) modeLargeSingle(4095);
    else if (mode == 8) modeLargeSingle(4096);
    else if (mode == 9) modeLargeSingle(4097);
    else if (mode == 10) modeLargeSingle(4999);
    else if (mode == 11) modeLargeSingle(5000);
    else if (mode == 12) modeMaximumTestCount();
    else if (mode == 13) modeRandomAggregate();
    else if (mode == 14) modeRandomSingle();
    else ensuref(false, "unknown mode: %d", mode);

    printTests();
    return 0;
}
