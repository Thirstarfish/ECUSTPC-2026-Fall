/**
 * Validator for "Quantum Stabilizer Initialization"
 *
 * Input Format:
 *   Line 1: N M H
 *   Following: 8 energy matrices A_1, A_2, ..., A_8
 *
 *   Each matrix consists of:
 *     - N blocks (one per x-coordinate)
 *     - Each block has M lines (one per y-coordinate)
 *     - Each line has H space-separated integers (z-coordinates)
 *     - Blocks within a matrix are separated by exactly one blank line
 *     - Matrices are separated by exactly one blank line
 *
 * Constraints:
 *   - 1 <= N, M, H <= 20
 *   - N * M * H <= 8000
 *   - All energy values in [0, 10^9]
 */

#include "testlib.h"
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    // Read grid dimensions
    int N = inf.readInt(1, 20, "N");
    inf.readSpace();
    int M = inf.readInt(1, 20, "M");
    inf.readSpace();
    int H = inf.readInt(1, 20, "H");
    inf.readEoln();

    // Validate total grid size
    long long totalSize = (long long)N * M * H;
    ensuref(totalSize <= 8000,
            "N*M*H must be <= 8000, but got %d*%d*%d = %lld",
            N, M, H, totalSize);

    // Read 8 energy matrices
    for (int matrixId = 1; matrixId <= 8; ++matrixId) {
        // Read N blocks for this matrix
        for (int x = 1; x <= N; ++x) {
            // Read M lines in this block
            for (int y = 1; y <= M; ++y) {
                // Read H integers in this line
                for (int z = 1; z <= H; ++z) {
                    if (z > 1) {
                        inf.readSpace();
                    }

                    // Create descriptive name for error messages
                    string varName = format("A_%d[%d][%d][%d]", matrixId, x, y, z);

                    // Read energy value in range [0, 10^9]
                    inf.readLong(0LL, 1000000000LL, varName);
                }
                inf.readEoln();
            }

            // Blank line between blocks (except after last block)
            if (x < N) {
                inf.readEoln();
            }
        }

        // Blank line between matrices (except after last matrix)
        if (matrixId < 8) {
            inf.readEoln();
        }
    }

    // Ensure no trailing content
    inf.readEof();

    return 0;
}
