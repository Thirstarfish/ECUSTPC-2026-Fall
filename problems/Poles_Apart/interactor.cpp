#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

#define vec vector
#define pb push_back

int main(int argc, char *argv[])
{
    registerInteraction(argc, argv);
    int run = inf.readInt();
    if (run == 0)
    {
        int n = inf.readInt();
        int seed = inf.readInt();
        int V = inf.readInt();
        int E = inf.readInt();
        cout << "send" << '\n';
        cout << n << '\n';
        cout << V << ' ' << E << '\n';
        vec<array<int, 2>> e(E + 1);
        vec g(V + 1, vec<char>(V + 1));
        for (int i = 1; i <= E; i++)
        {
            auto &u = e[i][0], &v = e[i][1];
            u = inf.readInt();
            v = inf.readInt();
            cout << u << ' ' << v << '\n';
            g[u][v] = g[v][u] = 1;
        }

        cout.flush(); // 只在最后统一 flush

        vec c4(n, vec<int>(4));
        for (int k = 0; k < n; k++)
        {
            auto &ring = c4[k];
            for (int i = 0; i < 4; i++)
                ring[i] = ouf.readInt(1, V);

            vec<int> deg(4);
            for (int i = 0; i < 4; i++)
                for (int j = i + 1; j < 4; j++)
                    if (g[ring[i]][ring[j]])
                        deg[i]++, deg[j]++;
            for (int i = 0; i < 4; i++)
                if (deg[i] != 2)
                    quitf(_wa, "the %dth output is not a induced 4-cycle", k);
        }
        if (!ouf.seekEof())
            quitf(_pe, "extra tokens in the output");

        tout << 1 << '\n'; // run = 1 传给下一次
        tout << n << '\n';
        tout << seed << '\n';
        tout << V << ' ' << E << '\n';
        for (int i = 1; i <= E; i++)
            tout << e[i][0] << ' ' << e[i][1] << '\n';
        for (auto &ring : c4)
            tout << ring[0] << ' ' << ring[1] << ' ' << ring[2] << ' ' << ring[3] << '\n';

        quitf(_ok, "%d c4s found", n);
    }
    else
    {
        int n = inf.readInt();
        int seed = inf.readInt();
        int V = inf.readInt();
        int E = inf.readInt();
        vec<array<int, 2>> e(E + 1);
        for (int i = 1; i <= E; i++)
        {
            auto &u = e[i][0], &v = e[i][1];
            u = inf.readInt();
            v = inf.readInt();
        }
        vector<vector<int>> c4s;
        for (int i = 0; i < n; i++)
        {
            vector<int> c4(4);
            for (int j = 0; j < 4; j++)
                c4[j] = inf.readInt();
            c4s.push_back(c4);
        }

        int q = 4 * n;
        rnd.setSeed(seed);
        vector<pair<vector<int>, int>> queries;

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                vector<int> rem;
                for (int k = 0; k < 4; k++)
                    if (j != k)
                        rem.push_back(c4s[i][k]);
                queries.push_back({rem, i + 1});
            }
        }

        shuffle(queries.begin(), queries.end());

        cout << "receive" << '\n';
        cout << V << " " << E << '\n';
        for (int i = 1; i <= E; i++)
            cout << e[i][0] << " " << e[i][1] << '\n';
        cout << q << '\n';
        for (auto &[triple, idx] : queries)
            cout << triple[0] << " " << triple[1] << " " << triple[2] << '\n';

        cout.flush();

        int correct = 0;
        for (int i = 0; i < q; i++)
        {
            int ans = ouf.readInt(1, n, format("answer[%d]", i + 1));
            if (ans == queries[i].second)
                correct++;
        }

        if (!ouf.seekEof())
            quitf(_pe, "extra tokens in the output");

        tout << correct << " " << q << '\n';
        quitf(_ok, "%d/%d correct.", correct, q);
    }
    return 0;
}
