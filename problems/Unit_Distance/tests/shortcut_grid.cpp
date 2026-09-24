#include <bits/stdc++.h>
using namespace std;

// No binary-vector subset sums, square roots, or floating-point arithmetic.
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        int w = 1;
        while (w * w < n) ++w;
        for (int i = 0; i < n; ++i) {
            long long r = i / w, c = i % w;
            if (n < 100) {
                cout << c * 10000000000000LL + r * 5000000000000LL
                     << ' ' << r * 8660254037844LL << '\n';
            } else {
                cout << c * 2000000000000LL << ' '
                     << r * 2000000000000LL << '\n';
            }
        }
    }
}
