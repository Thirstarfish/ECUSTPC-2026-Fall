/**
 * Generator for "Quantum Stabilizer Initialization"
 *
 * Usage:
 *   gen <N> <M> <H> <seed> [max_energy]
 *
 * Parameters:
 *   N, M, H: grid dimensions (1 <= N,M,H <= 20, N*M*H <= 8000)
 *   seed: random seed for reproducibility
 *   max_energy: maximum energy value (default: 1000000000, i.e., 10^9)
 *
 * Examples:
 *   gen 5 5 5 123 > 1
 *   gen 20 20 20 456 1000 > 2
 *   gen 10 10 8 789 > 3
 */

#include "testlib.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);

    // Read parameters
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int H = atoi(argv[3]);

    // Optional: max_energy (default 10^9)
    long long maxEnergy = 1000000000LL;
    if (argc >= 6) {
        maxEnergy = atoll(argv[5]);
    }

    // Validate parameters
    ensure(N >= 1 && N <= 20, "N must be in [1, 20]");
    ensure(M >= 1 && M <= 20, "M must be in [1, 20]");
    ensure(H >= 1 && H <= 20, "H must be in [1, 20]");
    ensure((long long)N * M * H <= 8000, "N*M*H must be <= 8000");
    ensure(maxEnergy >= 0 && maxEnergy <= 1000000000LL, "maxEnergy must be in [0, 10^9]");

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

                    // Generate random energy value in [0, maxEnergy]
                    long long energy = rnd.next(0LL, maxEnergy);
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
