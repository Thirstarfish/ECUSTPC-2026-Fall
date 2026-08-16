#include <bits/stdc++.h>
using namespace std;

#define int long long
using mat = vector<vector<int>>;
const int mod = 998244353;

struct ahocorasick
{
    static constexpr int sz = 26;
    struct node
    {
        int len;
        int link;
        bool end;
        array<int, sz> next;
        node() : len{0}, link{0}, end(0), next{} {}
    };
    vector<node> t;

    void init()
    {
        t.assign(2, node());
        t[0].next.fill(1);
        t[0].len = -1;
    }

    ahocorasick() { init(); };

    int newnode()
    {
        t.emplace_back();
        return t.size() - 1;
    }

    void add(const string &a)
    {
        int p = 1;
        for (auto c : a)
        {
            int x = c - 'a';
            if (t[p].next[x] == 0)
            {
                t[p].next[x] = newnode();
                t[t[p].next[x]].len = t[p].len + 1;
            }
            p = t[p].next[x];
        }
        t[p].end = 1;
    }

    void work()
    {
        queue<int> q;
        q.push(1);
        while (!q.empty())
        {
            int x = q.front();
            q.pop();
            for (int i = 0; i < sz; i++)
            {
                if (t[x].next[i] == 0)
                    t[x].next[i] = t[t[x].link].next[i];
                else
                {
                    t[t[x].next[i]].link = t[t[x].link].next[i];
                    t[t[x].next[i]].end |= t[t[t[x].next[i]].link].end;
                    q.push(t[x].next[i]);
                }
            }
        }
    }
};

void cold_beans()
{
    int l, k, n;
    cin >> l >> k >> n;
    vector<pair<int, char>> a(k + 1);
    for (int i = 1; i <= k; i++)
        cin >> a[i].first >> a[i].second;
    sort(a.begin() + 1, a.end());
    vector<string> s(n + 1);
    for (int i = 1; i <= n; i++)
        cin >> s[i];
    ahocorasick ac;
    for (int i = 1; i <= n; i++)
        ac.add(s[i]);
    ac.work();

    auto mul = [&](mat &a, mat &b) -> mat
    {
        int p = a.size() - 1, q = a[0].size() - 1, r = b[0].size() - 1;
        vector ret(p + 1, vector<int>(r + 1));
        for (int i = 1; i <= p; i++)
            for (int j = 1; j <= q; j++)
                for (int k = 1; k <= r; k++)
                    ret[i][k] = (ret[i][k] + a[i][j] * b[j][k]) % mod;
        return ret;
    };

    int m = ac.t.size() - 1;
    vector tr1(26, vector(m + 1, vector<int>(m + 1)));
    for (int i = 0; i < 26; i++)
        for (int j = 1; j <= m; j++)
        {
            int temp = ac.t[j].next[i];
            if (!ac.t[temp].end)
                tr1[i][j][temp]++;
        }

    vector tr2(60, vector(m + 1, vector<int>(m + 1)));
    for (int i = 0; i < 26; i++)
        for (int j = 1; j <= m; j++)
        {
            int temp = ac.t[j].next[i];
            if (!ac.t[temp].end)
                tr2[0][j][temp]++;
        }
    for (int i = 1; i < 60; i++)
        tr2[i] = mul(tr2[i - 1], tr2[i - 1]);

    vector ans(2, vector<int>(m + 1));
    ans[1][1] = 1;

    int cur = 0;
    for (int i = 1; i <= k; i++)
    {
        auto [pos, c] = a[i];
        int msk = pos - cur - 1;
        for (int j = 59; j >= 0; j--)
            if (msk & 1ll << j)
                ans = mul(ans, tr2[j]);
        ans = mul(ans, tr1[c - 'a']);
        cur = pos;
    }
    int msk = l - cur;
    for (int j = 59; j >= 0; j--)
        if (msk & 1ll << j)
            ans = mul(ans, tr2[j]);

    int out = 0;
    for (int i = 1; i <= m; i++)
        out = (out + ans[1][i]) % mod;
    cout << out << endl;
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cold_beans();
}