#include <bits/stdc++.h>
using namespace std;

using i128 = __int128_t;
static constexpr long long MAX_X = 1000000000000000000LL;
static constexpr int LEN = 13;

string encode(long long x) {
    string s(LEN, 'a');
    for (int i = LEN - 1; i >= 0; --i) {
        s[i] = char('a' + x % 26);
        x /= 26;
    }
    return s;
}

long long decode(const string &s) {
    i128 x = 0;
    for (char c : s) x = x * 26 + (c - 'a');
    return (long long)x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string mode;
    cin >> mode;
    if (mode == "send") {
        long long x;
        cin >> x;
        cout << encode(x) << '\n';
    } else {
        string s;
        cin >> s;
        cout << decode(s) << '\n';
    }
}
