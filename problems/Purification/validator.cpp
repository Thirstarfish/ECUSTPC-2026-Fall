#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    registerValidation(argc, argv);

    const int LIM = 1000000000; // 1e9
    int T = inf.readInt(1, 100000, "T");
    inf.readEoln();

    long long totalN = 0;
    for (int tc = 1; tc <= T; ++tc)
    {
        setTestCase(tc);
        int n = inf.readInt(1, 200000, "n");
        inf.readEoln();
        totalN += n;
        ensuref(totalN <= 200000, "sum of n exceeds 200000");

        vector<pair<int, int>> pts;
        pts.reserve(n);
        for (int i = 1; i <= n; ++i)
        {
            int x = inf.readInt(-LIM, LIM, format("x[%d]", i));
            inf.readSpace();
            int y = inf.readInt(-LIM, LIM, format("y[%d]", i));
            inf.readEoln();
            pts.emplace_back(x, y);
        }

        sort(pts.begin(), pts.end());
        for (int i = 1; i < n; ++i)
            ensuref(pts[i - 1] != pts[i], "duplicate cannon position (%d, %d)",
                    pts[i].first, pts[i].second);
    }

    inf.readEof();
    return 0;
}
