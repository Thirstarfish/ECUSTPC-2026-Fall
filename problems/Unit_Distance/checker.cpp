#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

namespace
{
const long double COORD_LIMIT = 100000.0L;
const long double MIN_DISTANCE = 0.0001L;
const long double UNIT_TOLERANCE = 0.000000001L;

struct Point
{
    long double x, y;
};

bool hasDecimalSyntax(const string &token)
{
    size_t pos = 0;
    if (pos < token.size() && (token[pos] == '+' || token[pos] == '-'))
        ++pos;

    bool hasDigit = false;
    while (pos < token.size() && isdigit((unsigned char)token[pos]))
        hasDigit = true, ++pos;

    if (pos < token.size() && token[pos] == '.')
    {
        ++pos;
        while (pos < token.size() && isdigit((unsigned char)token[pos]))
            hasDigit = true, ++pos;
    }
    if (!hasDigit)
        return false;

    if (pos < token.size() && (token[pos] == 'e' || token[pos] == 'E'))
    {
        ++pos;
        if (pos < token.size() && (token[pos] == '+' || token[pos] == '-'))
            ++pos;
        size_t exponentStart = pos;
        while (pos < token.size() && isdigit((unsigned char)token[pos]))
            ++pos;
        if (pos == exponentStart)
            return false;
    }
    return pos == token.size();
}

long double readCoordinate(InStream &stream, TResult verdict,
                           const char *streamName, int tc, int point,
                           const char *axis)
{
    if (stream.seekEof())
        stream.quitf(verdict,
                     "%s ends before coordinate %s of point %d in test case %d",
                     streamName, axis, point, tc);

    string token = stream.readToken();
    if (!hasDecimalSyntax(token))
        stream.quitf(verdict,
                     "%s: coordinate %s of point %d in test case %d is not a valid real number",
                     streamName, axis, point, tc);

    char *end = nullptr;
    long double value = strtold(token.c_str(), &end);
    if (end == token.c_str() || *end != '\0' || !isfinite(value))
        stream.quitf(verdict,
                     "%s: coordinate %s of point %d in test case %d is not finite",
                     streamName, axis, point, tc);
    if (value < -COORD_LIMIT || value > COORD_LIMIT)
        stream.quitf(verdict,
                     "%s: coordinate %s of point %d in test case %d is out of range",
                     streamName, axis, point, tc);
    return value;
}

long long requiredUnitPairs(int n)
{
    static map<int, long long> cache;
    auto found = cache.find(n);
    if (found != cache.end())
        return found->second;

    vector<uint32_t> power(1, 1);
    for (int i = 0; i < n; ++i)
    {
        uint64_t carry = 0;
        for (uint32_t &digit : power)
        {
            uint64_t product = (uint64_t)digit * n + carry;
            digit = (uint32_t)product;
            carry = product >> 32;
        }
        if (carry != 0)
            power.push_back((uint32_t)carry);
    }

    uint32_t highestDigit = power.back();
    int highestBit = 31 - __builtin_clz(highestDigit);
    long long floorLog2 = 32LL * ((long long)power.size() - 1) + highestBit;
    long long answer = max(0LL, floorLog2 / 2 - n);
    cache[n] = answer;
    return answer;
}

void readAndCheckOutput(InStream &stream, TResult verdict,
                        const char *streamName, const vector<int> &sizes)
{
    const long double minimumSquared = MIN_DISTANCE * MIN_DISTANCE;
    const long double unitLower = (1.0L - UNIT_TOLERANCE) *
                                  (1.0L - UNIT_TOLERANCE);
    const long double unitUpper = (1.0L + UNIT_TOLERANCE) *
                                  (1.0L + UNIT_TOLERANCE);

    for (int tc = 1; tc <= (int)sizes.size(); ++tc)
    {
        setTestCase(tc);
        int n = sizes[tc - 1];
        vector<Point> points(n);
        for (int i = 0; i < n; ++i)
        {
            points[i].x = readCoordinate(stream, verdict, streamName,
                                         tc, i + 1, "x");
            points[i].y = readCoordinate(stream, verdict, streamName,
                                         tc, i + 1, "y");
        }

        long long unitPairs = 0;
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
            {
                long double dx = points[i].x - points[j].x;
                long double dy = points[i].y - points[j].y;
                long double squaredDistance = dx * dx + dy * dy;

                if (squaredDistance < minimumSquared)
                    stream.quitf(verdict,
                                 "%s: points %d and %d in test case %d are too close",
                                 streamName, i + 1, j + 1, tc);
                if (unitLower <= squaredDistance && squaredDistance <= unitUpper)
                    ++unitPairs;
            }

        long long required = requiredUnitPairs(n);
        if (unitPairs < required)
            stream.quitf(verdict,
                         "%s: test case %d has only %lld unit-distance pairs, expected at least %lld",
                         streamName, tc, unitPairs, required);
    }

    if (!stream.seekEof())
        stream.quitf(verdict, "%s contains extra output", streamName);
}
} // namespace

int main(int argc, char *argv[])
{
    registerTestlibCmd(argc, argv);

    int T = inf.readInt();
    vector<int> sizes(T);
    for (int &n : sizes)
        n = inf.readInt();

    readAndCheckOutput(ans, _fail, "jury answer", sizes);
    readAndCheckOutput(ouf, _wa, "participant output", sizes);
    quitf(_ok, "%d test case(s) checked", T);
}
