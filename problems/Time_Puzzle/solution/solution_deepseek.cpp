#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        long long A, B;
        cin >> A >> B;

        // 先处理分钟进位
        A += B / 60;
        B %= 60;

        // 再处理小时取模
        A %= 24;

        cout << A << ' ' << B << '\n';
    }

    return 0;
}