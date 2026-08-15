#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// Validator for Crystal Collection problem
// Constraints:
// - 1 <= T <= 10^4
// - 1 <= n <= 2*10^5
// - -10^9 <= a_i <= 10^9
// - sum of all n <= 2*10^5

int main(int argc, char *argv[])
{
    registerValidation(argc, argv);

    int T = inf.readInt(1, 10000, "T");
    inf.readEoln();

    long long total_n = 0;

    for (int test = 1; test <= T; test++)
    {
        int n = inf.readInt(1, 200000, "n");
        inf.readEoln();

        total_n += n;

        for (int i = 1; i <= n; i++)
        {
            inf.readInt(-1000000000, 1000000000, "a");
            if (i < n)
                inf.readSpace();
            else
                inf.readEoln();
        }
    }

    inf.readEof();

    ensuref(total_n <= 200000, "sum of all n exceeds 200000: %lld", total_n);

    return 0;
}
