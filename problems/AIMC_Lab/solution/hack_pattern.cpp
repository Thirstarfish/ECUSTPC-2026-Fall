#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// 3D grid quantum stabilizer - Pattern-based with greedy refinement

int N, M, H, V;
ll A[8][100];
int state[100], best_state[100];

vector<int> nbr[3][100];
int nbr_cnt[3][100];

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

int cnt0[3][100], cnt1[3][100];

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

ll cell_energy(int u) {
    int s = state[u];
    ll e = (s == 0) ? A[0][u] : A[1][u];
    if (s == 0 && cnt1[0][u] == nbr_cnt[0][u]) e += A[2][u];
    if (s == 1 && cnt0[0][u] == nbr_cnt[0][u]) e += A[3][u];
    if (s == 0 && cnt0[1][u] == nbr_cnt[1][u]) e += A[4][u];
    if (s == 1 && cnt1[1][u] == nbr_cnt[1][u]) e += A[5][u];
    if (s == 0 && cnt1[2][u] == nbr_cnt[2][u]) e += A[6][u];
    if (s == 1 && cnt0[2][u] == nbr_cnt[2][u]) e += A[7][u];
    return e;
}

ll compute_delta(int u) {
    int s = state[u];
    ll e_before = cell_energy(u);

    ll nb_before = 0;
    for (int k = 0; k < 3; k++) {
        for (int v : nbr[k][u]) {
            int sv = state[v];
            if (k == 0 || k == 2) {
                if (sv == 0 && cnt1[k][v] == nbr_cnt[k][v]) nb_before += A[2+2*k][v];
                if (sv == 1 && cnt0[k][v] == nbr_cnt[k][v]) nb_before += A[2+2*k+1][v];
            } else {
                if (sv == 0 && cnt0[k][v] == nbr_cnt[k][v]) nb_before += A[2+2*k][v];
                if (sv == 1 && cnt1[k][v] == nbr_cnt[k][v]) nb_before += A[2+2*k+1][v];
            }
        }
    }

    for (int k = 0; k < 3; k++) {
        for (int v : nbr[k][u]) {
            if (s == 0) { cnt0[k][v]--; cnt1[k][v]++; }
            else { cnt1[k][v]--; cnt0[k][v]++; }
        }
    }
    state[u] = 1 - s;

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

    // Undo
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

// Apply greedy hill-climbing refinement
ll refine(int time_limit_ms, chrono::steady_clock::time_point start_time) {
    compute_counts();
    ll cur = compute_total_energy();
    bool improved = true;
    while (improved) {
        auto now = chrono::steady_clock::now();
        if (chrono::duration_cast<chrono::milliseconds>(now - start_time).count() >= time_limit_ms)
            break;
        improved = false;
        for (int u = 0; u < V; u++) {
            ll delta = compute_delta(u);
            if (delta > 0) {
                do_flip(u);
                cur += delta;
                improved = true;
            }
        }
    }
    return cur;
}

int coord_x[100], coord_y[100], coord_z[100];

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
        int u = cid(x, y, z);
        coord_x[u] = x; coord_y[u] = y; coord_z[u] = z;
    }

    precompute_neighbors();

    auto start_time = chrono::steady_clock::now();
    auto elapsed_ms = [&]() {
        return chrono::duration_cast<chrono::milliseconds>(
            chrono::steady_clock::now() - start_time).count();
    };

    ll best_energy = LLONG_MIN;

    // Pattern generators
    // 0: all-0
    // 1: all-1
    // 2: checkerboard (x+y+z)%2 == 0 -> 0, else 1
    // 3: checkerboard inverted
    // 4: x-layer alternating
    // 5: y-layer alternating
    // 6: z-layer alternating
    // 7: x-layer alternating inverted
    // 8: y-layer alternating inverted
    // 9: z-layer alternating inverted
    // 10: 2x2x2 blocks of same value (like all-same for S2)
    // 11: 2x2x2 blocks inverted

    auto apply_pattern = [&](int pat) {
        for (int u = 0; u < V; u++) {
            int x = coord_x[u], y = coord_y[u], z = coord_z[u];
            switch(pat) {
                case 0: state[u] = 0; break;
                case 1: state[u] = 1; break;
                case 2: state[u] = (x+y+z) % 2; break;
                case 3: state[u] = 1 - (x+y+z) % 2; break;
                case 4: state[u] = x % 2; break;
                case 5: state[u] = y % 2; break;
                case 6: state[u] = z % 2; break;
                case 7: state[u] = 1 - x % 2; break;
                case 8: state[u] = 1 - y % 2; break;
                case 9: state[u] = 1 - z % 2; break;
                case 10: state[u] = ((x/2) + (y/2) + (z/2)) % 2; break;
                case 11: state[u] = 1 - ((x/2) + (y/2) + (z/2)) % 2; break;
            }
        }
    };

    int num_patterns = 12;

    for (int pat = 0; pat < num_patterns && elapsed_ms() < 2500; pat++) {
        apply_pattern(pat);
        ll energy = refine(2500, start_time);
        if (energy > best_energy) {
            best_energy = energy;
            memcpy(best_state, state, sizeof(int) * V);
        }
    }

    cout << best_energy << "\n";
    return 0;
}
