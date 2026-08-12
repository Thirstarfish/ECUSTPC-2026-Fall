// Brute force: enumerate all 2^V assignments. Used only as reference for V <= ~22.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, H;
    cin >> N >> M >> H;
    int V = N * M * H;
    if (V > 22) { cerr << "brute: V too large (" << V << ")\n"; return 1; }

    auto id = [&](int x, int y, int z) { return (x * M + y) * H + z; };
    auto inGrid = [&](int x, int y, int z) {
        return 0 <= x && x < N && 0 <= y && y < M && 0 <= z && z < H;
    };

    vector<vector<long long>> A(8, vector<long long>(V));
    for (int t = 0; t < 8; ++t)
        for (int x = 0; x < N; ++x)
            for (int y = 0; y < M; ++y)
                for (int z = 0; z < H; ++z)
                    cin >> A[t][id(x, y, z)];

    vector<array<int, 3>> off[3];
    for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy)
            for (int dz = -1; dz <= 1; ++dz) {
                int s2 = dx*dx + dy*dy + dz*dz;
                if (s2 >= 1 && s2 <= 3) off[s2 - 1].push_back({dx, dy, dz});
            }

    long long best = LLONG_MIN;
    long long total = 1LL << V;
    for (long long mask = 0; mask < total; ++mask) {
        vector<int> st(V);
        for (int i = 0; i < V; ++i) st[i] = (mask >> i) & 1;

        long long sc = 0;
        for (int x = 0; x < N; ++x)
            for (int y = 0; y < M; ++y)
                for (int z = 0; z < H; ++z) {
                    int u = id(x, y, z);
                    sc += st[u] == 0 ? A[0][u] : A[1][u];

                    // For each cluster k, requirements:
                    //  k=0 (S1, different):  center=0 -> all neighbors=1 -> A3, center=1 -> all 0 -> A4
                    //  k=1 (S2, same):       center=0 -> all 0 -> A5, center=1 -> all 1 -> A6
                    //  k=2 (S3, different):  center=0 -> all 1 -> A7, center=1 -> all 0 -> A8
                    for (int k = 0; k < 3; ++k) {
                        bool diff = (k == 0 || k == 2);
                        int needNbr = diff ? (1 - st[u]) : st[u];
                        bool ok = true;
                        for (auto& d : off[k]) {
                            int nx = x + d[0], ny = y + d[1], nz = z + d[2];
                            if (!inGrid(nx, ny, nz)) continue;
                            if (st[id(nx, ny, nz)] != needNbr) { ok = false; break; }
                        }
                        if (ok) {
                            int aIdx = 2 + 2 * k + st[u]; // A3..A8 indices 2..7
                            sc += A[aIdx][u];
                        }
                    }
                }
        best = max(best, sc);
    }
    cout << best << "\n";
    return 0;
}
