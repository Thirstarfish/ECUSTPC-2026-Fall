#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

namespace
{
using Int = __int128_t;
const Int SCALE = 10000000000000LL;
const long long COORD_LIMIT = 1000000000000000000LL;
const Int MIN_DISTANCE = SCALE / 10000;
const Int UNIT_TOLERANCE = SCALE / 1000000000;

struct Point
{
    long long x, y;
};

long long requiredUnitPairs(int n)
{
    static map<int, long long> cache;
    auto found = cache.find(n);
    if (found != cache.end())
        return found->second;

    // bit_length(n^n) - 1 = floor(n * log2(n)), exactly.
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
    const Int minimumSquared = MIN_DISTANCE * MIN_DISTANCE;
    const Int unitLower = (SCALE - UNIT_TOLERANCE) *
                         (SCALE - UNIT_TOLERANCE);
    const Int unitUpper = (SCALE + UNIT_TOLERANCE) *
                         (SCALE + UNIT_TOLERANCE);

    for (int tc = 1; tc <= (int)sizes.size(); ++tc)
    {
        setTestCase(tc);
        int n = sizes[tc - 1];
        vector<Point> points(n);
        for (int i = 0; i < n; ++i)
        {
            points[i].x = stream.readLong(-COORD_LIMIT, COORD_LIMIT, "X");
            points[i].y = stream.readLong(-COORD_LIMIT, COORD_LIMIT, "Y");
        }

        long long unitPairs = 0;
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
            {
                Int dx = (Int)points[i].x - points[j].x;
                Int dy = (Int)points[i].y - points[j].y;
                // At most 8 * 10^36, below the signed 128-bit limit.
                Int squaredDistance = dx * dx + dy * dy;

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
