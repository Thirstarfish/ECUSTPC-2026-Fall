#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    registerValidation(argc, argv);

    const int MAX_TESTS = 100;
    const int MAX_N = 5000;
    const int MAX_SUM_N = 5000;

    int T = inf.readInt(1, MAX_TESTS, "T");
    inf.readEoln();

    int totalN = 0;
    for (int tc = 1; tc <= T; ++tc)
    {
        setTestCase(tc);
        int n = inf.readInt(2, MAX_N, "n");
        inf.readEoln();

        totalN += n;
        ensuref(totalN <= MAX_SUM_N,
                "sum of n exceeds %d after test case %d: %d",
                MAX_SUM_N, tc, totalN);
    }

    inf.readEof();
    return 0;
}
