#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// Usage: generator <mode> <seed> > output.in
//
// Every invocation emits exactly ONE test case (the problem has no T):
//   L K N
//   p_1 c_1 ... (K lines)
//   s_1 ...     (N lines)
//
// Constraints:
//   1 <= L <= 1e18, 0 <= K <= 500, 0 <= N <= 50,
//   1 <= p_i <= L (pairwise distinct), c_i in [a-z],
//   each s_i non-empty lowercase, sum |s_i| <= 100.

namespace
{
const long long LIM = 1000000000000000000LL; // 1e18
const int MAX_K = 500;
const int MAX_N = 50;
const int MAX_TOTAL_LEN = 100;

using Constraint = pair<long long, char>;

template <class T>
void shuffleWithRnd(vector<T> &v)
{
    for (int i = (int)v.size() - 1; i > 0; --i)
        swap(v[i], v[rnd.next(0, i)]);
}

string randomString(int len, int alphaSize = 26)
{
    ensuref(len >= 0, "negative length");
    string s;
    s.reserve(len);
    for (int i = 0; i < len; ++i)
        s.push_back(char('a' + rnd.next(0, alphaSize - 1)));
    return s;
}

// K distinct positions in [1, L].
vector<long long> distinctPositions(int K, long long L)
{
    ensuref(0 <= K && K <= L, "cannot place %d distinct positions in [1,%lld]", K, L);
    vector<long long> res;
    set<long long> seen;
    while ((int)res.size() < K)
    {
        long long p = rnd.next(1LL, L);
        if (seen.insert(p).second)
            res.push_back(p);
    }
    return res;
}

// Build K = gaps.size()-1 forced positions whose free gaps are exactly `gaps`.
// L = sum(gaps) + gaps.size() - 1. The trailing gap is gaps.back().
pair<long long, vector<Constraint>> fromGaps(const vector<long long> &gaps)
{
    ensuref(gaps.size() >= 1, "need at least one gap");
    long long L = 0;
    for (long long g : gaps)
        L += g;
    L += (long long)gaps.size() - 1;

    vector<Constraint> cons;
    long long p = 0;
    for (size_t i = 0; i + 1 < gaps.size(); ++i)
    {
        p += gaps[i] + 1;
        cons.push_back({p, char('a' + rnd.next(0, 25))});
    }
    return {L, cons};
}

// 50 distinct length-2 strings ("aa".."bx"): N = 50, total length = 100,
// which yields the maximum number of AC-automaton states (1 + 100 = 101).
vector<string> maxStatesForbidden()
{
    vector<string> f;
    for (int i = 0; i < 50; ++i)
    {
        string s;
        s.push_back(char('a' + i / 26));
        s.push_back(char('a' + i % 26));
        f.push_back(s);
    }
    return f;
}

void emit(long long L, int K, int N,
          const vector<Constraint> &constraints,
          const vector<string> &forbidden)
{
    ensuref(1 <= L && L <= LIM, "L=%lld out of range", L);
    ensuref(0 <= K && K <= MAX_K, "K=%d out of range", K);
    ensuref(0 <= N && N <= MAX_N, "N=%d out of range", N);
    ensuref((int)constraints.size() == K, "constraint count mismatch");
    ensuref((int)forbidden.size() == N, "forbidden count mismatch");

    set<long long> ps;
    for (const auto &pr : constraints)
    {
        ensuref(1 <= pr.first && pr.first <= L, "position %lld out of [1,%lld]", pr.first, L);
        ensuref('a' <= pr.second && pr.second <= 'z', "bad char %c", pr.second);
        ensuref(ps.insert(pr.first).second, "duplicate position %lld", pr.first);
    }

    long long total = 0;
    for (const string &s : forbidden)
    {
        ensuref(!s.empty(), "forbidden string must be non-empty");
        total += (long long)s.size();
        ensuref(total <= MAX_TOTAL_LEN, "sum of lengths exceeds %d", MAX_TOTAL_LEN);
        for (char ch : s)
            ensuref('a' <= ch && ch <= 'z', "bad char in forbidden string");
    }

    cout << L << ' ' << K << ' ' << N << '\n';
    for (const auto &pr : constraints)
        cout << pr.first << ' ' << pr.second << '\n';
    for (const string &s : forbidden)
        cout << s << '\n';
}

// mode 1 -- the statement sample.
void modeSample()
{
    emit(3, 1, 1, {{2, 'b'}}, {"ab"});
}

// mode 2 -- tiny random cases over alphabet {a,b,c} with L in [1,6].
void modeTiny()
{
    int L = rnd.next(1, 6);
    int alphaSize = 3;
    int K = rnd.next(0, min(3, L));
    int N = rnd.next(0, 4);

    vector<Constraint> constraints;
    for (long long p : distinctPositions(K, L))
        constraints.push_back({p, char('a' + rnd.next(0, alphaSize - 1))});
    shuffleWithRnd(constraints);

    vector<string> forbidden;
    for (int i = 0; i < N; ++i)
        forbidden.push_back(randomString(rnd.next(1, 3), alphaSize));

    emit(L, K, N, constraints, forbidden);
}

// mode 3 -- small random cases over the full alphabet, L in [10, 2000].
void modeSmall()
{
    int L = rnd.next(10, 2000);
    int K = rnd.next(0, MAX_K);
    if (K > L)
        K = L;

    vector<Constraint> constraints;
    for (long long p : distinctPositions(K, L))
        constraints.push_back({p, char('a' + rnd.next(0, 25))});
    shuffleWithRnd(constraints);

    int N = rnd.next(0, MAX_N);
    vector<string> forbidden;
    int total = 0;
    for (int i = 0; i < N; ++i)
    {
        int len = rnd.next(1, 6);
        if (total + len > MAX_TOTAL_LEN)
            break;
        forbidden.push_back(randomString(len));
        total += len;
    }

    emit(L, (int)constraints.size(), (int)forbidden.size(), constraints, forbidden);
}

// mode 4 -- medium random cases, L in [1e6, 1e12].
void modeMedium()
{
    long long L = rnd.next(1000000LL, 1000000000000LL);
    int K = rnd.next(0, MAX_K);

    vector<Constraint> constraints;
    for (long long p : distinctPositions(K, L))
        constraints.push_back({p, char('a' + rnd.next(0, 25))});
    shuffleWithRnd(constraints);

    int N = rnd.next(0, MAX_N);
    vector<string> forbidden;
    int total = 0;
    for (int i = 0; i < N; ++i)
    {
        int len = rnd.next(1, 10);
        if (total + len > MAX_TOTAL_LEN)
            break;
        forbidden.push_back(randomString(len));
        total += len;
    }

    emit(L, (int)constraints.size(), (int)forbidden.size(), constraints, forbidden);
}

// mode 5 -- maximum L = 1e18 with a full random structure.
void modeLarge()
{
    long long L = LIM;
    int K = rnd.next(0, MAX_K);

    vector<Constraint> constraints;
    for (long long p : distinctPositions(K, L))
        constraints.push_back({p, char('a' + rnd.next(0, 25))});
    shuffleWithRnd(constraints);

    int N = rnd.next(1, MAX_N);
    vector<string> forbidden;
    int total = 0;
    for (int i = 0; i < N; ++i)
    {
        int len = rnd.next(1, 10);
        if (total + len > MAX_TOTAL_LEN)
            break;
        forbidden.push_back(randomString(len));
        total += len;
    }

    emit(L, (int)constraints.size(), (int)forbidden.size(), constraints, forbidden);
}

// mode 6 -- single forbidden character "a" at max L: answer = 25^L.
void modeSingleForbidden()
{
    emit(LIM, 0, 1, {}, {"a"});
}

// mode 7 -- self-overlapping forbidden string "aa" at max L.
void modeSelfOverlap()
{
    emit(LIM, 0, 1, {}, {"aa"});
}

// mode 8 -- forbidden "ab" at max L.
void modeTwoCharForbidden()
{
    emit(LIM, 0, 1, {}, {"ab"});
}

// mode 9 -- duplicate forbidden strings "aa" twice == forbid "aa" once.
void modeDuplicates()
{
    emit(LIM, 0, 2, {}, {"aa", "aa"});
}

// mode 10 -- maximum L, maximum K = 500, no forbidden strings: answer = 26^(L-K).
// Positions are emitted unsorted on purpose (solution must sort them).
void modeMaxFreeNoForbidden()
{
    long long L = LIM;
    int K = MAX_K;
    vector<Constraint> constraints;
    for (long long p : distinctPositions(K, L))
        constraints.push_back({p, char('a' + rnd.next(0, 25))});
    shuffleWithRnd(constraints);
    emit(L, K, 0, constraints, {});
}

// mode 11 -- boundary positions 1 and L at maximum L, no forbidden strings.
void modeBoundaryPositions()
{
    long long L = LIM;
    emit(L, 2, 0, {{1, char('a' + rnd.next(0, 25))}, {L, char('a' + rnd.next(0, 25))}}, {});
}

// mode 12 -- planted zero: forbid "a" and force a position to be 'a'.
void modeContradictionSingle()
{
    emit(3, 1, 1, {{2, 'a'}}, {"a"});
}

// mode 13 -- planted zero via a fail-chain: forbid "ab" and force "ab" to appear.
void modeContradictionPair()
{
    emit(3, 2, 1, {{1, 'a'}, {2, 'b'}}, {"ab"});
}

// mode 14 -- maximum N = 50 and maximum total length = 100 at max L.
void modeMaxN()
{
    emit(LIM, 0, 50, {}, maxStatesForbidden());
}

// mode 15 -- a single forbidden string of maximum length 100 ("a"*100), L = 100.
// Only the all-'a' string is excluded: answer = 26^100 - 1.
void modeLongForbidden()
{
    emit(100, 0, 1, {}, {string(100, 'a')});
}

// mode 16 -- hack: a single middle gap of 2^59 - 1 (all 59 bits set), the largest
// all-ones gap that fits in L <= 1e18. Forces 59 matrix-vector products in one segment.
void modeAllOnesMiddle()
{
    auto [L, cons] = fromGaps({0, (1LL << 59) - 1, 0});
    emit(L, (int)cons.size(), 50, cons, maxStatesForbidden());
}

// mode 17 -- hack: greedily pack as many all-ones gaps (2^k - 1) as fit in L <= 1e18.
void modeAllOnesPack()
{
    vector<long long> gaps;
    long long sum = 0;
    for (int k = 59; k >= 1; --k)
    {
        long long g = (1LL << k) - 1;
        if (sum + g + (long long)gaps.size() <= LIM)
        {
            gaps.push_back(g);
            sum += g;
        }
    }
    if (gaps.empty())
        gaps.push_back(1);
    auto [L, cons] = fromGaps(gaps);
    emit(L, (int)cons.size(), 50, cons, maxStatesForbidden());
}

// mode 18 -- hack: maximum K = 500 with every gap = 2^50 - 1 (50 bits set) plus a
// remainder gap, maximising the total number of matrix-vector products.
void modeMaxKAllOnes()
{
    long long g = (1LL << 50) - 1;
    vector<long long> gaps(MAX_K, g);
    long long remainder = LIM - MAX_K * g - MAX_K;
    ensuref(remainder >= 0, "mode 18 remainder negative");
    gaps.push_back(remainder);

    auto [L, cons] = fromGaps(gaps);
    ensuref(L == LIM, "mode 18 L = %lld != LIM", L);
    emit(L, (int)cons.size(), 50, cons, maxStatesForbidden());
}
} // namespace

int main(int argc, char *argv[])
{
    registerGen(argc, argv, 1);
    int mode = opt<int>(1);

    if (mode == 1) modeSample();
    else if (mode == 2) modeTiny();
    else if (mode == 3) modeSmall();
    else if (mode == 4) modeMedium();
    else if (mode == 5) modeLarge();
    else if (mode == 6) modeSingleForbidden();
    else if (mode == 7) modeSelfOverlap();
    else if (mode == 8) modeTwoCharForbidden();
    else if (mode == 9) modeDuplicates();
    else if (mode == 10) modeMaxFreeNoForbidden();
    else if (mode == 11) modeBoundaryPositions();
    else if (mode == 12) modeContradictionSingle();
    else if (mode == 13) modeContradictionPair();
    else if (mode == 14) modeMaxN();
    else if (mode == 15) modeLongForbidden();
    else if (mode == 16) modeAllOnesMiddle();
    else if (mode == 17) modeAllOnesPack();
    else if (mode == 18) modeMaxKAllOnes();
    else ensuref(false, "unknown mode: %d", mode);

    return 0;
}
