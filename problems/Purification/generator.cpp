#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// Usage: generator <mode> <seed> > output.in

namespace
{
const int LIM = 1000000000; // 1e9
const int MAX_N = 200000;
const int MAX_T = 100000;

using Point = pair<int, int>;
using Test = vector<Point>;

vector<Test> tests;
int totalN = 0;

template <class T>
void shuffleWithRnd(vector<T> &v)
{
    for (int i = (int)v.size() - 1; i > 0; --i)
        swap(v[i], v[rnd.next(0, i)]);
}

void addTest(Test test)
{
    ensuref(!test.empty(), "a test case must not be empty");
    ensuref((int)test.size() <= MAX_N, "n exceeds 200000");
    ensuref((int)tests.size() < MAX_T, "T exceeds 100000");

    for (const Point &p : test)
    {
        ensuref(-LIM <= p.first && p.first <= LIM, "x=%d is out of range", p.first);
        ensuref(-LIM <= p.second && p.second <= LIM, "y=%d is out of range", p.second);
    }

    Test sorted = test;
    sort(sorted.begin(), sorted.end());
    for (size_t i = 1; i < sorted.size(); ++i)
        ensuref(sorted[i - 1] != sorted[i], "duplicate cannon position (%d, %d)",
                sorted[i].first, sorted[i].second);

    totalN += (int)test.size();
    ensuref(totalN <= 200000, "sum of n exceeds 200000");
    tests.push_back(std::move(test));
}

void printTests()
{
    cout << tests.size() << '\n';
    for (const Test &test : tests)
    {
        cout << test.size() << '\n';
        for (const Point &p : test)
            cout << p.first << ' ' << p.second << '\n';
    }
}

// n distinct points drawn uniformly from [xmin,xmax] x [ymin,ymax].
Test randomDistinct(int n, int xmin, int xmax, int ymin, int ymax)
{
    ensuref((long long)(xmax - xmin + 1) * (ymax - ymin + 1) >= n,
            "coordinate range too small for %d distinct points", n);
    Test test;
    test.reserve(n);
    set<Point> seen;
    while ((int)test.size() < n)
    {
        Point p = {rnd.next(xmin, xmax), rnd.next(ymin, ymax)};
        if (seen.insert(p).second)
            test.push_back(p);
    }
    return test;
}

// 8 points forming a directed 4-cycle of "middle -> endpoint" constraints.
Test makePinwheel(int bx, int by)
{
    ensuref(-LIM <= bx - 1 && bx + 2 <= LIM && -LIM <= by - 1 && by + 2 <= LIM,
            "pinwheel base out of range");
    return {{bx + 1, by + 0},
            {bx + 0, by + 0},
            {bx + 0, by + 1},
            {bx + 1, by + 1},
            {bx + 2, by + 0},
            {bx + 0, by - 1},
            {bx - 1, by + 1},
            {bx + 1, by + 2}};
}

// Monotone staircase of overlapping 3-lines: always acyclic, hence YES.
Test makeStaircase(int segments)
{
    Test t;
    int x = 0, y = 0;
    t.push_back({x, y});
    t.push_back({x + 1, y});
    x += 2;
    t.push_back({x, y});
    for (int i = 0; i < segments; ++i)
    {
        if (i % 2 == 0)
        {
            t.push_back({x, y + 1});
            t.push_back({x, y + 2});
            y += 2;
        }
        else
        {
            t.push_back({x + 1, y});
            t.push_back({x + 2, y});
            x += 2;
        }
    }
    return t;
}

void modeSample()
{
    addTest({{0, 0}, {0, 1}, {0, 2}});
    addTest({{0, 0}, {0, 1}, {0, 2}, {1, 1}, {2, 1}});
    addTest({{0, 0}, {1, 0}, {2, 0}, {3, 0}});
}

// Every non-empty subset of a 3x3 grid (511 tests, no line reaches 4).
void modeTinyExhaustive()
{
    vector<Point> grid;
    for (int x = 0; x <= 2; ++x)
        for (int y = 0; y <= 2; ++y)
            grid.push_back({x, y});
    int cells = (int)grid.size(); // 9
    for (int mask = 1; mask < (1 << cells); ++mask)
    {
        Test test;
        for (int i = 0; i < cells; ++i)
            if (mask >> i & 1)
                test.push_back(grid[i]);
        addTest(test);
    }
}

// Single lines of length 1..6 in both orientations, plus a plus-sign.
void modeLineStructures()
{
    for (int k = 1; k <= 6; ++k)
    {
        Test hor, ver;
        for (int i = 0; i < k; ++i)
        {
            hor.push_back({i, 0});
            ver.push_back({0, i});
        }
        addTest(hor); // k<=3 YES, k>=4 NO
        addTest(ver);
    }
    addTest({{0, 0}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}});
    addTest({{0, 0}, {1, 0}, {2, 0}, {100, 100}, {101, 100}, {102, 100}});

    // 4x3 grid: each column has 4 points -> NO.
    Test grid43;
    for (int x = 0; x <= 2; ++x)
        for (int y = 0; y <= 3; ++y)
            grid43.push_back({x, y});
    addTest(grid43);

    // 3-lines pinned to the coordinate extremes.
    addTest({{LIM, 0}, {LIM, 1}, {LIM, 2}});     // YES
    addTest({{-LIM, 0}, {-LIM, 1}, {-LIM, 2}});  // YES
    addTest({{0, LIM}, {1, LIM}, {2, LIM}, {3, LIM}}); // NO (4 in a row)
}

// Planted NO configurations whose constraint graph has a cycle.
void modeCycles()
{
    addTest(makePinwheel(0, 0));
    addTest(makePinwheel(500000000, 500000000));
    addTest(makePinwheel(-500000000, -500000000));

    Test two = makePinwheel(0, 0);
    Test other = makePinwheel(1000, 1000);
    two.insert(two.end(), other.begin(), other.end());
    shuffleWithRnd(two);
    addTest(two);
}

// Guaranteed-YES configurations with non-trivial cross-line structure.
void modePlantedYes()
{
    Test grid;
    for (int x = 0; x <= 2; ++x)
        for (int y = 0; y <= 2; ++y)
            grid.push_back({x, y});
    addTest(grid); // 9 points, full 3x3 grid

    Test ring;
    for (int x = 0; x <= 2; ++x)
        for (int y = 0; y <= 2; ++y)
            if (!(x == 1 && y == 1))
                ring.push_back({x, y});
    addTest(ring); // 8 points, 3x3 ring without centre

    addTest(makeStaircase(50)); // 103 points, monotone chain

    Test lines;
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 3; ++j)
            lines.push_back({i * 3 + j, i * 7});
    shuffleWithRnd(lines);
    addTest(lines); // 5 disjoint 3-lines (distinct rows and columns)
}

void modeSmallRandom()
{
    const int T = 200;
    for (int tc = 0; tc < T; ++tc)
    {
        int n = rnd.next(1, 30);
        Test test = randomDistinct(n, -15, 15, -15, 15);
        shuffleWithRnd(test);
        addTest(test);
    }
}

void modeLargeRandom()
{
    Test test = randomDistinct(MAX_N, -LIM, LIM, -LIM, LIM);
    shuffleWithRnd(test);
    addTest(test);
}

void modeMaxT()
{
    const int T = MAX_T;
    for (int tc = 0; tc < T; ++tc)
        addTest({{tc, 0}, {tc, 1}}); // n = 2, sum n = 2e5
}

void modeMaxLine()
{
    Test test;
    test.reserve(MAX_N);
    for (int i = 0; i < MAX_N; ++i)
        test.push_back({i, 0});
    shuffleWithRnd(test);
    addTest(test); // single line of 2e5 -> NO
}

void modeMaxDAG()
{
    Test test;
    int lines = MAX_N / 3; // 66666 disjoint 3-lines
    test.reserve(lines * 3);
    for (int i = 0; i < lines; ++i)
    {
        test.push_back({3 * i, i});
        test.push_back({3 * i + 1, i});
        test.push_back({3 * i + 2, i});
    }
    shuffleWithRnd(test);
    addTest(test); // acyclic DAG with 133332 edges -> YES
}
} // namespace

int main(int argc, char *argv[])
{
    registerGen(argc, argv, 1);
    int mode = opt<int>(1);

    if (mode == 1) modeSample();
    else if (mode == 2) modeTinyExhaustive();
    else if (mode == 3) modeLineStructures();
    else if (mode == 4) modeCycles();
    else if (mode == 5) modePlantedYes();
    else if (mode == 6) modeSmallRandom();
    else if (mode == 7) modeLargeRandom();
    else if (mode == 8) modeMaxT();
    else if (mode == 9) modeMaxLine();
    else if (mode == 10) modeMaxDAG();
    else ensuref(false, "unknown mode: %d", mode);

    printTests();
    return 0;
}
