#include <bits/stdc++.h>
using namespace std;
#define int long long

signed main()
{
    string mode;
    cin >> mode;
    if (mode == "send")
    {
        int n;
        cin >> n;
        string ans;
        for (int i = 1; i <= 13; i++)
        {
            ans.push_back(n % 26 + 'a');
            n /= 26;
        }
        cout << ans << endl;
    }
    else
    {
        string s;
        cin >> s;
        int ans = 0;
        for (auto c : s | std::views::reverse)
        {
            ans *= 26;
            ans += c - 'a';
        }
        cout << ans << endl;
    }
    return 0;
}