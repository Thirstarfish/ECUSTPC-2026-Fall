#include <bits/stdc++.h>

using namespace std;

static const int LIMIT = 200 * 5000;

static uint64_t lowMask(int bit) {
    int r = bit & 63;
    return r == 0 ? ~0ULL : (~0ULL << r);
}

static uint64_t highMask(int bit) {
    int r = bit & 63;
    return r == 63 ? ~0ULL : ((1ULL << (r + 1)) - 1);
}

static bool boundedDifferenceDp(const vector<int>& a, int limit) {
    const int bits = 2 * limit + 1;
    const int offset = limit;
    const int words = (bits + 63) >> 6;

    vector<uint64_t> cur(words, 0), nxt(words, 0);
    int lo = offset, hi = offset;
    cur[offset >> 6] = 1ULL << (offset & 63);

    for (int x : a) {
        int nlo = max(0, lo - x);
        int nhi = min(bits - 1, hi + x);
        int lw = nlo >> 6, hw = nhi >> 6;
        fill(nxt.begin() + lw, nxt.begin() + hw + 1, 0);

        int plw = lo >> 6, phw = hi >> 6;
        int ws = x >> 6, bs = x & 63;

        if (bs == 0) {
            for (int i = plw; i <= phw; ++i) {
                uint64_t v = cur[i];
                if (v == 0) continue;

                int j = i + ws;
                if (lw <= j && j <= hw) nxt[j] |= v;

                j = i - ws;
                if (lw <= j && j <= hw) nxt[j] |= v;
            }
        } else {
            for (int i = plw; i <= phw; ++i) {
                uint64_t v = cur[i];
                if (v == 0) continue;

                int j = i + ws;
                if (lw <= j && j <= hw) nxt[j] |= v << bs;
                if (lw <= j + 1 && j + 1 <= hw) nxt[j + 1] |= v >> (64 - bs);

                j = i - ws;
                if (lw <= j && j <= hw) nxt[j] |= v >> bs;
                if (lw <= j - 1 && j - 1 <= hw) nxt[j - 1] |= v << (64 - bs);
            }
        }

        if (lw == hw) {
            nxt[lw] &= lowMask(nlo) & highMask(nhi);
        } else {
            nxt[lw] &= lowMask(nlo);
            nxt[hw] &= highMask(nhi);
        }

        cur.swap(nxt);
        lo = nlo;
        hi = nhi;
    }

    return (cur[offset >> 6] >> (offset & 63)) & 1ULL;
}

static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

static bool solveOne(vector<int> a, int testId) {
    long long sum = 0;
    uint64_t seed = 0x4d595df4d0f33173ULL ^ (uint64_t)testId;
    for (int x : a) {
        sum += x;
        seed = splitmix64(seed ^ (uint64_t)x);
    }

    if (sum & 1LL) return false;

    int limit = (int)min<long long>(LIMIT, sum);
    int tries = (sum <= LIMIT ? 1 : 2);

    for (int attempt = 0; attempt < tries; ++attempt) {
        vector<int> b = a;
        mt19937_64 rng(splitmix64(seed ^ (uint64_t)a.size() ^ (uint64_t)attempt));
        shuffle(b.begin(), b.end(), rng);
        if (boundedDifferenceDp(b, limit)) return true;
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    for (int tc = 1; tc <= t; ++tc) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int& x : a) cin >> x;

        cout << (solveOne(a, tc) ? "YES" : "NO") << '\n';
    }

    return 0;
}
