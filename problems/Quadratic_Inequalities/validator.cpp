#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    registerValidation(argc, argv);

    const int LIM = 100000;
    int T = inf.readInt(1, 10000, "T");
    inf.readEoln();

    long long totalN = 0;
    for (int tc = 1; tc <= T; ++tc)
    {
        setTestCase(tc);
        int n = inf.readInt(1, 200000, "n");
        inf.readEoln();
        totalN += n;
        ensuref(totalN <= 200000, "sum of n exceeds 200000");

        for (int i = 1; i <= n; ++i)
        {
            int a = inf.readInt(-LIM, -1, format("a[%d]", i));
            inf.readSpace();
            inf.readInt(-LIM, LIM, format("b[%d]", i));
            inf.readSpace();
            inf.readInt(-LIM, LIM, format("c[%d]", i));
            inf.readEoln();
        }
    }

    inf.readEof();
    return 0;
}
