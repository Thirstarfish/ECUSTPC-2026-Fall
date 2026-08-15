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
    ensuref(a != 0, "the leading coefficient must be non-zero");
    ensuref(-COEF_LIM <= a && a <= COEF_LIM, "a=%lld is out of range", a);
    ensuref(-COEF_LIM <= b && b <= COEF_LIM, "b=%lld is out of range", b);
    ensuref(-COEF_LIM <= c && c <= COEF_LIM, "c=%lld is out of range", c);
    return {(int)a, (int)b, (int)c};
}

// (x-p)(x-q)>0: the forbidden integer interval is [p,q].
Quad forbiddenInterval(int p, int q, int scale = 1)
{
    ensuref(p <= q && scale >= 1, "invalid forbidden interval");
    return makeQuad(scale, -1LL * scale * (p + q), 1LL * scale * p * q);
}

// -(x-(l-1))(x-(r+1))>0: the allowed integer interval is [l,r].
Quad allowedInterval(int l, int r, int scale = 1)
{
    ensuref(l <= r && scale >= 1, "invalid allowed interval");
    long long p = l - 1LL;
    long long q = r + 1LL;
    return makeQuad(-scale, scale * (p + q), -1LL * scale * p * q);
}

Quad allowedSingleton(int x)
{
    return allowedInterval(x, x);
}

Quad randomSatisfiedAt(int x)
{
    for (int attempt = 0; attempt < 10000; ++attempt)
    {
        int a = 0;
        while (a == 0)
            a = rnd.next(-COEF_LIM, COEF_LIM);
        int b = rnd.next(-COEF_LIM, COEF_LIM);
        long long base = 1LL * a * x * x + 1LL * b * x;
        long long low = max(-1LL * COEF_LIM, 1 - base);
        if (low <= COEF_LIM)
            return makeQuad(a, b, rnd.next((int)low, COEF_LIM));
    }
    return makeQuad(1, 0, 1);
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
    corpus.push_back({makeQuad(1, 2, -3)});
    corpus.push_back({makeQuad(-1, 0, 0)});
    corpus.push_back({makeQuad(-1, 2, -1)});
    corpus.push_back({makeQuad(-1, 0, 1)});
    corpus.push_back({makeQuad(-1, 1, 0)});
    corpus.push_back({makeQuad(-5, 5, -1)});
    corpus.push_back({makeQuad(5, -5, 1)});
    corpus.push_back({makeQuad(4, -4, 1)});
    corpus.push_back({makeQuad(1, -100000, -100000), makeQuad(1, 100000, -100000)});
    corpus.push_back({allowedSingleton(0), forbiddenInterval(0, 0)});

    corpus.push_back({allowedInterval(0, 3), forbiddenInterval(0, 1),
                      forbiddenInterval(2, 3)});
    corpus.push_back({allowedInterval(0, 4), forbiddenInterval(0, 1),
                      forbiddenInterval(3, 4)});
    corpus.push_back({allowedInterval(0, 20), forbiddenInterval(0, 10),
                      forbiddenInterval(2, 3), forbiddenInterval(11, 20)});
    corpus.push_back({allowedInterval(-10, 0), allowedInterval(0, 10)});
    corpus.push_back({allowedInterval(-10, -1), allowedInterval(0, 10)});
    corpus.push_back({allowedInterval(-20, 20), forbiddenInterval(-20, -1),
                      forbiddenInterval(1, 20)});
    corpus.push_back({allowedInterval(-20, 20), forbiddenInterval(-20, 0),
                      forbiddenInterval(1, 20)});

    corpus.push_back({allowedSingleton(-1), makeQuad(1, 100000, 99998)});
    corpus.push_back({allowedSingleton(-1), makeQuad(1, 100000, 99999)});
    corpus.push_back({allowedSingleton(-1), makeQuad(1, 100000, 100000)});
    corpus.push_back({allowedSingleton(1), makeQuad(1, -100000, 99998)});
    corpus.push_back({allowedSingleton(1), makeQuad(1, -100000, 99999)});
    corpus.push_back({allowedSingleton(1), makeQuad(1, -100000, 100000)});
    corpus.push_back({allowedSingleton(1), makeQuad(-1, 100000, -99998)});
    corpus.push_back({allowedSingleton(1), makeQuad(-1, 100000, -99999)});

    corpus.push_back({allowedSingleton(0), makeQuad(-100000, 100000, 100000)});
    corpus.push_back({allowedSingleton(0), makeQuad(100000, 100000, -100000)});
    corpus.push_back({allowedSingleton(-2), makeQuad(25000, 100000, 100000)});
    corpus.push_back({allowedSingleton(0), makeQuad(100000, -100000, 25000)});
    corpus.push_back({allowedSingleton(-2), makeQuad(25000, 100000, 99999)});
    corpus.push_back({allowedSingleton(0), makeQuad(25000, 99999, 100000)});
    corpus.push_back({makeQuad(-1, 99999, 100000)});

    Test positiveExtreme = {makeQuad(-1, 100000, 100000),
                            forbiddenInterval(0, 99999),
                            makeQuad(100000, 0, -100000)};
    corpus.push_back(positiveExtreme);
    positiveExtreme.push_back(forbiddenInterval(0, 100000));
    corpus.push_back(positiveExtreme);

    Test negativeExtreme = {makeQuad(-1, -100000, 100000),
                            forbiddenInterval(-99999, 0),
                            makeQuad(100000, 0, -100000)};
    corpus.push_back(negativeExtreme);
    negativeExtreme.push_back(forbiddenInterval(-100000, 0));
    corpus.push_back(negativeExtreme);

    for (Test &test : corpus)
    {
        shuffleWithRnd(test);
        addTest(std::move(test));
    }
}

void modeExhaustiveSmall()
{
    vector<Test> all;
    for (int a = -5; a <= 5; ++a)
    {
        if (a == 0)
            continue;
        for (int b = -5; b <= 5; ++b)
            for (int c = -5; c <= 5; ++c)
                all.push_back({makeQuad(a, b, c)});
    }
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
            test.push_back(forbiddenInterval(witness, witness));
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
        Test test;
        int witness = 0;
        switch (tc % 12)
        {
        case 0: witness = -1; test = {allowedSingleton(witness), makeQuad(1, 100000, 99998)}; break;
        case 1: witness = -1; test = {allowedSingleton(witness), makeQuad(1, 100000, 99999)}; break;
        case 2: witness = -1; test = {allowedSingleton(witness), makeQuad(1, 100000, 100000)}; break;
        case 3: witness = 1; test = {allowedSingleton(witness), makeQuad(1, -100000, 99998)}; break;
        case 4: witness = 1; test = {allowedSingleton(witness), makeQuad(1, -100000, 99999)}; break;
        case 5: witness = 1; test = {allowedSingleton(witness), makeQuad(1, -100000, 100000)}; break;
        case 6: witness = 0; test = {allowedSingleton(witness), makeQuad(-100000, 100000, 100000)}; break;
        case 7: witness = 0; test = {allowedSingleton(witness), makeQuad(100000, 100000, -100000)}; break;
        case 8: witness = -2; test = {allowedSingleton(witness), makeQuad(25000, 100000, 100000)}; break;
        case 9: witness = 0; test = {allowedSingleton(witness), makeQuad(100000, -100000, 25000)}; break;
        case 10: witness = -2; test = {allowedSingleton(witness), makeQuad(25000, 100000, 99999)}; break;
        default: witness = 0; test = {allowedSingleton(witness), makeQuad(25000, 99999, 100000)}; break;
        }
        int extra = rnd.next(0, 6);
        while (extra--)
            test.push_back(randomSatisfiedAt(witness));
        shuffleWithRnd(test);
        addTest(std::move(test));
    }
}

void addCoverage(Test &test, vector<Quad> &segments, int lo, int hi)
{
    int current = lo;
    while (current <= hi)
    {
        int len = rnd.next(10, 30);
        int right = min(hi, current + len - 1);
        Quad q = forbiddenInterval(current, right);
        test.push_back(q);
        segments.push_back(q);
        current = right + 1;
    }
}

void modeIntervalUnions()
{
    const int T = 2500;
    for (int tc = 0; tc < T; ++tc)
    {
        int l = rnd.next(-250, -50);
        int r = rnd.next(50, 250);
        int hole = rnd.next(l, r);
        bool leaveHole = (tc % 2 == 0);

        Test test;
        vector<Quad> segments;
        test.push_back(allowedInterval(l, r));
        if (leaveHole)
        {
            if (l <= hole - 1)
                addCoverage(test, segments, l, hole - 1);
            if (hole + 1 <= r)
                addCoverage(test, segments, hole + 1, r);
        }
        else
        {
            addCoverage(test, segments, l, r);
        }

        int extra = rnd.next(5, 15);
        while (extra--)
        {
            int kind = rnd.next(0, 2);
            if (kind == 0 && !segments.empty())
                test.push_back(segments[rnd.next(0, (int)segments.size() - 1)]);
            else if (kind == 1)
                test.push_back(allowedInterval(l - rnd.next(0, 20), r + rnd.next(0, 20)));
            else if (leaveHole)
                test.push_back(randomSatisfiedAt(hole));
            else
                test.push_back(makeQuad(rnd.next(1, COEF_LIM), 0, rnd.next(1, COEF_LIM)));
        }
        shuffleWithRnd(test);
        addTest(std::move(test));
    }
}

void modeAllUpwardMaximum()
{
    Test test;
    test.reserve(SUM_N_LIM);
    test.push_back(makeQuad(1, -100000, -100000));
    test.push_back(makeQuad(1, 100000, -100000));
    while ((int)test.size() < SUM_N_LIM)
        test.push_back(makeQuad(rnd.next(1, COEF_LIM),
                                rnd.next(-COEF_LIM, COEF_LIM),
                                rnd.next(-COEF_LIM, COEF_LIM)));
    shuffleWithRnd(test);
    addTest(std::move(test));
}

void modeDelayedBlocker()
{
    Test test;
    test.reserve(SUM_N_LIM);
    test.push_back(makeQuad(-1, 100000, 100000));
    while ((int)test.size() + 1 < SUM_N_LIM)
    {
        int scale = rnd.next(1, COEF_LIM);
        test.push_back(makeQuad(scale, 0, scale));
    }
    test.push_back(makeQuad(1, -100000, -100000));
    addTest(std::move(test));
}

void modeMaximumCovered()
{
    Test test;
    test.reserve(SUM_N_LIM);
    test.push_back(makeQuad(-1, 0, 99856));
    test.push_back(forbiddenInterval(-315, -200));
    test.push_back(forbiddenInterval(-199, -1));
    test.push_back(forbiddenInterval(0, 100));
    test.push_back(forbiddenInterval(101, 315));
    while ((int)test.size() < SUM_N_LIM)
    {
        int p = rnd.next(-315, 315);
        int q = rnd.next(p, 315);
        test.push_back(forbiddenInterval(p, q));
    }
    shuffleWithRnd(test);
    addTest(std::move(test));
}

void modeMaximumOneHole()
{
    int hole = rnd.next(-300, 300);
    Test test;
    test.reserve(SUM_N_LIM);
    test.push_back(makeQuad(-1, 0, 99856));
    test.push_back(allowedSingleton(hole));
    if (hole > -315)
        test.push_back(forbiddenInterval(-315, hole - 1));
    if (hole < 315)
        test.push_back(forbiddenInterval(hole + 1, 315));
    while ((int)test.size() < SUM_N_LIM)
        test.push_back(randomSatisfiedAt(hole));
    shuffleWithRnd(test);
    addTest(std::move(test));
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
            test.push_back(forbiddenInterval(witness, witness));
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
        if (witness > 0)
        {
            test.push_back(makeQuad(-1, 100000, 100000));
            test.push_back(forbiddenInterval(0, 99999));
        }
        else
        {
            test.push_back(makeQuad(-1, -100000, 100000));
            test.push_back(forbiddenInterval(-99999, 0));
        }
        while ((int)test.size() < 100000)
            test.push_back(randomSatisfiedAt(witness));
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
    else if (mode == 5) modeIntervalUnions();
    else if (mode == 6) modeAllUpwardMaximum();
    else if (mode == 7) modeDelayedBlocker();
    else if (mode == 8) modeMaximumCovered();
    else if (mode == 9) modeMaximumOneHole();
    else if (mode == 10) modeMaximumTestCount();
    else if (mode == 11) modeExtremeIntegerWitnesses();
    else ensuref(false, "unknown mode: %d", mode);

    printTests();
    return 0;
}
