#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// 检验空格 inf.readSpace();
// 检验换行 inf.readEoln();
// 读取整数 inf.readInt(l, r, "name");

int main(int argc, char *argv[])
{
    registerValidation(argc, argv);
    int T = inf.readInt(1, 10000, "T");
    inf.readEoln();
    int cnt = 0;
    while (T--)
    {
        int n = inf.readInt(1, 200000, "n");
        cnt += n;
        inf.readSpace();
        inf.readInt(1, (n + 1) / 2, "m");
        inf.readEoln();
        for (int i = 1; i < n; i++)
        {
            inf.readInt(-1e9, 1e9, "a");
            inf.readSpace();
        }
        inf.readInt(-1e9, 1e9, "a");
        inf.readEoln();
        for (int i = 1; i < n; i++)
        {
            inf.readInt(-1e9, 1e9, "b");
            inf.readSpace();
        }
        inf.readInt(-1e9, 1e9, "b");
        inf.readEoln();
    }
    inf.readEof();
    ensuref(cnt <= 200000, "sum_n larger than 2e5");
    return 0;
}
