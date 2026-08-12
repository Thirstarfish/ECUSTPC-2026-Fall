#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// 3D grid quantum stabilizer - Greedy assignment with random orderings

int N, M, H, V;
int state[8000];
int assigned[8000];
ll A[8][8000];

vector<int> nbr[3][8000];
int nbr_cnt[3][8000];
int cnt0[3][8000], cnt1[3][8000];

inline int cid(int x, int y, int z) { return (x * M + y) * H + z; }

void precompute_neighbors() {
    for (int x = 0; x < N; x++)
    for (int y = 0; y < M; y++)
    for (int z = 0; z < H; z++) {
        int u = cid(x, y, z);
        for (int k = 0; k < 3; k++) nbr[k][u].clear();
        for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
        for (int dz = -1; dz <= 1; dz++) {
            int d2 = dx*dx + dy*dy + dz*dz;
            if (d2 < 1 || d2 > 3) continue;
            int nx = x+dx, ny = y+dy, nz = z+dz;
            if (nx < 0 || nx >= N || ny < 0 || ny >= M || nz < 0 || nz >= H) continue;
            int v = cid(nx, ny, nz);
            nbr[d2-1][u].push_back(v);
        }
        for (int k = 0; k < 3; k++) nbr_cnt[k][u] = (int)nbr[k][u].size();
    }
}

// Compute energy contribution of cell u considering only assigned neighbors
// For cluster bonuses, we check: all assigned neighbors satisfy condition AND
// no unassigned neighbors remain (i.e., all neighbors are assigned and satisfy)
// Actually for greedy, we use a simpler heuristic:
// compute immediate energy assuming unassigned neighbors will be "optimal"
// But simplest approach: only count cluster bonus if ALL neighbors are assigned and satisfy condition
ll eval_cell(int u, int s) {
    ll e = (s == 0) ? A[0][u] : A[1][u];

    for (int k = 0; k < 3; k++) {
        bool all_assigned = true;
        bool all_satisfy = true;
        for (int v : nbr[k][u]) {
            if (!assigned[v]) { all_assigned = false; break; }
            if (k == 0 || k == 2) { // all-opposite
                if (state[v] == s) { all_satisfy = false; break; }
            } else { // k==1, all-same
                if (state[v] != s) { all_satisfy = false; break; }
            }
        }
        if (all_assigned && all_satisfy) {
            e += A[2 + 2*k + (s == 1 ? 1 : 0)][u];
        }
    }

    return e;
}

// Also consider: flipping u might activate neighbors' cluster bonuses
ll eval_with_neighbor_effect(int u, int s) {
    ll e = eval_cell(u, s);

    // Check if assigning u completes any neighbor's cluster condition
    for (int k = 0; k < 3; k++) {
        for (int v : nbr[k][u]) {
            if (!assigned[v]) continue;
            int sv = state[v];
            // Check if all of v's neighbors in cluster k are now assigned and satisfy
            bool all_ok = true;
            for (int w : nbr[k][v]) {
                if (w == u) {
                    // w is being assigned to s
                    if (k == 0 || k == 2) { // all-opposite
                        if (s == sv) { all_ok = false; break; }
                    } else { // all-same
                        if (s != sv) { all_ok = false; break; }
                    }
                } else if (!assigned[w]) {
                    all_ok = false; break;
                } else {
                    if (k == 0 || k == 2) {
                        if (state[w] == sv) { all_ok = false; break; }
                    } else {
                        if (state[w] != sv) { all_ok = false; break; }
                    }
                }
            }
            if (all_ok) {
                // Check if it wasn't already active before (it couldn't be, since u wasn't assigned)
                e += A[2 + 2*k + (sv == 1 ? 1 : 0)][v];
            }
        }
    }

    return e;
}

ll run_greedy(vector<int>& order) {
    memset(assigned, 0, sizeof(int) * V);
    memset(cnt0, 0, sizeof(int) * V * 3);
    memset(cnt1, 0, sizeof(int) * V * 3);

    ll total = 0;
    for (int u : order) {
        ll e0 = eval_with_neighbor_effect(u, 0);
        ll e1 = eval_with_neighbor_effect(u, 1);
        if (e0 >= e1) {
            state[u] = 0;
        } else {
            state[u] = 1;
        }
        assigned[u] = 1;
    }

    // Compute actual total energy
    // Reset counts
    for (int u = 0; u < V; u++) {
        for (int k = 0; k < 3; k++) {
            cnt0[k][u] = cnt1[k][u] = 0;
            for (int v : nbr[k][u]) {
                if (state[v] == 0) cnt0[k][u]++;
                else cnt1[k][u]++;
            }
        }
    }

    total = 0;
    for (int u = 0; u < V; u++) {
        int s = state[u];
        total += (s == 0) ? A[0][u] : A[1][u];
        // S1
        if (s == 0 && cnt1[0][u] == nbr_cnt[0][u]) total += A[2][u];
        if (s == 1 && cnt0[0][u] == nbr_cnt[0][u]) total += A[3][u];
        // S2
        if (s == 0 && cnt0[1][u] == nbr_cnt[1][u]) total += A[4][u];
        if (s == 1 && cnt1[1][u] == nbr_cnt[1][u]) total += A[5][u];
        // S3
        if (s == 0 && cnt1[2][u] == nbr_cnt[2][u]) total += A[6][u];
        if (s == 1 && cnt0[2][u] == nbr_cnt[2][u]) total += A[7][u];
    }

    return total;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M >> H;
    V = N * M * H;

    for (int t = 0; t < 8; t++)
        for (int i = 0; i < V; i++)
            cin >> A[t][i];

    precompute_neighbors();

    mt19937 rng(12345);
    auto start_time = chrono::steady_clock::now();
    auto elapsed_ms = [&]() {
        return chrono::duration_cast<chrono::milliseconds>(
            chrono::steady_clock::now() - start_time).count();
    };

    ll best_energy = LLONG_MIN;

    // Try multiple random orderings
    vector<int> order(V);
    iota(order.begin(), order.end(), 0);

    for (int trial = 0; elapsed_ms() < 2500; trial++) {
        if (trial > 0) shuffle(order.begin(), order.end(), rng);

        ll e = run_greedy(order);
        if (e > best_energy) best_energy = e;

        // Also try reverse
        reverse(order.begin(), order.end());
        e = run_greedy(order);
        if (e > best_energy) best_energy = e;
    }

    cout << best_energy << "\n";
    return 0;
}
