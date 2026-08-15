#include "testlib.h"
using namespace std;

int main(int argc, char *argv[])
{
    registerTestlibCmd(argc, argv);

    int correct = ouf.readInt(1, 800000, "correct");
    int total = ouf.readInt(1, 800000, "total");

    if (correct == total)
        quitf(_ok, "Perfect! All %d queries answered correctly", total);
    else
    {
        double ratio = (double)correct / total;
        quitf(_wa, "Correct: %d/%d (%.10f%%)", correct, total, ratio * 100);
    }

    return 0;
}