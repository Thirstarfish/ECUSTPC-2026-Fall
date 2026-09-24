#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    cout << fixed << setprecision(13);

    const long double EPSILON = 0.0002L;
    while (T--)
    {
        int n;
        cin >> n;

        int dimensions = 0;
        while ((1 << dimensions) < n)
            ++dimensions;

        vector<long double> vx(dimensions);
        for (int bit = 0; bit < dimensions; ++bit)
        {
            long double vy = EPSILON * (1 << bit);
            vx[bit] = sqrtl(1.0L - vy * vy);
        }

        for (int index = 0; index < n; ++index)
        {
            long double x = 0.0L;
            for (int bit = 0; bit < dimensions; ++bit)
                if ((index >> bit) & 1)
                    x += vx[bit];
            long double y = EPSILON * index;
            cout << x << ' ' << y << '\n';
        }
    }
    return 0;
}
