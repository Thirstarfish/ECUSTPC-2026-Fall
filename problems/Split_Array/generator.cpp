#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// ./gen mode seed> $
const int mn = -1000000000, mx = 1000000000;

void gen1()
{
    int T = 10000;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(1, 20);
        int m = rnd.next(1, (n + 1) / 2);
        cout << n << ' ' << m << '\n';
        for (int i = 1; i <= n; i++)
            cout << rnd.next(-15, 15) << " \n"[i == n];
        for (int i = 1; i <= n; i++)
            cout << rnd.next(-15, 15) << " \n"[i == n];
    }
}

void gen2()
{
    int T = 10000;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(1, 20);
        int m = rnd.next(1, (n + 1) / 2);
        cout << n << ' ' << m << '\n';
        for (int i = 1; i <= n; i++)
            cout << rnd.next(mn, mx) << " \n"[i == n];
        for (int i = 1; i <= n; i++)
            cout << rnd.next(mn, mx) << " \n"[i == n];
    }
}

void gen3()
{
    int T = 100;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(1000, 2000);
        int m = rnd.next(1, (n + 1) / 2);
        cout << n << ' ' << m << '\n';
        for (int i = 1; i <= n; i++)
            cout << rnd.next(mn, mx) << " \n"[i == n];
        for (int i = 1; i <= n; i++)
            cout << rnd.next(mn, mx) << " \n"[i == n];
    }
}

void gen4()
{
    int T = 100;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(1000, 2000);
        int m = (n + 1) / 2;
        if (rnd.next(0, 1))
            m = 1;
        cout << n << ' ' << m << '\n';
        for (int i = 1; i <= n; i++)
            cout << rnd.next(mn, mx) << " \n"[i == n];
        for (int i = 1; i <= n; i++)
            cout << rnd.next(mn, mx) << " \n"[i == n];
    }
}

void gen5()
{
    int T = 1;
    cout << T << '\n';
    int n = rnd.next(199999, 200000);
    int m = rnd.next(1, (n + 1) / 2);
    cout << n << ' ' << m << '\n';
    for (int i = 1; i <= n; i++)
        cout << rnd.next(mn, mx) << " \n"[i == n];
    for (int i = 1; i <= n; i++)
        cout << rnd.next(mn, mx) << " \n"[i == n];
}

void gen6()
{
    int T = 1;
    cout << T << '\n';
    int n = rnd.next(199999, 200000);
    int m = (n + 1) / 2;
    if (rnd.next(0, 1))
        m = 1;
    cout << n << ' ' << m << '\n';
    for (int i = 1; i <= n; i++)
        cout << rnd.next(mn, mx) << " \n"[i == n];
    for (int i = 1; i <= n; i++)
        cout << rnd.next(mn, mx) << " \n"[i == n];
}

int main(int argc, char *argv[])
{
    registerGen(argc, argv, 1);
    int mode = opt<int>(1);
    if (mode == 1)
        gen1();
    else if (mode == 2)
        gen2();
    else if (mode == 3)
        gen3();
    else if (mode == 4)
        gen4();
    else if (mode == 5)
        gen5();
    else if (mode == 6)
        gen6();
    return 0;
}