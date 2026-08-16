#include "testlib.h"
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    int t = inf.readInt(1, 1000, "t");
    inf.readEoln();

    int total_n = 0;

    for (int test = 1; test <= t; test++) {
        setTestCase(test);

        int n = inf.readInt(1, 5000, "n");
        inf.readEoln();

        total_n += n;
        ensuref(total_n <= 5000, "Sum of n exceeds 5000");

        for (int i = 0; i < n; i++) {
            inf.readInt(1, n, format("a[%d]", i + 1));
            if (i < n - 1) {
                inf.readSpace();
            }
        }
        inf.readEoln();
    }

    inf.readEof();

    return 0;
}
