/**
 * Anti-Random Generator for "Quantum Stabilizer Initialization"
 * Constructs adversarial cases where random approaches fail
 *
 * Usage:
 *   gen-anti <N> <M> <H> <seed> <pattern>
 *
 * Patterns:
 *   0 - checkerboard: only alternating 0/1 pattern achieves high energy
 *   1 - layered: specific layers must be all-0 or all-1
 *   2 - symmetric-trap: symmetric input but asymmetric optimal solution
 *   3 - greedy-trap: greedy leads to local optimum far from global
 *   4 - sparse-critical: only a few critical bits matter, rest are noise
 */

#include "testlib.h"
#include <iostream>
#include <vector>

using namespace std;

void generateCheckerboard(int N, int M, int H, int seed) {
    // Pattern: (x+y+z)%2 == 0 should be |0⟩, else |1⟩
    // Give huge bonus only when this exact pattern is followed

    long long HUGE = 1000000000LL;
    long long TINY = 1LL;

    cout << N << " " << M << " " << H << endl;

    for (int matrixId = 1; matrixId <= 8; ++matrixId) {
        for (int x = 1; x <= N; ++x) {
            for (int y = 1; y <= M; ++y) {
                for (int z = 1; z <= H; ++z) {
                    if (z > 1) cout << " ";

                    int parity = (x + y + z) % 2;
                    long long energy = 0;

                    // Matrix 1 (A1): huge if correct parity for |0⟩
                    // Matrix 2 (A2): huge if correct parity for |1⟩
                    if (matrixId == 1) {
                        energy = (parity == 0) ? HUGE : TINY;
                    } else if (matrixId == 2) {
                        energy = (parity == 1) ? HUGE : TINY;
                    } else {
                        // Other matrices: small random noise
                        energy = rnd.next(0LL, 1000LL);
                    }

                    cout << energy;
                }
                cout << endl;
            }
            if (x < N) cout << endl;
        }
        if (matrixId < 8) cout << endl;
    }
}

void generateLayered(int N, int M, int H, int seed) {
    // Layers z=1,3,5,... must be all |0⟩, z=2,4,6,... must be all |1⟩
    // Heavy penalty for breaking this structure

    long long HUGE = 1000000000LL;
    long long PENALTY = 500000000LL;

    cout << N << " " << M << " " << H << endl;

    for (int matrixId = 1; matrixId <= 8; ++matrixId) {
        for (int x = 1; x <= N; ++x) {
            for (int y = 1; y <= M; ++y) {
                for (int z = 1; z <= H; ++z) {
                    if (z > 1) cout << " ";

                    long long energy = 0;

                    if (matrixId == 1) {
                        // A1: reward |0⟩ on odd layers
                        energy = (z % 2 == 1) ? HUGE : 0;
                    } else if (matrixId == 2) {
                        // A2: reward |1⟩ on even layers
                        energy = (z % 2 == 0) ? HUGE : 0;
                    } else if (matrixId == 5 || matrixId == 6) {
                        // S2 stabilizers: require layer consistency
                        energy = PENALTY;
                    } else {
                        energy = rnd.next(0LL, 10000LL);
                    }

                    cout << energy;
                }
                cout << endl;
            }
            if (x < N) cout << endl;
        }
        if (matrixId < 8) cout << endl;
    }
}

void generateGreedyTrap(int N, int M, int H, int seed) {
    // Greedy on A1/A2 leads to suboptimal; need to sacrifice for S2/S3 bonuses

    long long BASE = 100000LL;
    long long STAB_BONUS = 900000000LL;

    cout << N << " " << M << " " << H << endl;

    for (int matrixId = 1; matrixId <= 8; ++matrixId) {
        for (int x = 1; x <= N; ++x) {
            for (int y = 1; y <= M; ++y) {
                for (int z = 1; z <= H; ++z) {
                    if (z > 1) cout << " ";

                    long long energy = 0;

                    if (matrixId == 1) {
                        // A1: small reward for |0⟩
                        energy = BASE + rnd.next(-1000LL, 1000LL);
                    } else if (matrixId == 2) {
                        // A2: small reward for |1⟩ (slightly higher to bait greedy)
                        energy = BASE + 1000 + rnd.next(-1000LL, 1000LL);
                    } else if (matrixId == 5) {
                        // A5: huge bonus for all |0⟩ in S2 (but needs sacrifice on A2)
                        energy = STAB_BONUS;
                    } else if (matrixId == 6) {
                        // A6: huge bonus for all |1⟩ in S2
                        energy = STAB_BONUS;
                    } else {
                        energy = rnd.next(0LL, 10000LL);
                    }

                    cout << energy;
                }
                cout << endl;
            }
            if (x < N) cout << endl;
        }
        if (matrixId < 8) cout << endl;
    }
}

void generateSparseCritical(int N, int M, int H, int seed) {
    // Only center region matters; boundary is noise
    // Random approaches waste time on irrelevant regions

    int cx = N / 2 + 1;
    int cy = M / 2 + 1;
    int cz = H / 2 + 1;
    int radius = min({N, M, H}) / 3 + 1;

    long long HUGE = 1000000000LL;
    long long NOISE = 1000LL;

    cout << N << " " << M << " " << H << endl;

    for (int matrixId = 1; matrixId <= 8; ++matrixId) {
        for (int x = 1; x <= N; ++x) {
            for (int y = 1; y <= M; ++y) {
                for (int z = 1; z <= H; ++z) {
                    if (z > 1) cout << " ";

                    int dist2 = (x - cx) * (x - cx) + (y - cy) * (y - cy) + (z - cz) * (z - cz);
                    bool inCenter = (dist2 <= radius * radius);

                    long long energy = 0;

                    if (inCenter && (matrixId == 1 || matrixId == 2)) {
                        // Center: high energy
                        energy = HUGE / 2 + rnd.next(-HUGE/10, HUGE/10);
                    } else {
                        // Boundary: pure noise
                        energy = rnd.next(0LL, NOISE);
                    }

                    cout << energy;
                }
                cout << endl;
            }
            if (x < N) cout << endl;
        }
        if (matrixId < 8) cout << endl;
    }
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);

    ensure(argc >= 6, "Usage: gen-anti <N> <M> <H> <seed> <pattern>");

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int H = atoi(argv[3]);
    int pattern = atoi(argv[5]);

    ensure(N >= 1 && N <= 20, "N must be in [1, 20]");
    ensure(M >= 1 && M <= 20, "M must be in [1, 20]");
    ensure(H >= 1 && H <= 20, "H must be in [1, 20]");
    ensure((long long)N * M * H <= 8000, "N*M*H must be <= 8000");
    ensure(pattern >= 0 && pattern <= 4, "pattern must be in [0, 4]");

    switch (pattern) {
        case 0:
            generateCheckerboard(N, M, H, atoi(argv[4]));
            break;
        case 1:
            generateLayered(N, M, H, atoi(argv[4]));
            break;
        case 2:
            generateGreedyTrap(N, M, H, atoi(argv[4]));
            break;
        case 3:
            generateGreedyTrap(N, M, H, atoi(argv[4]));  // reuse for now
            break;
        case 4:
            generateSparseCritical(N, M, H, atoi(argv[4]));
            break;
    }

    return 0;
}
