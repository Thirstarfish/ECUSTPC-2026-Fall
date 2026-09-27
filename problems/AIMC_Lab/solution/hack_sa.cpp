#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// 3D grid quantum stabilizer - Simulated Annealing solution
// Strong heuristic: efficient delta computation, exponential cooling, multiple restarts

int N, M, H, V;
int state[100];
ll A[8][100];

// Neighbor lists for each cell, by type (0=S1/face, 1=S2/edge, 2=S3/body-diag)
vector<int> nbr[3][100];
int nbr_cnt[3][100]; // count of neighbors in grid

// For each cell u and cluster k: count of neighbors with state=0 and state=1
int cnt0[3][100], cnt1[3][100];

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

void recount_all() {
    for (int u = 0; u < V; u++) {
        for (int k = 0; k < 3; k++) {
            cnt0[k][u] = cnt1[k][u] = 0;
            for (int v : nbr[k][u]) {
                if (state[v] == 0) cnt0[k][u]++;
                else cnt1[k][u]++;
            }
        }
    }
}

// Compute energy contribution of cell u given current state
inline ll cell_energy(int u) {
    int s = state[u];
    ll e = (s == 0) ? A[0][u] : A[1][u];

    // S1 (face, all-opposite): u=0 needs all nbr=1; u=1 needs all nbr=0
    if (s == 0) {
        if (cnt1[0][u] == nbr_cnt[0][u]) e += A[2][u];
    } else {
        if (cnt0[0][u] == nbr_cnt[0][u]) e += A[3][u];
    }

    // S2 (edge, all-same): u=0 needs all nbr=0; u=1 needs all nbr=1
    if (s == 0) {
        if (cnt0[1][u] == nbr_cnt[1][u]) e += A[4][u];
    } else {
        if (cnt1[1][u] == nbr_cnt[1][u]) e += A[5][u];
    }

    // S3 (body-diag, all-opposite): u=0 needs all nbr=1; u=1 needs all nbr=0
    if (s == 0) {
        if (cnt1[2][u] == nbr_cnt[2][u]) e += A[6][u];
    } else {
        if (cnt0[2][u] == nbr_cnt[2][u]) e += A[7][u];
    }

    return e;
}

// Compute delta energy if we flip cell u
// We need to consider: change in u's own energy + change in neighbors' cluster bonuses
ll compute_delta(int u) {
    int old_s = state[u];
    int new_s = old_s ^ 1;

    // Energy of u before flip
    ll e_before = cell_energy(u);

    // Energy of u after flip (simulate)
    state[u] = new_s;
    // Temporarily update counts for u itself
    // u's own counts don't change (they depend on neighbors' states)
    // But we need to recompute u's energy with new state
    ll e_after_u = 0;
    if (new_s == 0) {
        e_after_u = A[0][u];
        if (cnt1[0][u] == nbr_cnt[0][u]) e_after_u += A[2][u];
        if (cnt0[1][u] == nbr_cnt[1][u]) e_after_u += A[4][u];
        if (cnt1[2][u] == nbr_cnt[2][u]) e_after_u += A[6][u];
    } else {
        e_after_u = A[1][u];
        if (cnt0[0][u] == nbr_cnt[0][u]) e_after_u += A[3][u];
        if (cnt1[1][u] == nbr_cnt[1][u]) e_after_u += A[5][u];
        if (cnt0[2][u] == nbr_cnt[2][u]) e_after_u += A[7][u];
    }
    state[u] = old_s; // restore

    ll delta = e_after_u - e_before;

    // Now consider effect on neighbors
    // For each neighbor v of u (in all 3 clusters), flipping u changes v's counts
    // This may activate or deactivate v's cluster bonus
    for (int k = 0; k < 3; k++) {
        for (int v : nbr[k][u]) {
            int sv = state[v];
            int nc = nbr_cnt[k][v];
            if (nc == 0) continue;

            // Before flip of u: check if v's cluster k bonus was active
            bool was_active = false;
            if (k == 0) { // S1: all-opposite
                if (sv == 0) was_active = (cnt1[0][v] == nc);
                else         was_active = (cnt0[0][v] == nc);
            } else if (k == 1) { // S2: all-same
                if (sv == 0) was_active = (cnt0[1][v] == nc);
                else         was_active = (cnt1[1][v] == nc);
            } else { // S3: all-opposite
                if (sv == 0) was_active = (cnt1[2][v] == nc);
                else         was_active = (cnt0[2][v] == nc);
            }

            // After flip of u: u changes from old_s to new_s
            // For v's perspective in cluster k:
            //   cnt0[k][v] changes by (new_s==0?+1:-1) and cnt1[k][v] changes by (new_s==1?+1:-1)
            int new_cnt0 = cnt0[k][v] + (old_s == 0 ? -1 : 1);
            int new_cnt1 = cnt1[k][v] + (old_s == 1 ? -1 : 1);

            bool now_active = false;
            if (k == 0) { // S1: all-opposite
                if (sv == 0) now_active = (new_cnt1 == nc);
                else         now_active = (new_cnt0 == nc);
            } else if (k == 1) { // S2: all-same
                if (sv == 0) now_active = (new_cnt0 == nc);
                else         now_active = (new_cnt1 == nc);
            } else { // S3: all-opposite
                if (sv == 0) now_active = (new_cnt1 == nc);
                else         now_active = (new_cnt0 == nc);
            }

            if (was_active && !now_active) {
                delta -= A[2 + 2*k + (sv == 1 ? 1 : 0)][v];
            } else if (!was_active && now_active) {
                delta += A[2 + 2*k + (sv == 1 ? 1 : 0)][v];
            }
        }
    }

    return delta;
}

void do_flip(int u) {
    int old_s = state[u];
    int new_s = old_s ^ 1;
    state[u] = new_s;
    // Update neighbor counts
    for (int k = 0; k < 3; k++) {
        for (int v : nbr[k][u]) {
            if (old_s == 0) { cnt0[k][v]--; cnt1[k][v]++; }
            else            { cnt1[k][v]--; cnt0[k][v]++; }
        }
    }
}

ll compute_total_energy() {
    ll total = 0;
    for (int u = 0; u < V; u++) total += cell_energy(u);
    return total;
}

int coord_x[100], coord_y[100], coord_z[100];

void hill_climb(ll &cur_energy, int time_limit, chrono::steady_clock::time_point start_time) {
    bool improved = true;
    while (improved) {
        auto now = chrono::steady_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(now - start_time).count() >= time_limit) break;
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
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M >> H;
    V = N * M * H;

    for (int t = 0; t < 8; t++)
        for (int i = 0; i < V; i++)
            cin >> A[t][i];

    for (int x = 0; x < N; x++)
    for (int y = 0; y < M; y++)
    for (int z = 0; z < H; z++) {
        int u = (x * M + y) * H + z;
        coord_x[u] = x; coord_y[u] = y; coord_z[u] = z;
    }

    precompute_neighbors();

    mt19937 rng(42);
    auto start_time = chrono::steady_clock::now();
    auto elapsed_ms = [&]() {
        return chrono::duration_cast<chrono::milliseconds>(
            chrono::steady_clock::now() - start_time).count();
    };

    ll best_energy = LLONG_MIN;
    int best_saved[100];

    // Helper: save best state
    auto try_update_best = [&](ll e) {
        if (e > best_energy) {
            best_energy = e;
            memcpy(best_saved, state, sizeof(int) * V);
        }
    };

    // Phase 1: Try pattern-based starts with hill-climb (fast, gets good baseline)
    auto apply_pattern = [&](int pat) {
        for (int u = 0; u < V; u++) {
            int x = coord_x[u], y = coord_y[u], z = coord_z[u];
            switch(pat) {
                case 0: state[u] = 0; break;
                case 1: state[u] = 1; break;
                case 2: state[u] = (x+y+z) & 1; break;
                case 3: state[u] = 1 - ((x+y+z) & 1); break;
                case 4: state[u] = x & 1; break;
                case 5: state[u] = y & 1; break;
                case 6: state[u] = z & 1; break;
                case 7: state[u] = 1 - (x & 1); break;
            }
        }
    };

    for (int pat = 0; pat < 8 && elapsed_ms() < 800; pat++) {
        apply_pattern(pat);
        recount_all();
        ll cur = compute_total_energy();
        hill_climb(cur, 800, start_time);
        try_update_best(cur);
    }

    // Phase 2: SA restarts with time-based cooling, starting from best found
    const int TIME_LIMIT = 2500;

    while (elapsed_ms() < TIME_LIMIT) {
        long long time_left = TIME_LIMIT - elapsed_ms();
        if (time_left < 50) break;

        // Start from best known + small perturbation, or random
        int run_start = elapsed_ms();
        if (best_energy > LLONG_MIN && (rng() % 3) != 0) {
            // Start from best with perturbation
            memcpy(state, best_saved, sizeof(int) * V);
            recount_all();
            int perturb = max(1, V / 20);
            for (int i = 0; i < perturb; i++) {
                int u = rng() % V;
                do_flip(u);
            }
        } else {
            // Random start
            for (int u = 0; u < V; u++) state[u] = rng() & 1;
            recount_all();
        }

        ll cur_energy = compute_total_energy();
        ll run_best = cur_energy;

        // Time-based exponential cooling: T goes from T_start to T_end over allocated time
        // Allocate at most 1500ms for SA per run, rest for hill-climb
        long long sa_time = min(time_left - 30, (long long)1500);
        double T_start = 5e8;
        double T_end = 1e3;
        double log_ratio = log(T_end / T_start);

        long long sa_start = elapsed_ms();

        int iter = 0;
        while (true) {
            if ((iter & 1023) == 0) {
                long long now = elapsed_ms();
                long long sa_elapsed = now - sa_start;
                if (sa_elapsed >= sa_time) break;
            }

            int u = rng() % V;
            ll delta = compute_delta(u);

            if (delta >= 0) {
                do_flip(u);
                cur_energy += delta;
                if (cur_energy > run_best) run_best = cur_energy;
            } else {
                // Compute temperature based on progress
                long long now_ms = elapsed_ms();
                double progress = (double)(now_ms - sa_start) / (double)sa_time;
                if (progress > 1.0) progress = 1.0;
                double T = T_start * exp(log_ratio * progress);
                double prob = exp((double)delta / T);
                if ((double)(rng() & 0xFFFF) / 65536.0 < prob) {
                    do_flip(u);
                    cur_energy += delta;
                }
            }
            iter++;
        }

        // Hill-climb to local optimum
        hill_climb(cur_energy, TIME_LIMIT, start_time);
        if (cur_energy > run_best) run_best = cur_energy;
        try_update_best(run_best);
    }

    cout << best_energy << "\n";
    return 0;
}
