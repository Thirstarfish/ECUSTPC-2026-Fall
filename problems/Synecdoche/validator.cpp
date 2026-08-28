#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// Validator for Synecdoche (Problem A).
//
// Input is a single test case:
//   L K N
//   p_1 c_1        (K lines, p_i in [1, L], c_i a lowercase letter, all p_i distinct)
//   ...
//   p_K c_K
//   s_1            (N lines, non-empty lowercase strings, sum |s_i| <= 100)
//   ...
//   s_N
//
// Constraints:
// - 1 <= L <= 10^18
// - 0 <= K <= 500
// - 0 <= N <= 50
// - 1 <= p_i <= L, p_i pairwise distinct, c_i in [a-z]
// - sum |s_i| <= 100

int main(int argc, char *argv[])
{
    registerValidation(argc, argv);

    const long long LIM = 1000000000000000000LL; // 1e18
    const int MAX_K = 500;
    const int MAX_N = 50;
    const int MAX_TOTAL_LEN = 100;

    long long L = inf.readLong(1, LIM, "L");
    inf.readSpace();
    int K = inf.readInt(0, MAX_K, "K");
    inf.readSpace();
    int N = inf.readInt(0, MAX_N, "N");
    inf.readEoln();

    vector<long long> ps;
    ps.reserve(K);
    for (int i = 1; i <= K; ++i)
    {
        long long p = inf.readLong(1, L, format("p[%d]", i));
        inf.readSpace();
        inf.readToken("[a-z]", format("c[%d]", i));
        ps.push_back(p);
        inf.readEoln();
    }

    sort(ps.begin(), ps.end());
    for (int i = 1; i < K; ++i)
        ensuref(ps[i - 1] != ps[i], "position %lld appears more than once", ps[i]);

    long long totalLen = 0;
    for (int i = 1; i <= N; ++i)
    {
        string s = inf.readToken("[a-z]+", format("s[%d]", i));
        totalLen += (long long)s.size();
        ensuref(totalLen <= MAX_TOTAL_LEN, "sum of forbidden string lengths exceeds %d: %lld",
                MAX_TOTAL_LEN, totalLen);
        inf.readEoln();
    }

    inf.readEof();
    return 0;
}
