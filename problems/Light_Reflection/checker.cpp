#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

namespace
{
enum Direction
{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

struct Endpoint
{
    char side;
    int position;
};

struct TestCase
{
    int n, m;
    vector<string> initial;
    Endpoint start, target;
};

bool sameEndpoint(const Endpoint &a, const Endpoint &b)
{
    return a.side == b.side && a.position == b.position;
}

int traversedCells(const TestCase &test, const vector<string> &grid)
{
    int row, column, direction;
    if (test.start.side == 'U')
        row = 0, column = test.start.position - 1, direction = DOWN;
    else if (test.start.side == 'D')
        row = test.n - 1, column = test.start.position - 1, direction = UP;
    else if (test.start.side == 'L')
        row = test.start.position - 1, column = 0, direction = RIGHT;
    else
        row = test.start.position - 1, column = test.m - 1, direction = LEFT;

    static const int dr[] = {-1, 0, 1, 0};
    static const int dc[] = {0, 1, 0, -1};
    vector<unsigned char> seen(4 * test.n * test.m, 0);
    int count = 0;

    while (0 <= row && row < test.n && 0 <= column && column < test.m)
    {
        int state = 4 * (row * test.m + column) + direction;
        if (seen[state])
            return -1;
        seen[state] = 1;
        ++count; // Repeated visits to the same cell are counted separately.

        if (grid[row][column] == '/')
            direction ^= 1;
        else if (grid[row][column] == '\\')
            direction ^= 3;

        row += dr[direction];
        column += dc[direction];
    }

    Endpoint actual;
    if (row < 0)
        actual = {'U', column + 1};
    else if (row >= test.n)
        actual = {'D', column + 1};
    else if (column < 0)
        actual = {'L', row + 1};
    else
        actual = {'R', row + 1};
    return sameEndpoint(actual, test.target) ? count : -1;
}

int minimumCells(const TestCase &test)
{
    // BFS states are (cell, incoming travel direction), with one visit per step.
    // A shortest relaxed path never needs conflicting choices in a free cell:
    // in the equivalent port graph its clique provides a strict shortcut.
    // Thus the relaxed minimum is attainable by a single completed grid.
    int sr, sc, sd;
    if (test.start.side == 'U') sr = 0, sc = test.start.position - 1, sd = DOWN;
    else if (test.start.side == 'D') sr = test.n - 1, sc = test.start.position - 1, sd = UP;
    else if (test.start.side == 'L') sr = test.start.position - 1, sc = 0, sd = RIGHT;
    else sr = test.start.position - 1, sc = test.m - 1, sd = LEFT;
    static const int dr[] = {-1, 0, 1, 0};
    static const int dc[] = {0, 1, 0, -1};
    vector<int> dist(4 * test.n * test.m, -1);
    queue<int> q;
    int source = 4 * (sr * test.m + sc) + sd;
    dist[source] = 1; q.push(source);
    while (!q.empty())
    {
        int state = q.front(); q.pop();
        int cell = state / 4, direction = state % 4;
        int row = cell / test.m, column = cell % test.m;
        string choices = test.initial[row][column] == '.' ? "./\\" : string(1, test.initial[row][column]);
        for (char ch : choices)
        {
            int nd = direction;
            if (ch == '/') nd ^= 1;
            else if (ch == '\\') nd ^= 3;
            int nr = row + dr[nd], nc = column + dc[nd];
            if (nr < 0 || nr >= test.n || nc < 0 || nc >= test.m)
            {
                bool hit = (nr == -1 && test.target.side == 'U' && test.target.position == nc + 1) ||
                           (nr == test.n && test.target.side == 'D' && test.target.position == nc + 1) ||
                           (nc == -1 && test.target.side == 'L' && test.target.position == nr + 1) ||
                           (nc == test.m && test.target.side == 'R' && test.target.position == nr + 1);
                if (hit) return dist[state];
            }
            else
            {
                int next = 4 * (nr * test.m + nc) + nd;
                if (dist[next] == -1) dist[next] = dist[state] + 1, q.push(next);
            }
        }
    }
    return -1;
}

void validateRow(InStream &stream, TResult verdict, const char *streamName,
                 int testIndex, int rowIndex, const TestCase &test,
                 const string &row)
{
    if ((int)row.size() != test.m)
        stream.quitf(verdict,
                     "%s: test case %d, row %d has length %d, expected %d",
                     streamName, testIndex, rowIndex + 1, (int)row.size(), test.m);

    for (int column = 0; column < test.m; ++column)
    {
        char value = row[column];
        if (value != '.' && value != '/' && value != '\\')
            stream.quitf(verdict,
                         "%s: test case %d, cell (%d,%d) has invalid character",
                         streamName, testIndex, rowIndex + 1, column + 1);
        char fixedValue = test.initial[rowIndex][column];
        if (fixedValue != '.' && value != fixedValue)
            stream.quitf(verdict,
                         "%s: test case %d changes fixed cell (%d,%d)",
                         streamName, testIndex, rowIndex + 1, column + 1);
    }
}

vector<int> readAndCheckAnswers(InStream &stream, TResult verdict,
                                 const char *streamName,
                                 const vector<TestCase> &tests)
{
    vector<int> scores(tests.size(), -1);
    for (int tc = 0; tc < (int)tests.size(); ++tc)
    {
        setTestCase(tc + 1);
        if (stream.seekEof())
            stream.quitf(verdict, "%s ends before test case %d", streamName, tc + 1);

        string firstToken = stream.readToken();
        if (firstToken == "IMPOSSIBLE")
            continue;

        const TestCase &test = tests[tc];
        vector<string> grid(test.n);
        grid[0] = firstToken;
        validateRow(stream, verdict, streamName, tc + 1, 0, test, grid[0]);
        for (int row = 1; row < test.n; ++row)
        {
            if (stream.seekEof())
                stream.quitf(verdict,
                             "%s ends inside the grid of test case %d",
                             streamName, tc + 1);
            grid[row] = stream.readToken();
            validateRow(stream, verdict, streamName, tc + 1, row, test, grid[row]);
        }

        scores[tc] = traversedCells(test, grid);
        if (scores[tc] < 0)
            stream.quitf(verdict,
                         "%s: the light does not leave through the target in test case %d",
                         streamName, tc + 1);
    }

    if (!stream.seekEof())
        stream.quitf(verdict, "%s contains extra output", streamName);
    return scores;
}
} // namespace

int main(int argc, char *argv[])
{
    registerTestlibCmd(argc, argv);

    int T = inf.readInt();
    vector<TestCase> tests(T);
    for (TestCase &test : tests)
    {
        test.n = inf.readInt();
        test.m = inf.readInt();
        test.initial.resize(test.n);
        for (string &row : test.initial)
            row = inf.readToken();
        string d1 = inf.readToken();
        int p1 = inf.readInt();
        string d2 = inf.readToken();
        int p2 = inf.readInt();
        test.start = {d1[0], p1};
        test.target = {d2[0], p2};
    }

    vector<int> juryScore = readAndCheckAnswers(ans, _fail, "jury answer", tests);
    vector<int> participantScore = readAndCheckAnswers(ouf, _wa, "participant output", tests);

    for (int tc = 0; tc < T; ++tc)
    {
        setTestCase(tc + 1);
        int optimum = minimumCells(tests[tc]);
        if (juryScore[tc] != optimum)
            quitf(_fail, "jury answer has length %d, expected %d in test case %d (-1 means IMPOSSIBLE)",
                  juryScore[tc], optimum, tc + 1);
        if (participantScore[tc] != optimum)
            quitf(_wa, "participant answer has length %d, expected %d in test case %d (-1 means IMPOSSIBLE)",
                  participantScore[tc], optimum, tc + 1);
    }

    quitf(_ok, "%d test case(s) checked", T);
}
