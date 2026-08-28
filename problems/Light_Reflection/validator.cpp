#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

namespace
{
bool isBoundary(char side)
{
    return side == 'U' || side == 'D' || side == 'L' || side == 'R';
}

int positionLimit(char side, int n, int m)
{
    return side == 'U' || side == 'D' ? m : n;
}
} // namespace

int main(int argc, char *argv[])
{
    registerValidation(argc, argv);

    int T = inf.readInt(1, 100, "T");
    inf.readEoln();

    long long totalCells = 0;
    for (int tc = 1; tc <= T; ++tc)
    {
        setTestCase(tc);

        int n = inf.readInt(1, 200000, "n");
        inf.readSpace();
        int m = inf.readInt(1, 200000, "m");
        inf.readEoln();

        long long cells = 1LL * n * m;
        ensuref(cells <= 200000, "n*m exceeds 200000: %lld", cells);
        totalCells += cells;
        ensuref(totalCells <= 200000,
                "sum of n*m exceeds 200000 after test case %d: %lld",
                tc, totalCells);

        for (int i = 1; i <= n; ++i)
        {
            string row = inf.readToken();
            inf.readEoln();
            ensuref((int)row.size() == m,
                    "grid[%d] has length %d, expected %d",
                    i, (int)row.size(), m);
            for (int j = 1; j <= m; ++j)
                ensuref(row[j - 1] == '.' || row[j - 1] == '/' || row[j - 1] == '\\',
                        "grid[%d][%d] has invalid character", i, j);
        }

        string firstSideToken = inf.readToken("[UDLR]", "d1");
        ensuref(firstSideToken.size() == 1 && isBoundary(firstSideToken[0]),
                "d1 must be one of U, D, L, R");
        char d1 = firstSideToken[0];
        inf.readSpace();
        int p1 = inf.readInt(1, positionLimit(d1, n, m), "p1");
        inf.readSpace();

        string secondSideToken = inf.readToken("[UDLR]", "d2");
        ensuref(secondSideToken.size() == 1 && isBoundary(secondSideToken[0]),
                "d2 must be one of U, D, L, R");
        char d2 = secondSideToken[0];
        inf.readSpace();
        int p2 = inf.readInt(1, positionLimit(d2, n, m), "p2");
        inf.readEoln();

        ensuref(d1 != d2 || p1 != p2,
                "the entry and target boundary positions must be different");
    }

    inf.readEof();
    return 0;
}
