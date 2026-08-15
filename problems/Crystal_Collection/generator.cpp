#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// Generator for Crystal Collection problem
// Usage: ./gen mode seed > output.in

const int MIN_VAL = -1000000000;
const int MAX_VAL = 1000000000;

// Mode 1: Small random cases (n <= 20)
void gen1()
{
    int T = 10000;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(1, 20);
        cout << n << '\n';
        for (int i = 1; i <= n; i++)
        {
            cout << rnd.next(-100, 100);
            if (i < n) cout << ' ';
        }
        cout << '\n';
    }
}

// Mode 2: Medium cases with small values (n <= 1000)
void gen2()
{
    int T = 200;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(100, 1000);
        cout << n << '\n';
        for (int i = 1; i <= n; i++)
        {
            cout << rnd.next(-1000, 1000);
            if (i < n) cout << ' ';
        }
        cout << '\n';
    }
}

// Mode 3: Large cases with full range values
void gen3()
{
    int T = 100;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(1000, 2000);
        cout << n << '\n';
        for (int i = 1; i <= n; i++)
        {
            cout << rnd.next(MIN_VAL, MAX_VAL);
            if (i < n) cout << ' ';
        }
        cout << '\n';
    }
}

// Mode 4: Maximum size single test case
void gen4()
{
    int T = 1;
    cout << T << '\n';
    int n = rnd.next(199000, 200000);
    cout << n << '\n';
    for (int i = 1; i <= n; i++)
    {
        cout << rnd.next(MIN_VAL, MAX_VAL);
        if (i < n) cout << ' ';
    }
    cout << '\n';
}

// Mode 5: All positive values
void gen5()
{
    int T = 200;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(50, 1000);
        cout << n << '\n';
        for (int i = 1; i <= n; i++)
        {
            cout << rnd.next(1, MAX_VAL);
            if (i < n) cout << ' ';
        }
        cout << '\n';
    }
}

// Mode 6: All negative values
void gen6()
{
    int T = 200;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(50, 1000);
        cout << n << '\n';
        for (int i = 1; i <= n; i++)
        {
            cout << rnd.next(MIN_VAL, -1);
            if (i < n) cout << ' ';
        }
        cout << '\n';
    }
}

// Mode 7: Alternating positive and negative
void gen7()
{
    int T = 200;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(50, 1000);
        cout << n << '\n';
        for (int i = 1; i <= n; i++)
        {
            if (i % 2 == 1)
                cout << rnd.next(1, MAX_VAL);
            else
                cout << rnd.next(MIN_VAL, -1);
            if (i < n) cout << ' ';
        }
        cout << '\n';
    }
}

// Mode 8: Sorted (increasing)
void gen8()
{
    int T = 200;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(50, 1000);
        cout << n << '\n';
        vector<int> a(n);
        for (int i = 0; i < n; i++)
        {
            a[i] = rnd.next(MIN_VAL, MAX_VAL);
        }
        sort(a.begin(), a.end());
        for (int i = 0; i < n; i++)
        {
            cout << a[i];
            if (i < n - 1) cout << ' ';
        }
        cout << '\n';
    }
}

// Mode 9: Sorted (decreasing)
void gen9()
{
    int T = 200;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(50, 1000);
        cout << n << '\n';
        vector<int> a(n);
        for (int i = 0; i < n; i++)
        {
            a[i] = rnd.next(MIN_VAL, MAX_VAL);
        }
        sort(a.begin(), a.end(), greater<int>());
        for (int i = 0; i < n; i++)
        {
            cout << a[i];
            if (i < n - 1) cout << ' ';
        }
        cout << '\n';
    }
}

// Mode 10: Edge case - all zeros
void gen10()
{
    int T = 200;
    cout << T << '\n';
    while (T--)
    {
        int n = rnd.next(1, 1000);
        cout << n << '\n';
        for (int i = 1; i <= n; i++)
        {
            cout << 0;
            if (i < n) cout << ' ';
        }
        cout << '\n';
    }
}

// Mode 11: Edge case - n = 1
void gen11()
{
    int T = 10000;
    cout << T << '\n';
    while (T--)
    {
        cout << 1 << '\n';
        cout << rnd.next(MIN_VAL, MAX_VAL) << '\n';
    }
}

// Mode 12: Mixed small and large n
void gen12()
{
    int T = rnd.next(1000, 5000);
    cout << T << '\n';
    int total_n = 0;
    while (T--)
    {
        int n;
        if (total_n < 100000)
            n = rnd.next(1, 100);
        else
            n = rnd.next(1, 10);
        total_n += n;
        cout << n << '\n';
        for (int i = 1; i <= n; i++)
        {
            cout << rnd.next(MIN_VAL, MAX_VAL);
            if (i < n) cout << ' ';
        }
        cout << '\n';
    }
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
    else if (mode == 7)
        gen7();
    else if (mode == 8)
        gen8();
    else if (mode == 9)
        gen9();
    else if (mode == 10)
        gen10();
    else if (mode == 11)
        gen11();
    else if (mode == 12)
        gen12();
    else
    {
        // Default: random
        gen3();
    }

    return 0;
}
