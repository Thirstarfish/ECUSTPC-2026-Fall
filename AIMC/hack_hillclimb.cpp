#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// 3D grid quantum stabilizer - Multi-start hill climbing

int N, M, H, V;
ll A[8][8000];
int state[8000], best_state[8000];

vector<int> nbr[3][8000];
int nbr_cnt[3][8000];

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
            nbr[d2-1][u].push_back(cid(nx, ny, nz));
        }
        for (int k = 0; k < 3; k++) nbr_cnt[k][u] = (int)nbr[k][u].size();
    }
}

// Count neighbors of u in each state for each cluster type
int cnt0[3][8000], cnt1[3][8000];

void compute_counts() {
    for (int u = 0; u < V; u++)
        for (int k = 0; k < 3; k++) {
            cnt0[k][u] = cnt1[k][u] = 0;
            for (int v : nbr[k][u]) {
                if (state[v] == 0) cnt0[k][u]++;
                else cnt1[k][u]++;
            }
        }
}

ll compute_total_energy() {
    ll total = 0;
    for (int u = 0; u < V; u++) {
        int s = state[u];
        total += (s == 0) ? A[0][u] : A[1][u];
        if (s == 0 && cnt1[0][u] == nbr_cnt[0][u]) total += A[2][u];
        if (s == 1 && cnt0[0][u] == nbr_cnt[0][u]) total += A[3][u];
        if (s == 0 && cnt0[1][u] == nbr_cnt[1][u]) total += A[4][u];
        if (s == 1 && cnt1[1][u] == nbr_cnt[1][u]) total += A[5][u];
        if (s == 0 && cnt1[2][u] == nbr_cnt[2][u]) total += A[6][u];
        if (s == 1 && cnt0[2][u] == nbr_cnt[2][u]) total += A[7][u];
    }
    return total;
}

// Compute the energy contribution from cell u given current state
ll cell_energy(int u) {
    int s = state[u];
    ll e = (s == 0) ? A[0][u] : A[1][u];
    // S1: all face-neighbors opposite
    if (s == 0 && cnt1[0][u] == nbr_cnt[0][u]) e += A[2][u];
    if (s == 1 && cnt0[0][u] == nbr_cnt[0][u]) e += A[3][u];
    // S2: all edge-neighbors same
    if (s == 0 && cnt0[1][u] == nbr_cnt[1][u]) e += A[4][u];
    if (s == 1 && cnt1[1][u] == nbr_cnt[1][u]) e += A[5][u];
    // S3: all body-diagonal-neighbors opposite
    if (s == 0 && cnt1[2][u] == nbr_cnt[2][u]) e += A[6][u];
    if (s == 1 && cnt0[2][u] == nbr_cnt[2][u]) e += A[7][u];
    return e;
}

// Compute delta energy if we flip cell u
// We need to consider:
// 1. Change in u's own energy contribution
// 2. Change in neighbors' cluster bonuses that depend on u
ll compute_delta(int u) {
    int s = state[u]; // current state

    // Energy of u before flip
    ll e_before = cell_energy(u);

    // Energy contributions of neighbors that might change (their cluster bonuses involving u)
    ll nb_before = 0;
    for (int k = 0; k < 3; k++) {
        for (int v : nbr[k][u]) {
            int sv = state[v];
            // Check if v's cluster k bonus is active
            if (k == 0 || k == 2) { // all-opposite
                if (sv == 0 && cnt1[k][v] == nbr_cnt[k][v]) nb_before += A[2+2*k][v];
                if (sv == 1 && cnt0[k][v] == nbr_cnt[k][v]) nb_before += A[2+2*k+1][v];
            } else { // k==1, all-same
                if (sv == 0 && cnt0[k][v] == nbr_cnt[k][v]) nb_before += A[2+2*k][v];
                if (sv == 1 && cnt1[k][v] == nbr_cnt[k][v]) nb_before += A[2+2*k+1][v];
            }
        }
    }

    // Simulate flip
    int ns = 1 - s;

    // Update counts temporarily
    for (int k = 0; k < 3; k++) {
        for (int v : nbr[k][u]) {
            if (s == 0) { cnt0[k][v]--; cnt1[k][v]++; }
            else { cnt1[k][v]--; cnt0[k][v]++; }
        }
    }
    state[u] = ns;

    // Recompute u's counts (they don't change since neighbors didn't flip)
    // Actually u's counts are about u's neighbors, which didn't change
    // But we need to recompute cell_energy with new state
    ll e_after = cell_energy(u);

    ll nb_after = 0;
    for (int k = 0; k < 3; k++) {
        for (int v : nbr[k][u]) {
            int sv = state[v];
            if (k == 0 || k == 2) {
                if (sv == 0 && cnt1[k][v] == nbr_cnt[k][v]) nb_after += A[2+2*k][v];
                if (sv == 1 && cnt0[k][v] == nbr_cnt[k][v]) nb_after += A[2+2*k+1][v];
            } else {
                if (sv == 0 && cnt0[k][v] == nbr_cnt[k][v]) nb_after += A[2+2*k][v];
                if (sv == 1 && cnt1[k][v] == nbr_cnt[k][v]) nb_after += A[2+2*k+1][v];
            }
        }
    }

    // Undo flip
    state[u] = s;
    for (int k = 0; k < 3; k++) {
        for (int v : nbr[k][u]) {
            if (s == 0) { cnt0[k][v]++; cnt1[k][v]--; }
            else { cnt1[k][v]++; cnt0[k][v]--; }
        }
    }

    return (e_after + nb_after) - (e_before + nb_before);
}

void do_flip(int u) {
    int s = state[u];
    for (int k = 0; k < 3; k++) {
        for (int v : nbr[k][u]) {
            if (s == 0) { cnt0[k][v]--; cnt1[k][v]++; }
            else { cnt1[k][v]--; cnt0[k][v]++; }
        }
    }
    state[u] = 1 - s;
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

    mt19937 rng(42);
    auto start_time = chrono::steady_clock::now();
    auto elapsed_ms = [&]() {
        return chrono::duration_cast<chrono::milliseconds>(
            chrono::steady_clock::now() - start_time).count();
    };

    ll best_energy = LLONG_MIN;
    int restarts = 0;

    while (elapsed_ms() < 2500) {
        restarts++;

        // Random initial state
        for (int u = 0; u < V; u++)
            state[u] = rng() & 1;

        compute_counts();
        ll cur_energy = compute_total_energy();

        // Hill climb until local optimum
        bool improved = true;
        while (improved && elapsed_ms() < 2500) {
            improved = false;
            for (int u = 0; u < V; u++) {
                ll delta = compute_delta(u);
                if (delta > 0) {
                    do_flip(u);
                    cur_energy += delta;
                    improved = true;
                }
            }
        }

        if (cur_energy > best_energy) {
            best_energy = cur_energy;
            memcpy(best_state, state, sizeof(int) * V);
        }
    }

    cout << best_energy << "\n";
    return 0;
}
