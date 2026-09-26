#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

static constexpr long long MAX_X = 1000000000000000000LL;
static constexpr int LEN = 13;

int main(int argc, char *argv[])
{
    registerInteraction(argc, argv);
    int run = inf.readInt(0, 1, "run");
    if (run == 0)
    {
        long long x = inf.readLong(0, MAX_X, "x");
        cout << "send\n"
             << x << '\n'
             << flush;
        string code = ouf.readToken();
        if ((int)code.size() != LEN)
            quitf(_wa, "the code must contain exactly %d letters", LEN);
        for (int i = 0; i < LEN; ++i)
            if (code[i] < 'a' || code[i] > 'z')
                quitf(_wa, "code[%d] is not a lowercase English letter", i + 1);
        if (!ouf.seekEof())
            quitf(_pe, "extra tokens in the first run");
        tout << 1 << '\n'
             << x << '\n'
             << code << '\n';
        quitf(_ok, "first run accepted");
    }
    else
    {
        long long expected = inf.readLong(0, MAX_X, "expected");
        string code = inf.readToken();
        cout << "receive\n"
             << code << '\n'
             << flush;
        long long answer = ouf.readLong(0, MAX_X, "answer");
        if (!ouf.seekEof())
            quitf(_pe, "extra tokens in the second run");
        int correct = (answer == expected);
        tout << correct << " 1\n";
        if (!correct)
            quitf(_wa, "expected %lld, received %lld", expected, answer);
        quitf(_ok, "decoded correctly");
    }
    return 0;
}
