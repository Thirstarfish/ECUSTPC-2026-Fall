/**
 * Advanced Generator for "Quantum Stabilizer Initialization"
 *
 * Usage:
 *   gen-adv <N> <M> <H> <seed> <strategy> [params...]
 *
 * Parameters:
 *   N, M, H: grid dimensions (1 <= N,M,H <= 20, N*M*H <= 8000)
 *   seed: random seed for reproducibility
 *   strategy: data generation strategy
 *     0 - uniform random in [0, 10^9]
 *     1 - small values [0, 100]
 *     2 - large values [10^8, 10^9]
 *     3 - sparse (mostly zeros with some large values)
 *     4 - biased (favor one matrix over others)
 *     5 - custom range [min, max] (requires 2 extra params)
 *
 * Examples:
 *   gen-adv 5 5 5 123 0 > 1           (uniform random)
 *   gen-adv 10 10 8 456 1 > 2         (small values)
 *   gen-adv 20 20 20 789 2 > 3        (large values)
 *   gen-adv 15 15 10 111 3 > 4        (sparse)
 *   gen-adv 8 8 8 222 4 > 5           (biased)
 *   gen-adv 12 12 12 333 5 1000 5000 > 6  (custom range [1000, 5000])
 */

#include "testlib.h"
#include <iostream>
#include <vector>

using namespace std;

long long generateEnergy(int strategy, int matrixId, long long minVal, long long maxVal) {
    switch (strategy) {
        case 0: // Uniform random [0, 10^9]
            return rnd.next(0LL, 1000000000LL);

        case 1: // Small values [0, 100]
            return rnd.next(0LL, 100LL);

        case 2: // Large values [10^8, 10^9]
            return rnd.next(100000000LL, 1000000000LL);

        case 3: // Sparse: 80% chance of 0, 20% chance of large value
            if (rnd.next(100) < 80) {
                return 0LL;
            } else {
                return rnd.next(100000000LL, 1000000000LL);
            }

        case 4: // Biased: favor certain matrices
            // Matrices 1-2 (eigenstate): medium values
            // Matrices 3-4 (S1): higher values
            // Matrices 5-6 (S2): lower values
            // Matrices 7-8 (S3): medium values
            if (matrixId <= 2) {
                return rnd.next(1000LL, 100000LL);
            } else if (matrixId <= 4) {
                return rnd.next(100000LL, 1000000000LL);
            } else if (matrixId <= 6) {
                return rnd.next(0LL, 10000LL);
            } else {
                return rnd.next(10000LL, 500000LL);
            }

        case 5: // Custom range
            return rnd.next(minVal, maxVal);

        default:
            return rnd.next(0LL, 1000000000LL);
    }
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);

    // Read parameters
    ensure(argc >= 5, "Usage: gen-adv <N> <M> <H> <seed> <strategy> [params...]");

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int H = atoi(argv[3]);
    int strategy = atoi(argv[5]);

    // Validate parameters
    ensure(N >= 1 && N <= 20, "N must be in [1, 20]");
    ensure(M >= 1 && M <= 20, "M must be in [1, 20]");
    ensure(H >= 1 && H <= 20, "H must be in [1, 20]");
    ensure((long long)N * M * H <= 8000, "N*M*H must be <= 8000");
    ensure(strategy >= 0 && strategy <= 5, "strategy must be in [0, 5]");

    long long minVal = 0, maxVal = 1000000000LL;
    if (strategy == 5) {
        ensure(argc >= 8, "Strategy 5 requires min and max parameters");
        minVal = atoll(argv[6]);
        maxVal = atoll(argv[7]);
        ensure(minVal >= 0 && minVal <= 1000000000LL, "minVal must be in [0, 10^9]");
        ensure(maxVal >= 0 && maxVal <= 1000000000LL, "maxVal must be in [0, 10^9]");
        ensure(minVal <= maxVal, "minVal must be <= maxVal");
    }

    // Output grid dimensions
    cout << N << " " << M << " " << H << endl;

    // Generate 8 energy matrices
    for (int matrixId = 1; matrixId <= 8; ++matrixId) {
        // Generate N blocks
        for (int x = 1; x <= N; ++x) {
            // Generate M lines
            for (int y = 1; y <= M; ++y) {
                // Generate H integers
                for (int z = 1; z <= H; ++z) {
                    if (z > 1) cout << " ";

                    // Generate energy value based on strategy
                    long long energy = generateEnergy(strategy, matrixId, minVal, maxVal);
                    cout << energy;
                }
                cout << endl;
            }

            // Blank line between blocks (except after last block)
            if (x < N) {
                cout << endl;
            }
        }

        // Blank line between matrices (except after last matrix)
        if (matrixId < 8) {
            cout << endl;
        }
    }

    return 0;
}
