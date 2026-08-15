#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    registerValidation(argc, argv);

    const int LIM = 1000000000; // 1e9
    int T = inf.readInt(1, 100000, "T");
    inf.readEoln();

    for (int tc = 1; tc <= T; ++tc)
    {
        setTestCase(tc);
        inf.readInt(0, LIM, format("A[%d]", tc));
        inf.readSpace();
        inf.readInt(0, LIM, format("B[%d]", tc));
        inf.readEoln();
    }

    inf.readEof();
    return 0;
}
