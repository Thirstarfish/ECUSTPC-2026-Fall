#include "testlib.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Test {
    vector<int> a;
};

enum class Order {
    Shuffled,
    Split,
    Ascending,
    Descending,
    Adjacent
};

int readIntArg(int argc, char* argv[], int pos, int low, int high, const string& name) {
    ensuref(pos < argc, "missing argument: %s", name.c_str());
    char* end = nullptr;
    long long value = strtoll(argv[pos], &end, 10);
    ensuref(end != argv[pos] && *end == '\0', "argument %s must be an integer", name.c_str());
    ensuref(low <= value && value <= high, "argument %s must be in [%d, %d]", name.c_str(), low, high);
    return (int)value;
}

void rndShuffle(vector<int>& a) {
    for (int i = (int)a.size() - 1; i > 0; --i) {
        swap(a[i], a[rnd.next(0, i)]);
    }
}

void applyOrder(vector<int>& a, Order order) {
    if (order == Order::Shuffled) {
        rndShuffle(a);
    } else if (order == Order::Ascending) {
        sort(a.begin(), a.end());
    } else if (order == Order::Descending) {
        sort(a.rbegin(), a.rend());
    }
}

vector<int> makeYesPairs(int n, int maxValue, Order order) {
    ensuref(n >= 2, "YES pair construction needs n >= 2");
    maxValue = min(maxValue, n);
    ensuref(maxValue >= 1, "maxValue must be positive");

    vector<int> left, right, a;
    if (n % 2 == 1) {
        ensuref(n >= 3, "odd YES construction needs n >= 3");
        ensuref(n >= 2, "n must allow value 2");
        a.push_back(1);
        a.push_back(1);
        a.push_back(2);
        n -= 3;
    }

    for (int i = 0; i < n / 2; ++i) {
        int v = rnd.next(1, maxValue);
        if (order == Order::Adjacent) {
            a.push_back(v);
            a.push_back(v);
        } else {
            left.push_back(v);
            right.push_back(v);
        }
    }

    if (order != Order::Adjacent) {
        a.insert(a.end(), left.begin(), left.end());
        a.insert(a.end(), right.begin(), right.end());
    }
    applyOrder(a, order);
    return a;
}

vector<int> makeNoOdd(int n, int maxValue) {
    ensuref(n >= 1, "n must be positive");
    maxValue = min(maxValue, n);
    ensuref(maxValue >= 1, "maxValue must be positive");

    vector<int> a(n);
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        a[i] = rnd.next(1, maxValue);
        sum += a[i];
    }

    if (sum % 2 == 0) {
        bool changed = false;
        for (int i = 0; i < n && !changed; ++i) {
            if (a[i] < maxValue) {
                ++a[i];
                changed = true;
            } else if (a[i] > 1) {
                --a[i];
                changed = true;
            }
        }
        ensuref(changed, "cannot force odd sum with these parameters");
    }

    rndShuffle(a);
    return a;
}

vector<int> makeNoGcd(int n, int value) {
    ensuref(n >= 1 && n <= 5000, "n must be in [1, 5000]");
    ensuref(n % 2 == 1, "no-gcd needs odd n");
    ensuref(value >= 2 && value <= n && value % 2 == 0, "value must be even and in [2, n]");
    return vector<int>(n, value);
}

vector<int> makeNoResidue(int n) {
    ensuref(n >= 4 && n % 2 == 0, "no-residue needs even n >= 4");
    vector<int> a;
    a.reserve(n);
    a.push_back(1);
    a.push_back(3);
    while ((int)a.size() < n) {
        a.push_back(4);
    }
    rndShuffle(a);
    return a;
}

vector<int> makeAllSame(int n, int value) {
    ensuref(n >= 1 && n <= 5000, "n must be in [1, 5000]");
    ensuref(value >= 1 && value <= n, "value must be in [1, n]");
    return vector<int>(n, value);
}

vector<int> makeRandomCase(int n, int maxValue) {
    ensuref(n >= 1 && n <= 5000, "n must be in [1, 5000]");
    maxValue = min(maxValue, n);
    ensuref(maxValue >= 1, "maxValue must be positive");
    vector<int> a(n);
    for (int& x : a) {
        x = rnd.next(1, maxValue);
    }
    return a;
}

vector<int> chooseNoCase(int n) {
    for (int tries = 0; tries < 5; ++tries) {
        int kind = rnd.next(0, 2);
        if (kind == 0) {
            return makeNoOdd(n, n);
        }
        if (kind == 1 && n % 2 == 1 && n >= 3) {
            return makeNoGcd(n, 2);
        }
        if (kind == 2 && n % 2 == 0 && n >= 4) {
            return makeNoResidue(n);
        }
    }
    return makeNoOdd(n, n);
}

void addTest(vector<Test>& tests, const vector<int>& a) {
    ensuref(!a.empty(), "empty test case");
    int n = (int)a.size();
    ensuref(n >= 1 && n <= 5000, "n out of bounds");
    for (int i = 0; i < n; ++i) {
        ensuref(1 <= a[i] && a[i] <= n, "a[%d]=%d is outside [1, n=%d]", i + 1, a[i], n);
    }

    int totalN = n;
    for (const Test& test : tests) {
        totalN += (int)test.a.size();
    }
    ensuref((int)tests.size() + 1 <= 1000, "too many test cases");
    ensuref(totalN <= 5000, "sum of n exceeds 5000");
    tests.push_back({a});
}

void printTests(const vector<Test>& tests) {
    cout << tests.size() << '\n';
    for (const Test& test : tests) {
        cout << test.a.size() << '\n';
        for (int i = 0; i < (int)test.a.size(); ++i) {
            if (i) {
                cout << ' ';
            }
            cout << test.a[i];
        }
        cout << '\n';
    }
}

vector<Test> makeSample(int id) {
    vector<Test> tests;
    if (id == 1) {
        addTest(tests, vector<int>{1});
        addTest(tests, vector<int>{1, 1, 2, 2});
        addTest(tests, vector<int>{1, 3, 4, 4, 4, 4});
    } else if (id == 2) {
        addTest(tests, vector<int>{1, 1});
        addTest(tests, vector<int>{2, 2, 2});
        addTest(tests, vector<int>{1, 1, 2, 3, 3});
        addTest(tests, vector<int>{2, 5, 3, 4, 1, 5});
    } else {
        ensuref(false, "unknown sample id");
    }
    return tests;
}

vector<Test> makeMixed(int totalN, int t, int maxValue, int yesPercent) {
    ensuref(1 <= t && t <= 1000, "t must be in [1, 1000]");
    ensuref(t <= totalN && totalN <= 5000, "need t <= totalN <= 5000");
    ensuref(1 <= maxValue && maxValue <= 5000, "maxValue must be in [1, 5000]");
    ensuref(0 <= yesPercent && yesPercent <= 100, "yesPercent must be in [0, 100]");

    vector<int> sizes(t, 1);
    for (int extra = totalN - t; extra > 0; --extra) {
        ++sizes[rnd.next(0, t - 1)];
    }
    rndShuffle(sizes);

    vector<Test> tests;
    for (int n : sizes) {
        bool wantYes = (rnd.next(0, 99) < yesPercent);
        if (wantYes && n >= 2) {
            addTest(tests, makeYesPairs(n, maxValue, Order::Shuffled));
        } else {
            addTest(tests, chooseNoCase(n));
        }
    }
    return tests;
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);

    ensuref(argc >= 2, "usage: gen <mode> [params...]");
    string mode = argv[1];
    vector<Test> tests;

    if (mode == "sample") {
        int id = readIntArg(argc, argv, 2, 1, 2, "id");
        tests = makeSample(id);
    } else if (mode == "yes-pairs") {
        int n = readIntArg(argc, argv, 2, 2, 5000, "n");
        int maxValue = readIntArg(argc, argv, 3, 1, 5000, "maxValue");
        addTest(tests, makeYesPairs(n, maxValue, Order::Shuffled));
    } else if (mode == "yes-split") {
        int n = readIntArg(argc, argv, 2, 2, 5000, "n");
        int maxValue = readIntArg(argc, argv, 3, 1, 5000, "maxValue");
        addTest(tests, makeYesPairs(n, maxValue, Order::Split));
    } else if (mode == "yes-adjacent") {
        int n = readIntArg(argc, argv, 2, 2, 5000, "n");
        int maxValue = readIntArg(argc, argv, 3, 1, 5000, "maxValue");
        addTest(tests, makeYesPairs(n, maxValue, Order::Adjacent));
    } else if (mode == "yes-ascending") {
        int n = readIntArg(argc, argv, 2, 2, 5000, "n");
        int maxValue = readIntArg(argc, argv, 3, 1, 5000, "maxValue");
        addTest(tests, makeYesPairs(n, maxValue, Order::Ascending));
    } else if (mode == "yes-descending") {
        int n = readIntArg(argc, argv, 2, 2, 5000, "n");
        int maxValue = readIntArg(argc, argv, 3, 1, 5000, "maxValue");
        addTest(tests, makeYesPairs(n, maxValue, Order::Descending));
    } else if (mode == "no-odd") {
        int n = readIntArg(argc, argv, 2, 1, 5000, "n");
        int maxValue = readIntArg(argc, argv, 3, 1, 5000, "maxValue");
        addTest(tests, makeNoOdd(n, maxValue));
    } else if (mode == "no-gcd") {
        int n = readIntArg(argc, argv, 2, 1, 5000, "n");
        int value = readIntArg(argc, argv, 3, 1, 5000, "value");
        addTest(tests, makeNoGcd(n, value));
    } else if (mode == "no-residue") {
        int n = readIntArg(argc, argv, 2, 4, 5000, "n");
        addTest(tests, makeNoResidue(n));
    } else if (mode == "all") {
        int n = readIntArg(argc, argv, 2, 1, 5000, "n");
        int value = readIntArg(argc, argv, 3, 1, 5000, "value");
        addTest(tests, makeAllSame(n, value));
    } else if (mode == "random") {
        int n = readIntArg(argc, argv, 2, 1, 5000, "n");
        int maxValue = readIntArg(argc, argv, 3, 1, 5000, "maxValue");
        addTest(tests, makeRandomCase(n, maxValue));
    } else if (mode == "mixed") {
        int totalN = readIntArg(argc, argv, 2, 1, 5000, "totalN");
        int t = readIntArg(argc, argv, 3, 1, 1000, "t");
        int maxValue = readIntArg(argc, argv, 4, 1, 5000, "maxValue");
        int yesPercent = readIntArg(argc, argv, 5, 0, 100, "yesPercent");
        tests = makeMixed(totalN, t, maxValue, yesPercent);
    } else {
        ensuref(false, "unknown mode: %s", mode.c_str());
    }

    printTests(tests);
    return 0;
}
