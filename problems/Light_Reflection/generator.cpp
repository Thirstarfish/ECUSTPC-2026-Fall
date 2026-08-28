#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// Usage: generator <mode> <seed> > output.in

namespace
{
const int MAX_TESTS = 100;
const int MAX_CELLS = 200000;

enum Port
{
    U_PORT = 0,
    R_PORT = 1,
    D_PORT = 2,
    L_PORT = 3
};

enum Expected
{
    ANY = -1,
    NO = 0,
    YES = 1
};

struct Endpoint
{
    char side;
    int position;
};

struct TestCase
{
    int n, m;
    vector<string> grid;
    Endpoint start, target;
};

struct TraceResult
{
    bool exited;
    Endpoint endpoint;
    vector<unsigned char> usedCell;
};

vector<TestCase> tests;
int totalCells = 0;

bool sameEndpoint(const Endpoint &a, const Endpoint &b)
{
    return a.side == b.side && a.position == b.position;
}

int boundaryCount(int n, int m)
{
    return 2 * (n + m);
}

Endpoint endpointByIndex(int n, int m, int index)
{
    ensuref(0 <= index && index < boundaryCount(n, m), "invalid endpoint index");
    if (index < m)
        return {'U', index + 1};
    index -= m;
    if (index < m)
        return {'D', index + 1};
    index -= m;
    if (index < n)
        return {'L', index + 1};
    index -= n;
    return {'R', index + 1};
}

Endpoint randomEndpoint(int n, int m)
{
    return endpointByIndex(n, m, rnd.next(0, boundaryCount(n, m) - 1));
}

Endpoint randomEndpointExcept(int n, int m, const vector<Endpoint> &excluded)
{
    int count = boundaryCount(n, m);
    int offset = rnd.next(0, count - 1);
    for (int delta = 0; delta < count; ++delta)
    {
        Endpoint candidate = endpointByIndex(n, m, (offset + delta) % count);
        bool allowed = true;
        for (const Endpoint &endpoint : excluded)
            allowed &= !sameEndpoint(candidate, endpoint);
        if (allowed)
            return candidate;
    }
    ensuref(false, "no boundary endpoint remains");
    return {'U', 1};
}

int endpointNode(const TestCase &test, const Endpoint &endpoint)
{
    int row, column, port;
    if (endpoint.side == 'U')
        row = 0, column = endpoint.position - 1, port = U_PORT;
    else if (endpoint.side == 'D')
        row = test.n - 1, column = endpoint.position - 1, port = D_PORT;
    else if (endpoint.side == 'L')
        row = endpoint.position - 1, column = 0, port = L_PORT;
    else
        row = endpoint.position - 1, column = test.m - 1, port = R_PORT;
    return 4 * (row * test.m + column) + port;
}

bool relaxedFeasible(const TestCase &test)
{
    int source = endpointNode(test, test.start);
    int target = endpointNode(test, test.target);
    vector<unsigned char> visited(4 * test.n * test.m, 0);
    queue<int> bfs;
    visited[source] = 1;
    bfs.push(source);

    auto push = [&](int node) {
        if (!visited[node])
        {
            visited[node] = 1;
            bfs.push(node);
        }
    };

    static const int slashPair[] = {L_PORT, D_PORT, R_PORT, U_PORT};
    static const int backslashPair[] = {R_PORT, U_PORT, L_PORT, D_PORT};

    while (!bfs.empty())
    {
        int node = bfs.front();
        bfs.pop();
        if (node == target)
            return true;

        int port = node % 4;
        int cell = node / 4;
        int row = cell / test.m;
        int column = cell % test.m;
        char value = test.grid[row][column];

        if (value == '.')
        {
            for (int nextPort = 0; nextPort < 4; ++nextPort)
                if (nextPort != port)
                    push(4 * cell + nextPort);
        }
        else
        {
            int nextPort = value == '/' ? slashPair[port] : backslashPair[port];
            push(4 * cell + nextPort);
        }

        if (port == U_PORT && row > 0)
            push(4 * ((row - 1) * test.m + column) + D_PORT);
        else if (port == R_PORT && column + 1 < test.m)
            push(4 * (row * test.m + column + 1) + L_PORT);
        else if (port == D_PORT && row + 1 < test.n)
            push(4 * ((row + 1) * test.m + column) + U_PORT);
        else if (port == L_PORT && column > 0)
            push(4 * (row * test.m + column - 1) + R_PORT);
    }
    return false;
}

TraceResult traceCompletedGrid(const vector<string> &grid, const Endpoint &start)
{
    int n = (int)grid.size();
    int m = (int)grid[0].size();
    int row, column, direction;
    if (start.side == 'U')
        row = 0, column = start.position - 1, direction = 2;
    else if (start.side == 'D')
        row = n - 1, column = start.position - 1, direction = 0;
    else if (start.side == 'L')
        row = start.position - 1, column = 0, direction = 1;
    else
        row = start.position - 1, column = m - 1, direction = 3;

    static const int dr[] = {-1, 0, 1, 0};
    static const int dc[] = {0, 1, 0, -1};
    vector<unsigned char> seen(4 * n * m, 0);
    vector<unsigned char> usedCell(n * m, 0);

    while (0 <= row && row < n && 0 <= column && column < m)
    {
        int state = 4 * (row * m + column) + direction;
        if (seen[state])
            return {false, {'U', 1}, std::move(usedCell)};
        seen[state] = 1;
        usedCell[row * m + column] = 1;

        if (grid[row][column] == '/')
            direction ^= 1;
        else if (grid[row][column] == '\\')
            direction ^= 3;
        row += dr[direction];
        column += dc[direction];
    }

    Endpoint endpoint;
    if (row < 0)
        endpoint = {'U', column + 1};
    else if (row >= n)
        endpoint = {'D', column + 1};
    else if (column < 0)
        endpoint = {'L', row + 1};
    else
        endpoint = {'R', row + 1};
    return {true, endpoint, std::move(usedCell)};
}

bool bruteFeasible(TestCase test)
{
    vector<pair<int, int>> freeCells;
    for (int row = 0; row < test.n; ++row)
        for (int column = 0; column < test.m; ++column)
            if (test.grid[row][column] == '.')
                freeCells.push_back({row, column});
    ensuref((int)freeCells.size() <= 10, "too many cells for brute force");

    static const char choices[] = {'.', '/', '\\'};
    function<bool(int)> dfs = [&](int index) {
        if (index == (int)freeCells.size())
        {
            TraceResult trace = traceCompletedGrid(test.grid, test.start);
            return trace.exited && sameEndpoint(trace.endpoint, test.target);
        }
        auto [row, column] = freeCells[index];
        for (char value : choices)
        {
            test.grid[row][column] = value;
            if (dfs(index + 1))
                return true;
        }
        test.grid[row][column] = '.';
        return false;
    };
    return dfs(0);
}

void validateCase(const TestCase &test)
{
    ensuref(1 <= test.n && 1 <= test.m, "grid dimensions must be positive");
    ensuref(1LL * test.n * test.m <= MAX_CELLS, "n*m exceeds 200000");
    ensuref((int)test.grid.size() == test.n, "wrong number of grid rows");
    for (const string &row : test.grid)
    {
        ensuref((int)row.size() == test.m, "wrong grid row length");
        for (char value : row)
            ensuref(value == '.' || value == '/' || value == '\\',
                    "invalid grid character");
    }

    auto validEndpoint = [&](const Endpoint &endpoint) {
        if (endpoint.side == 'U' || endpoint.side == 'D')
            return 1 <= endpoint.position && endpoint.position <= test.m;
        if (endpoint.side == 'L' || endpoint.side == 'R')
            return 1 <= endpoint.position && endpoint.position <= test.n;
        return false;
    };
    ensuref(validEndpoint(test.start), "invalid start endpoint");
    ensuref(validEndpoint(test.target), "invalid target endpoint");
    ensuref(!sameEndpoint(test.start, test.target),
            "entry and target endpoints must be different");
}

void addTest(TestCase test, Expected expected = ANY)
{
    validateCase(test);
    bool feasible = relaxedFeasible(test);
    if (expected != ANY)
        ensuref(feasible == (expected == YES),
                "constructed case has unexpected feasibility");

    ensuref((int)tests.size() < MAX_TESTS, "T exceeds 100");
    totalCells += test.n * test.m;
    ensuref(totalCells <= MAX_CELLS, "sum of n*m exceeds 200000");
    tests.push_back(std::move(test));
}

TestCase makeCase(vector<string> grid, Endpoint start, Endpoint target)
{
    return {(int)grid.size(), (int)grid[0].size(), std::move(grid), start, target};
}

vector<string> completedPattern(int n, int m, int style)
{
    vector<string> grid(n, string(m, '/'));
    for (int row = 0; row < n; ++row)
        for (int column = 0; column < m; ++column)
        {
            if (style == 0)
                grid[row][column] = rnd.next(0, 1) ? '/' : '\\';
            else if (style == 1)
                grid[row][column] = '/';
            else if (style == 2)
                grid[row][column] = '\\';
            else if (style == 3)
                grid[row][column] = (row + column) % 2 ? '/' : '\\';
            else if (style == 4)
                grid[row][column] = column % 2 ? '/' : '\\';
            else
                grid[row][column] = row % 2 ? '/' : '\\';
        }
    return grid;
}

vector<string> randomInitialGrid(int n, int m, int freePercent)
{
    vector<string> grid(n, string(m, '.'));
    for (string &row : grid)
        for (char &value : row)
        {
            int roll = rnd.next(1, 100);
            if (roll <= freePercent)
                value = '.';
            else
                value = rnd.next(0, 1) ? '/' : '\\';
        }
    return grid;
}

TestCase fixedOutcomeCase(vector<string> completed, Endpoint start, bool wantYes)
{
    int n = (int)completed.size();
    int m = (int)completed[0].size();
    TraceResult trace = traceCompletedGrid(completed, start);
    ensuref(trace.exited, "completed grid unexpectedly cycles");
    Endpoint target = wantYes
                          ? trace.endpoint
                          : randomEndpointExcept(n, m, {start, trace.endpoint});
    return makeCase(std::move(completed), start, target);
}

TestCase plantedCase(int n, int m, int freePercent, bool wantYes, int style = 0)
{
    vector<string> completed = completedPattern(n, m, style);
    Endpoint start = randomEndpoint(n, m);
    TraceResult trace = traceCompletedGrid(completed, start);
    ensuref(trace.exited, "completed grid unexpectedly cycles");

    vector<string> initial = completed;
    for (int row = 0; row < n; ++row)
        for (int column = 0; column < m; ++column)
        {
            int cell = row * m + column;
            bool mayBecomeFree = wantYes || !trace.usedCell[cell];
            if (mayBecomeFree && rnd.next(1, 100) <= freePercent)
                initial[row][column] = '.';
        }

    Endpoint target = wantYes
                          ? trace.endpoint
                          : randomEndpointExcept(n, m, {start, trace.endpoint});
    return makeCase(std::move(initial), start, target);
}

TestCase randomCaseWithOutcome(int n, int m, bool wantYes)
{
    for (int attempt = 0; attempt < 200; ++attempt)
    {
        int freePercent = wantYes ? rnd.next(25, 80) : rnd.next(0, 25);
        vector<string> grid = randomInitialGrid(n, m, freePercent);
        Endpoint start = randomEndpoint(n, m);
        Endpoint target = randomEndpointExcept(n, m, {start});
        TestCase test = makeCase(std::move(grid), start, target);
        if (relaxedFeasible(test) == wantYes)
            return test;
    }
    return plantedCase(n, m, wantYes ? 70 : 30, wantYes);
}

void printTests()
{
    cout << tests.size() << '\n';
    for (const TestCase &test : tests)
    {
        cout << test.n << ' ' << test.m << '\n';
        for (const string &row : test.grid)
            cout << row << '\n';
        cout << test.start.side << ' ' << test.start.position << ' '
             << test.target.side << ' ' << test.target.position << '\n';
    }
}

void modeHandmade()
{
    for (char value : string("./\\"))
        for (int sourceIndex = 0; sourceIndex < 4; ++sourceIndex)
            for (int targetIndex = 0; targetIndex < 4; ++targetIndex)
            {
                if (sourceIndex == targetIndex)
                    continue;
                Endpoint start = endpointByIndex(1, 1, sourceIndex);
                Endpoint target = endpointByIndex(1, 1, targetIndex);
                TestCase test = makeCase({string(1, value)}, start, target);
                bool answer = value == '.' || sameEndpoint(
                    traceCompletedGrid(test.grid, start).endpoint, target);
                addTest(std::move(test), answer ? YES : NO);
            }

    addTest(makeCase({"..", ".."}, {'L', 1}, {'U', 2}), YES);
    addTest(makeCase({"..", ".."}, {'U', 1}, {'U', 2}), YES);
    // A valid route uses both internal pairs of the fixed bottom-right cell.
    addTest(makeCase({"..", ".\\"}, {'D', 2}, {'R', 2}), YES);
    addTest(makeCase({"...."}, {'L', 1}, {'R', 1}), YES);
    addTest(makeCase({".../"}, {'L', 1}, {'R', 1}), NO);
    addTest(makeCase({".", ".", ".", "/"}, {'U', 1}, {'D', 1}), NO);

    vector<vector<string>> fixedGrids = {
        {"/\\/", "\\//", "//\\"},
        {"////", "////", "////"},
        {"\\\\\\\\", "\\\\\\\\", "\\\\\\\\"},
        {"/\\/\\", "\\/\\/", "/\\/\\"}};
    vector<Endpoint> starts = {{'L', 2}, {'U', 4}, {'R', 1}, {'D', 3}};
    for (int i = 0; i < (int)fixedGrids.size(); ++i)
    {
        addTest(fixedOutcomeCase(fixedGrids[i], starts[i], true), YES);
        addTest(fixedOutcomeCase(fixedGrids[i], starts[i], false), NO);
    }
}

void modeTinyBrute()
{
    for (int tc = 0; tc < MAX_TESTS; ++tc)
    {
        bool wantYes = tc % 2 == 0;
        TestCase chosen;
        bool found = false;
        for (int attempt = 0; attempt < 1000 && !found; ++attempt)
        {
            int n = rnd.next(1, 3);
            int m = rnd.next(1, 3);
            if (n * m > 6)
                continue;
            int freePercent = wantYes ? rnd.next(35, 90) : rnd.next(0, 35);
            vector<string> grid = randomInitialGrid(n, m, freePercent);
            Endpoint start = randomEndpoint(n, m);
            Endpoint target = randomEndpointExcept(n, m, {start});
            TestCase candidate = makeCase(std::move(grid), start, target);
            if (relaxedFeasible(candidate) == wantYes)
                chosen = std::move(candidate), found = true;
        }
        ensuref(found, "failed to find a tiny case with requested outcome");
        bool graphAnswer = relaxedFeasible(chosen);
        bool bruteAnswer = bruteFeasible(chosen);
        ensuref(graphAnswer == bruteAnswer,
                "relaxed graph disagrees with exhaustive filling");
        addTest(std::move(chosen), wantYes ? YES : NO);
    }
}

void modePlantedVariable()
{
    static const int percentages[] = {1, 5, 20, 50, 80, 100};
    for (int tc = 0; tc < MAX_TESTS; ++tc)
    {
        int n = rnd.next(2, 30);
        int m = rnd.next(2, 30);
        bool wantYes = tc % 2 == 0;
        int freePercent = percentages[tc % 6];
        addTest(plantedCase(n, m, freePercent, wantYes, tc % 6),
                wantYes ? YES : NO);
    }
}

void modeAllFixedPatterns()
{
    for (int tc = 0; tc < MAX_TESTS; ++tc)
    {
        int n = rnd.next(1, 35);
        int m = rnd.next(1, 35);
        vector<string> grid = completedPattern(n, m, tc % 6);
        Endpoint start = endpointByIndex(n, m, tc % boundaryCount(n, m));
        bool wantYes = tc % 2 == 0;
        addTest(fixedOutcomeCase(std::move(grid), start, wantYes),
                wantYes ? YES : NO);
    }
}

void modeRandomBalanced()
{
    for (int tc = 0; tc < MAX_TESTS; ++tc)
    {
        int n = rnd.next(2, 30);
        int m = rnd.next(2, 30);
        bool wantYes = tc % 2 == 0;
        addTest(randomCaseWithOutcome(n, m, wantYes), wantYes ? YES : NO);
    }
}

void modeMaximumHorizontalYes()
{
    int n = 1, m = MAX_CELLS;
    addTest(makeCase(vector<string>(n, string(m, '.')),
                     {'L', 1}, {'R', 1}),
            YES);
}

void modeMaximumVerticalLateNo()
{
    int n = MAX_CELLS;
    vector<string> grid(n, ".");
    grid.back() = "/";
    addTest(makeCase(std::move(grid), {'U', 1}, {'D', 1}), NO);
}

void modeMaximumRectangleYes()
{
    addTest(plantedCase(400, 500, 70, true, 0), YES);
}

void modeMaximumRectangleNo()
{
    addTest(plantedCase(400, 500, 90, false, 0), NO);
}

void modeMaximumTestCount()
{
    static const int divisors[] = {1, 2, 4, 5, 8, 10, 16, 20, 25, 40,
                                   50, 80, 100, 125, 200, 250, 400, 500,
                                   1000, 2000};
    for (int tc = 0; tc < MAX_TESTS; ++tc)
    {
        int n = divisors[tc % 20];
        int m = 2000 / n;
        if (tc % 2)
            swap(n, m);
        bool wantYes = tc % 4 < 2;
        addTest(plantedCase(n, m, 60, wantYes, tc % 6),
                wantYes ? YES : NO);
    }
}
} // namespace

int main(int argc, char *argv[])
{
    registerGen(argc, argv, 1);
    int mode = opt<int>(1);

    if (mode == 1) modeHandmade();
    else if (mode == 2) modeTinyBrute();
    else if (mode == 3) modePlantedVariable();
    else if (mode == 4) modeAllFixedPatterns();
    else if (mode == 5) modeRandomBalanced();
    else if (mode == 6) modeMaximumHorizontalYes();
    else if (mode == 7) modeMaximumVerticalLateNo();
    else if (mode == 8) modeMaximumRectangleYes();
    else if (mode == 9) modeMaximumRectangleNo();
    else if (mode == 10) modeMaximumTestCount();
    else ensuref(false, "unknown mode: %d", mode);

    printTests();
    return 0;
}
