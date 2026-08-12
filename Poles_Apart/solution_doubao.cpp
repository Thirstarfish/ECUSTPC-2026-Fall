#include <bits/stdc++.h>
using namespace std;

const int MAXV = 2000;
using BitSet = bitset<MAXV>;

// ---------- encoding helpers ----------
uint64_t pack_quad(int a, int b, int c, int d)
{
    return ((a * 2000ULL + b) * 2000ULL + c) * 2000ULL + d;
}

void unpack_quad(uint64_t val, int &a, int &b, int &c, int &d)
{
    d = val % 2000;
    val /= 2000;
    c = val % 2000;
    val /= 2000;
    b = val % 2000;
    val /= 2000;
    a = (int)val;
}

uint64_t triplet_key(int x, int y, int z)
{
    return (x * 2001ULL + y) * 2001ULL + z;
}

// ---------- main ----------
int main()
{
    freopen("send2.in", "r", stdin);
    freopen("send2.out", "w", stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string mode;
    cin >> mode;

    int V, E;
    int n_send = 0, q_recv = 0; // only one is used depending on mode

    if (mode == "send")
    {
        cin >> n_send >> V >> E;
    }
    else
    {
        cin >> V >> E;
    }

    // ----- read the graph -----
    BitSet adj[MAXV];
    for (int i = 0; i < E; ++i)
    {
        int u, v;
        cin >> u >> v;
        --u;
        --v;
        adj[u][v] = adj[v][u] = 1;
    }

    if (mode == "receive")
    {
        cin >> q_recv;
    }

    int target = (mode == "send") ? n_send : q_recv / 4;

    // ----- Step 1 : collect all induced C4 -----
    vector<uint64_t> raw_quads;
    for (int a = 0; a < V; ++a)
    {
        for (int c = a + 1; c < V; ++c)
        {
            if (adj[a][c])
                continue; // diagonal must be non-edge
            BitSet common = adj[a] & adj[c];
            vector<int> neigh;
            for (int v = common._Find_first(); v < V; v = common._Find_next(v))
                neigh.push_back(v);
            int sz = (int)neigh.size();
            for (int i = 0; i < sz; ++i)
            {
                int b = neigh[i];
                for (int j = i + 1; j < sz; ++j)
                {
                    int d = neigh[j];
                    if (adj[b][d])
                        continue; // the other diagonal must be non-edge
                    int arr[4] = {a, b, c, d};
                    sort(arr, arr + 4);
                    raw_quads.push_back(pack_quad(arr[0], arr[1], arr[2], arr[3]));
                }
            }
        }
    }

    sort(raw_quads.begin(), raw_quads.end());
    raw_quads.erase(unique(raw_quads.begin(), raw_quads.end()), raw_quads.end());
    const vector<uint64_t> &uniq = raw_quads; // unique induced C4

    // ----- Step 2 : frequencies of all triplets -----
    vector<uint64_t> all_tri;
    all_tri.reserve(uniq.size() * 4);
    for (uint64_t q : uniq)
    {
        int a, b, c, d;
        unpack_quad(q, a, b, c, d);
        int v[4] = {a, b, c, d}; // already sorted
        for (int omit = 0; omit < 4; ++omit)
        {
            int tri[3], t = 0;
            for (int k = 0; k < 4; ++k)
                if (k != omit)
                    tri[t++] = v[k];
            all_tri.push_back(triplet_key(tri[0], tri[1], tri[2]));
        }
    }
    sort(all_tri.begin(), all_tri.end());

    // compressed frequency table
    vector<uint64_t> key_list;
    vector<int> freq_list;
    for (size_t i = 0; i < all_tri.size();)
    {
        uint64_t key = all_tri[i];
        int cnt = 0;
        while (i < all_tri.size() && all_tri[i] == key)
        {
            ++cnt;
            ++i;
        }
        key_list.push_back(key);
        freq_list.push_back(cnt);
    }

    // ----- Step 3 : weight each unique quad -----
    vector<pair<int, uint64_t>> quad_weight; // (weight, packed_quad)
    quad_weight.reserve(uniq.size());
    for (uint64_t q : uniq)
    {
        int a, b, c, d;
        unpack_quad(q, a, b, c, d);
        int v[4] = {a, b, c, d};
        int w = 0;
        for (int omit = 0; omit < 4; ++omit)
        {
            int tri[3], t = 0;
            for (int k = 0; k < 4; ++k)
                if (k != omit)
                    tri[t++] = v[k];
            uint64_t key = triplet_key(tri[0], tri[1], tri[2]);
            int pos = int(lower_bound(key_list.begin(), key_list.end(), key) - key_list.begin());
            w += freq_list[pos];
        }
        quad_weight.emplace_back(w, q);
    }

    // sort by weight ascending, then by quad (deterministic tie breaker)
    sort(quad_weight.begin(), quad_weight.end(),
         [](const pair<int, uint64_t> &x, const pair<int, uint64_t> &y)
         {
             if (x.first != y.first)
                 return x.first < y.first;
             return x.second < y.second;
         });

    // ----- Step 4 : greedy selection -----
    unordered_set<uint64_t> used;
    used.reserve(target * 4);
    vector<uint64_t> chosen;
    chosen.reserve(target);

    for (const auto &p : quad_weight)
    {
        if ((int)chosen.size() == target)
            break;
        uint64_t q = p.second;
        int a, b, c, d;
        unpack_quad(q, a, b, c, d);
        int v[4] = {a, b, c, d};
        bool conflict = false;
        uint64_t keys[4];
        for (int omit = 0; omit < 4; ++omit)
        {
            int tri[3], t = 0;
            for (int k = 0; k < 4; ++k)
                if (k != omit)
                    tri[t++] = v[k];
            keys[omit] = triplet_key(tri[0], tri[1], tri[2]);
            if (used.count(keys[omit]))
            {
                conflict = true;
                break;
            }
        }
        if (!conflict)
        {
            for (int k = 0; k < 4; ++k)
                used.insert(keys[k]);
            chosen.push_back(q);
        }
    }

    // ----- output / answer queries -----
    if (mode == "send")
    {
        for (uint64_t q : chosen)
        {
            int a, b, c, d;
            unpack_quad(q, a, b, c, d);
            cout << a + 1 << " " << b + 1 << " " << c + 1 << " " << d + 1 << "\n";
        }
    }
    else
    {
        // build triplet -> cycle index mapping
        vector<pair<uint64_t, int>> tri2idx;
        tri2idx.reserve(target * 4);
        for (int idx = 0; idx < target; ++idx)
        {
            uint64_t q = chosen[idx];
            int a, b, c, d;
            unpack_quad(q, a, b, c, d);
            int v[4] = {a, b, c, d};
            for (int omit = 0; omit < 4; ++omit)
            {
                int tri[3], t = 0;
                for (int k = 0; k < 4; ++k)
                    if (k != omit)
                        tri[t++] = v[k];
                uint64_t key = triplet_key(tri[0], tri[1], tri[2]);
                tri2idx.emplace_back(key, idx + 1);
            }
        }
        sort(tri2idx.begin(), tri2idx.end());

        for (int i = 0; i < q_recv; ++i)
        {
            int x, y, z;
            cin >> x >> y >> z;
            --x;
            --y;
            --z;
            int tri[3] = {x, y, z};
            sort(tri, tri + 3);
            uint64_t key = triplet_key(tri[0], tri[1], tri[2]);
            auto it = lower_bound(tri2idx.begin(), tri2idx.end(), make_pair(key, 0));
            cout << it->second << "\n";
        }
    }

    return 0;
}