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

bool reachesTarget(const TestCase &test, const vector<string> &grid)
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

    while (0 <= row && row < test.n && 0 <= column && column < test.m)
    {
        int state = 4 * (row * test.m + column) + direction;
        if (seen[state])
            return false;
        seen[state] = 1;

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
    return sameEndpoint(actual, test.target);
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

vector<bool> readAndCheckAnswers(InStream &stream, TResult verdict,
                                 const char *streamName,
                                 const vector<TestCase> &tests)
{
    vector<bool> possible(tests.size(), false);
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

        if (!reachesTarget(test, grid))
            stream.quitf(verdict,
                         "%s: the light does not leave through the target in test case %d",
                         streamName, tc + 1);
        possible[tc] = true;
    }

    if (!stream.seekEof())
        stream.quitf(verdict, "%s contains extra output", streamName);
    return possible;
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

    vector<bool> juryPossible = readAndCheckAnswers(ans, _fail, "jury answer", tests);
    vector<bool> participantPossible = readAndCheckAnswers(ouf, _wa, "participant output", tests);

    for (int tc = 0; tc < T; ++tc)
    {
        setTestCase(tc + 1);
        if (juryPossible[tc] && !participantPossible[tc])
            quitf(_wa, "jury has a construction, but participant reports IMPOSSIBLE");
        if (!juryPossible[tc] && participantPossible[tc])
            quitf(_fail, "participant found a construction while jury reports IMPOSSIBLE");
    }

    quitf(_ok, "%d test case(s) checked", T);
}
