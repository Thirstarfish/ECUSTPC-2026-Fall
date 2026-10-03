#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// Usage: generator <mode> <seed> > output.in

namespace
{
const int COEF_LIM = 100000;
const int SUM_N_LIM = 200000;

struct Quad
{
    int a, b, c;
};

using Test = vector<Quad>;

vector<Test> tests;
int totalN = 0;

Quad makeQuad(long long a, long long b, long long c)
{
    ensuref(-COEF_LIM <= a && a <= -1, "a=%lld must satisfy -100000 <= a <= -1", a);
    ensuref(-COEF_LIM <= b && b <= COEF_LIM, "b=%lld is out of range", b);
    ensuref(-COEF_LIM <= c && c <= COEF_LIM, "c=%lld is out of range", c);
    return {(int)a, (int)b, (int)c};
}

// -(x-(l-1))(x-(r+1)) > 0：可行的整数集合恰为区间 [l,r]。
// 需要 |l+r| <= 10^5 且 |(l-1)(r+1)| <= 10^5，因此 l,r 大致被限制在 [-316,316]。
Quad allowedInterval(int l, int r, int scale = 1)
{
    ensuref(l <= r && scale >= 1, "invalid allowed interval");
    long long p = l - 1LL;
    long long q = r + 1LL;
    return makeQuad(-scale, 1LL * scale * (p + q), -1LL * scale * p * q);
}

Quad allowedSingleton(int x)
{
    return allowedInterval(x, x);
}

// 对给定区间随机取一个仍然合法的放缩倍数（系数不越界）。
int randomScale(int l, int r)
{
    long long p = l - 1LL, q = r + 1LL;
    long long byC = abs(p * q) ? COEF_LIM / abs(p * q) : COEF_LIM;
    long long byB = abs(p + q) ? COEF_LIM / abs(p + q) : COEF_LIM;
    long long mx = max(1LL, min(byC, byB));
    return (int)rnd.next(1LL, mx);
}

// -(x-p)(x-q) > 0：两根恰为整数 p,q，判别式是完全平方数。
// 可行的整数恰好是开区间 (p,q) 中的整数。
Quad integerRootsQuad(long long p, long long q)
{
    ensuref(p != q, "the two roots must be distinct");
    return makeQuad(-1, p + q, -1LL * p * q);
}

// -x^2+(2k+1)x+(1-k^2-k) > 0：判别式为 5，两根 ((2k+1)±sqrt(5))/2 为无理数，
// 可行的整数恰为 k 与 k+1。
Quad irrationalRootsQuad(int k)
{
    return makeQuad(-1, 2LL * k + 1, 1LL - 1LL * k * k - k);
}

// -4x^2+8m x+(1-4m^2) > 0：两根为 m±1/2，可行的整数恰为 m。
Quad halfRootsQuad(int m)
{
    return makeQuad(-4, 8LL * m, 1 - 4LL * m * m);
}

// 随机生成一个被 x 满足的不等式（|x| <= 316 时直接随机构造系数）。
Quad randomSatisfiedAt(int x)
{
    if (abs(x) > 316)
        return makeQuad(-1, x > 0 ? COEF_LIM : -COEF_LIM, rnd.next(1, COEF_LIM));

    long long sq = max(1LL, 1LL * x * x);
    int maxAbsA = (int)max(1LL, min<long long>(COEF_LIM, 2LL * COEF_LIM / sq));
    for (int attempt = 0; attempt < 500; ++attempt)
    {
        int a = -rnd.next(1, maxAbsA);
        int b = rnd.next(-COEF_LIM, COEF_LIM);
        long long base = 1LL * a * x * x + 1LL * b * x;
        long long low = max(-1LL * COEF_LIM, 1 - base);
        if (low <= COEF_LIM)
            return makeQuad(a, b, rnd.next((int)low, COEF_LIM));
    }
    return allowedSingleton(x);
}

// a=-1, b=±10^5, c>0 时 -x^2±10^5x+c > 0 恰好覆盖到 |x| = 10^5。
Quad extremeSatisfiedAt(int x)
{
    int s = (x > 0) ? 1 : -1;
    switch (rnd.next(0, 2))
    {
    case 0: return makeQuad(-1, 1LL * s * COEF_LIM, COEF_LIM);
    case 1: return makeQuad(-1, 1LL * s * COEF_LIM, COEF_LIM - 1);
    default: return makeQuad(-1, 1LL * s * COEF_LIM, rnd.next(1, COEF_LIM));
    }
}

// -x^2+10^5 x+c：当 c >= 0 时右端点恰好是 10^5（c=0 时退化到 10^5-1）。
Quad rightmostQuad(int c)
{
    return makeQuad(-1, COEF_LIM, c);
}

// -x^2-10^5 x+c：当 c >= 0 时左端点恰好是 -10^5。
Quad leftmostQuad(int c)
{
    return makeQuad(-1, -COEF_LIM, c);
}

template <class T>
void shuffleWithRnd(vector<T> &v)
{
    for (int i = (int)v.size() - 1; i > 0; --i)
        swap(v[i], v[rnd.next(0, i)]);
}

void addTest(Test test)
{
    ensuref(!test.empty(), "a test case must not be empty");
    ensuref((int)test.size() <= SUM_N_LIM, "n exceeds 200000");
    ensuref((int)tests.size() < 10000, "T exceeds 10000");
    totalN += (int)test.size();
    ensuref(totalN <= SUM_N_LIM, "sum of n exceeds 200000");
    tests.push_back(std::move(test));
}

void printTests()
{
    cout << tests.size() << '\n';
    for (const Test &test : tests)
    {
        cout << test.size() << '\n';
        for (const Quad &q : test)
            cout << q.a << ' ' << q.b << ' ' << q.c << '\n';
    }
}

void modeHandmade()
{
    vector<Test> corpus;

    // 单条不等式：无解（空集、判别式 < 0、判别式 = 0、严格不等号导致的空区间）
    corpus.push_back({makeQuad(-1, 0, 0)});                 // -x^2 > 0
    corpus.push_back({makeQuad(-1, 2, -1)});                // -(x-1)^2 > 0
    corpus.push_back({makeQuad(-1, 3, -2)});                // 1 < x < 2
    corpus.push_back({makeQuad(-1, 1, 0)});                 // 0 < x < 1
    corpus.push_back({makeQuad(-1, -1, 0)});                // -1 < x < 0
    corpus.push_back({integerRootsQuad(0, 1)});             // 0 < x < 1
    corpus.push_back({integerRootsQuad(-1, 0)});            // -1 < x < 0
    corpus.push_back({integerRootsQuad(2, 3)});             // 2 < x < 3
    corpus.push_back({makeQuad(-100000, 100000, -1)});      // 两根极接近 0 与 1
    corpus.push_back({makeQuad(-99999, 100000, -1)});       // 判别式完全平方，仍无解
    corpus.push_back({makeQuad(-100000, 100000, -100000)});
    corpus.push_back({makeQuad(-100000, -100000, -100000)});

    // 单条不等式：恰有一个解
    corpus.push_back({makeQuad(-1, 0, 1)});                 // x = 0
    corpus.push_back({makeQuad(-100000, 0, 100000)});       // x = 0
    corpus.push_back({makeQuad(-100000, 100000, 100000)});  // x ∈ {0,1}
    corpus.push_back({makeQuad(-99999, 100000, 0)});        // x = 1
    corpus.push_back({halfRootsQuad(0)});                   // x = 0
    corpus.push_back({halfRootsQuad(100)});                 // x = 100
    corpus.push_back({halfRootsQuad(-100)});                // x = -100
    corpus.push_back({allowedSingleton(0)});
    corpus.push_back({allowedSingleton(315)});
    corpus.push_back({allowedSingleton(-315)});
    corpus.push_back({irrationalRootsQuad(0)});             // x ∈ {0,1}
    corpus.push_back({irrationalRootsQuad(315)});           // x ∈ {315,316}
    corpus.push_back({irrationalRootsQuad(-1)});            // x ∈ {-1,0}

    // 单条不等式：区间右端 / 左端恰好落在 10^5 上
    corpus.push_back({rightmostQuad(1)});                   // x ∈ [0,100000]
    corpus.push_back({rightmostQuad(0)});                   // x ∈ [0,99999]
    corpus.push_back({rightmostQuad(-99999)});              // x ∈ [2,99998]
    corpus.push_back({leftmostQuad(1)});                    // x ∈ [-100000,0]
    corpus.push_back({leftmostQuad(0)});                    // x ∈ [-99999,-1]
    corpus.push_back({leftmostQuad(-99999)});               // x ∈ [-99998,-2]

    // 交集：恰好留下一个整数 / 恰好空掉
    corpus.push_back({allowedInterval(-10, 0), allowedInterval(0, 10)});
    corpus.push_back({allowedInterval(-10, -1), allowedInterval(0, 10)});
    corpus.push_back({allowedInterval(0, 3), allowedInterval(2, 5)});
    corpus.push_back({allowedInterval(0, 3), allowedInterval(4, 5)});
    corpus.push_back({allowedInterval(-315, 315), allowedInterval(-315, 314)});
    corpus.push_back({allowedInterval(-315, 315), allowedInterval(-314, 315)});
    corpus.push_back({allowedInterval(-315, 315), allowedSingleton(315)});
    corpus.push_back({allowedInterval(-315, 315), allowedSingleton(315), allowedSingleton(316)});
    corpus.push_back({allowedInterval(-315, 315), allowedSingleton(315), allowedInterval(-315, -315)});
    corpus.push_back({rightmostQuad(1), leftmostQuad(1)});
    corpus.push_back({rightmostQuad(0), leftmostQuad(0)});
    corpus.push_back({rightmostQuad(1), leftmostQuad(-99999)});
    corpus.push_back({rightmostQuad(-99999), allowedInterval(-315, 315)});
    corpus.push_back({rightmostQuad(-99999), allowedInterval(1, 1)});
    corpus.push_back({rightmostQuad(-99999), allowedSingleton(2)});
    corpus.push_back({leftmostQuad(-99999), allowedSingleton(-2)});
    corpus.push_back({rightmostQuad(-99999), irrationalRootsQuad(2)});
    corpus.push_back({halfRootsQuad(0), allowedInterval(-1, 1)});
    corpus.push_back({halfRootsQuad(0), allowedInterval(1, 2)});
    corpus.push_back({halfRootsQuad(158), allowedInterval(-315, 315)});
    corpus.push_back({halfRootsQuad(-158), allowedInterval(-315, 315)});

    for (Test &test : corpus)
    {
        shuffleWithRnd(test);
        addTest(std::move(test));
    }
}

void modeExhaustiveSmall()
{
    vector<Test> all;
    for (int a = -5; a <= -1; ++a)
        for (int b = -5; b <= 5; ++b)
            for (int c = -5; c <= 5; ++c)
                all.push_back({makeQuad(a, b, c)});
    shuffleWithRnd(all);
    for (Test &test : all)
        addTest(std::move(test));
}

void modePlantedTiny()
{
    const int T = 10000;
    for (int tc = 0; tc < T; ++tc)
    {
        int n = rnd.next(2, 20);
        int witness = rnd.next(-300, 300);
        bool answerYes = rnd.next(0, 1);
        Test test;
        test.reserve(n);
        test.push_back(allowedSingleton(witness));
        if (!answerYes)
        {
            // 再放一个只允许 witness+1 的式子，把 witness 挤掉
            switch (rnd.next(0, 2))
            {
            case 0: test.push_back(allowedSingleton(witness + 1)); break;
            case 1: test.push_back(integerRootsQuad(witness, witness + 1)); break;
            default: test.push_back(irrationalRootsQuad(witness + 1)); break;
            }
        }
        while ((int)test.size() < n)
            test.push_back(randomSatisfiedAt(witness));
        shuffleWithRnd(test);
        addTest(std::move(test));
    }
}

void modeStrictArithmetic()
{
    const int T = 3000;
    for (int tc = 0; tc < T; ++tc)
    {
        int x = rnd.next(-100, 100);
        Test test;
        test.push_back(allowedSingleton(x));
        switch (tc % 12)
        {
        case 0: test.push_back(halfRootsQuad(x)); break;                           // 根 x±1/2
        case 1: test.push_back(makeQuad(-1, 2LL * x, 1LL - 1LL * x * x)); break;   // 根 x±1
        case 2: test.push_back(irrationalRootsQuad(x - 1)); break;                 // {x-1,x}
        case 3: test.push_back(irrationalRootsQuad(x)); break;                     // {x,x+1}
        case 4: test.push_back(integerRootsQuad(x - 1, x)); break;                 // 空
        case 5: test.push_back(integerRootsQuad(x, x + 1)); break;                 // 空
        case 6: test.push_back(allowedInterval(x, x, 2)); break;                   // 放缩后的同一个点
        case 7: test.push_back(allowedInterval(x, x, 10)); break;
        case 8: test.push_back(allowedInterval(x - rnd.next(0, 3), x + rnd.next(0, 3))); break;
        case 9: test.push_back(makeQuad(-rnd.next(1, 5), 2LL * x * rnd.next(1, 5), 0)); break;
        case 10: test.push_back(makeQuad(-1, 2LL * x + 1, -1LL * x * x - x)); break; // 根 x, x+1
        default: test.push_back(makeQuad(-1, 2LL * x + 1, -1LL * x * x - x + 1)); break; // 根 x-0.618, x+1.618
        }
        int extra = rnd.next(0, 5);
        while (extra--)
            test.push_back(randomSatisfiedAt(x));
        shuffleWithRnd(test);
        addTest(std::move(test));
    }
}

void modeNestedIntervals()
{
    const int T = 2000;
    for (int tc = 0; tc < T; ++tc)
    {
        int l = rnd.next(-315, 0);
        int r = rnd.next(0, 315);
        bool keepOne = (tc % 2 == 0) || (l == r);
        int victim = keepOne ? rnd.next(l, r) : rnd.next(l, r - 1);

        Test test;
        test.push_back(allowedInterval(l, r));

        if (keepOne)
        {
            // 左右两端分别逐步向 victim 收紧，最终交集恰为 {victim}
            int step = rnd.next(10, 30);
            for (int c = l; c < victim; c += step)
                test.push_back(allowedInterval(min(c, victim), r));
            for (int d = r; d > victim; d -= step)
                test.push_back(allowedInterval(l, max(d, victim)));
            test.push_back(allowedInterval(l, victim));
            test.push_back(allowedInterval(victim, r));
            test.push_back(allowedSingleton(victim));
        }
        else
        {
            // 两条互不相交但各自非空的区间，交集为空
            test.push_back(allowedInterval(l, victim));
            test.push_back(allowedInterval(victim + 1, r));
        }

        int extra = rnd.next(5, 15);
        while (extra--)
        {
            if (rnd.next(0, 2) == 0)
                test.push_back(allowedInterval(max(-315, l - rnd.next(0, 10)),
                                               min(315, r + rnd.next(0, 10))));
            else
                test.push_back(randomSatisfiedAt(keepOne ? victim : l));
        }
        shuffleWithRnd(test);
        addTest(std::move(test));
    }
}

void modeMaximumYes()
{
    Test test;
    test.reserve(SUM_N_LIM);
    test.push_back(rightmostQuad(1));
    test.push_back(leftmostQuad(1));
    test.push_back(allowedInterval(-315, 315));
    test.push_back(makeQuad(-100000, 0, 100000));
    while ((int)test.size() < SUM_N_LIM)
    {
        switch (rnd.next(0, 4))
        {
        case 0: test.push_back(allowedInterval(rnd.next(-315, 0), rnd.next(0, 315))); break;
        case 1: test.push_back(randomSatisfiedAt(0)); break;
        case 2: test.push_back(irrationalRootsQuad(rnd.next(-1, 0))); break;
        case 3: test.push_back(integerRootsQuad(rnd.next(-315, -1), rnd.next(1, 315))); break;
        default:
        {
            int l = rnd.next(-315, 0), r = rnd.next(0, 315);
            test.push_back(allowedInterval(l, r, randomScale(l, r)));
            break;
        }
        }
    }
    addTest(std::move(test));
}

void modeMaximumNo()
{
    Test test;
    test.reserve(SUM_N_LIM);
    bool delayed = rnd.next(0, 1);

    // 逐步把可行域收窄到单点 -315
    for (int k = 315; k >= -315; --k)
        test.push_back(allowedInterval(-315, k));
    test.push_back(allowedSingleton(-315));

    // 一条与 -315 不相交（但自身非空）的区间，直接把交集清空
    Quad killer = allowedInterval(-314, 316);
    if (delayed)
    {
        while ((int)test.size() < SUM_N_LIM - 1)
            test.push_back(randomSatisfiedAt(-315));
        test.push_back(killer); // 破坏者放在最后
    }
    else
    {
        test.push_back(killer);
        while ((int)test.size() < SUM_N_LIM)
            test.push_back(randomSatisfiedAt(-315));
        shuffleWithRnd(test);
    }
    addTest(std::move(test));
}

void modeDegenerate()
{
    Test test;
    test.reserve(SUM_N_LIM);
    bool hasImpossible = rnd.next(0, 1);
    int killer = hasImpossible ? rnd.next(0, SUM_N_LIM - 1) : -1;
    for (int i = 0; i < SUM_N_LIM; ++i)
    {
        if (i == killer)
            test.push_back(makeQuad(-1, 0, -1)); // 判别式 < 0，直接不可能
        else if (rnd.next(0, 1))
            test.push_back(allowedInterval(-315, 315));
        else
            test.push_back(randomSatisfiedAt(0));
    }
    shuffleWithRnd(test);
    addTest(std::move(test));
}

void modeExtremeCoefficients()
{
    const int T = 600;
    for (int tc = 0; tc < T; ++tc)
    {
        Test test;
        switch (tc % 10)
        {
        case 0: test = {rightmostQuad(1), leftmostQuad(1)}; break;
        case 1: test = {rightmostQuad(0), leftmostQuad(0)}; break;
        case 2: test = {rightmostQuad(1), leftmostQuad(0)}; break;
        case 3: test = {rightmostQuad(0), leftmostQuad(1)}; break;
        case 4: test = {rightmostQuad(-99999), allowedInterval(-315, 315)}; break;
        case 5: test = {leftmostQuad(-99999), allowedInterval(-315, 315)}; break;
        case 6: test = {makeQuad(-99999, 100000, 0)}; break;
        case 7: test = {makeQuad(-99999, 100000, -1)}; break;
        case 8: test = {makeQuad(-100000, 100000, -1)}; break;
        default: test = {makeQuad(-100000, 100000, 100000)}; break;
        }
        int extra = rnd.next(0, 6);
        while (extra--)
        {
            switch (rnd.next(0, 2))
            {
            case 0: test.push_back(makeQuad(-rnd.next(1, COEF_LIM), rnd.next(-COEF_LIM, COEF_LIM), rnd.next(-COEF_LIM, COEF_LIM))); break;
            default: test.push_back(randomSatisfiedAt(rnd.next(0, 1) ? 0 : rnd.next(-316, 316))); break;
            }
        }
        shuffleWithRnd(test);
        addTest(std::move(test));
    }
}

void modeMaximumTestCount()
{
    const int T = 10000;
    const int N = 20;
    for (int tc = 0; tc < T; ++tc)
    {
        int witness = rnd.next(-300, 300);
        bool answerYes = (tc % 2 == 0);
        Test test;
        test.reserve(N);
        test.push_back(allowedSingleton(witness));
        if (!answerYes)
            test.push_back(rnd.next(0, 1) ? allowedSingleton(witness + 1)
                                          : integerRootsQuad(witness, witness + 1));
        while ((int)test.size() < N)
            test.push_back(randomSatisfiedAt(witness));
        shuffleWithRnd(test);
        addTest(std::move(test));
    }
}

void modeExtremeIntegerWitnesses()
{
    for (int witness : {100000, -100000})
    {
        Test test;
        test.reserve(100000);
        // 唯一能覆盖 ±10^5 的形式：a=-1, b=±10^5, c>0
        test.push_back(witness > 0 ? rightmostQuad(1) : leftmostQuad(1));
        while ((int)test.size() < 100000)
            test.push_back(extremeSatisfiedAt(witness));
        shuffleWithRnd(test);
        addTest(std::move(test));
    }
}
} // namespace

int main(int argc, char *argv[])
{
    registerGen(argc, argv, 1);
    int mode = opt<int>(1);

    if (mode == 1) modeHandmade();
    else if (mode == 2) modeExhaustiveSmall();
    else if (mode == 3) modePlantedTiny();
    else if (mode == 4) modeStrictArithmetic();
    else if (mode == 5) modeNestedIntervals();
    else if (mode == 6) modeMaximumYes();
    else if (mode == 7) modeMaximumNo();
    else if (mode == 8) modeDegenerate();
    else if (mode == 9) modeExtremeCoefficients();
    else if (mode == 10) modeMaximumTestCount();
    else if (mode == 11) modeExtremeIntegerWitnesses();
    else ensuref(false, "unknown mode: %d", mode);

    printTests();
    return 0;
}
