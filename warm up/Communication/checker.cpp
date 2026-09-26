#include "testlib.h"
using namespace std;

int main(int argc, char *argv[]) {
    registerTestlibCmd(argc, argv);
    int correct = ouf.readInt(0, 1, "correct");
    int total = ouf.readInt(1, 1, "total");
    if (!ouf.seekEof())
        quitf(_pe, "extra tokens in checker output");
    if (correct == total)
        quitf(_ok, "decoded correctly");
    quitf(_wa, "decoded incorrectly");
}
