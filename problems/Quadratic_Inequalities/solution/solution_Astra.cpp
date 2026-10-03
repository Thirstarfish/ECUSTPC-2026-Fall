#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 因为 a < 0，记 A = -a > 0。对整数 x：
//   a x^2 + b x + c > 0  <=>  a x^2 + b x + c >= 1
//                        <=>  A x^2 + B x + C' <= 0   (B = -b, C' = 1 - c)
// 两边同乘 4A > 0 配方：
//   (2Ax + B)^2 <= B^2 - 4A C' =: D
// 由于 2Ax + B 是整数，上式等价于 |2Ax + B| <= S，其中 S = floor(sqrt(D))。
// 于是每条不等式可行的整数集合恰好是一段连续闭区间
//   [ceil((b - S) / 2A), floor((b + S) / 2A)]，
// 全部求交即可。
ll floorDiv(ll p, ll q) // q > 0
{
    ll r = p / q;
    if (p % q != 0 && p < 0)
        --r;
    return r;
}

ll ceilDiv(ll p, ll q) // q > 0
{
    ll r = p / q;
    if (p % q != 0 && p > 0)
        ++r;
    return r;
}

ll isqrt(ll d)
{
    ll r = (ll)sqrtl((long double)d);
    while (r > 0 && r * r > d)
        --r;
    while ((r + 1) * (r + 1) <= d)
        ++r;
    return r;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--)
    {
        int n;
        cin >> n;
        ll lo = LLONG_MIN / 4, hi = LLONG_MAX / 4;
        bool ok = true;
        for (int i = 0; i < n; ++i)
        {
            ll a, b, c;
            cin >> a >> b >> c;
            if (!ok)
                continue;
            ll A = -a;
            ll D = b * b - 4 * a * c + 4 * a;
            if (D < 0)
            {
                ok = false;
                continue;
            }
            ll S = isqrt(D);
            ll l = ceilDiv(b - S, 2 * A);
            ll r = floorDiv(b + S, 2 * A);
            if (l > r)
            {
                ok = false;
                continue;
            }
            lo = max(lo, l);
            hi = min(hi, r);
        }
        cout << (ok && lo <= hi ? "YES\n" : "NO\n");
    }
}
