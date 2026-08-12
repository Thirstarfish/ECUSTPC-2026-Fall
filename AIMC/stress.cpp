// Random stress test: compare std vs brute on small grids.
// Covers shapes that exercise all three cluster types.
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int iters = argc >= 2 ? atoi(argv[1]) : 50;
    mt19937 rng(argc >= 3 ? atoi(argv[2]) : 42);

    vector<array<int, 3>> shapes = {
        {1,1,1},{1,1,2},{1,2,2},{2,2,2},{1,1,3},{1,3,3},{2,2,3},
        {3,2,2},{1,2,4},{2,2,4},{1,1,8},{1,4,4},{2,3,3},{3,3,2},
    };

    for (int it = 0; it < iters; ++it) {
        auto sh = shapes[rng() % shapes.size()];
        int N = sh[0], M = sh[1], H = sh[2];
        int V = N * M * H;
        long long maxV = 100;

        ostringstream os;
        os << N << ' ' << M << ' ' << H << '\n';
        for (int t = 0; t < 8; ++t) {
            for (int x = 0; x < N; ++x) {
                for (int y = 0; y < M; ++y) {
                    for (int z = 0; z < H; ++z) {
                        os << (long long)(rng() % (maxV + 1));
                        if (z + 1 < H) os << ' ';
                    }
                    os << '\n';
                }
                if (x + 1 < N) os << '\n';
            }
            if (t + 1 < 8) os << '\n';
        }
        string in = os.str();

        FILE* f = fopen("_stress.in", "w"); fwrite(in.data(),1,in.size(),f); fclose(f);
        string r1, r2;
        if (system(".\\std.exe < _stress.in > _std.out") != 0) { cerr<<"std failed\n"; return 2; }
        if (system(".\\brute.exe < _stress.in > _brute.out") != 0) { cerr<<"brute failed\n"; return 2; }
        { FILE* p = fopen("_std.out","r"); char buf[64]; size_t n=fread(buf,1,63,p); buf[n]=0; fclose(p); r1=buf; }
        { FILE* p = fopen("_brute.out","r"); char buf[64]; size_t n=fread(buf,1,63,p); buf[n]=0; fclose(p); r2=buf; }

        auto trim = [](string s){ while(!s.empty() && (s.back()=='\n'||s.back()==' '||s.back()=='\r')) s.pop_back(); return s; };
        r1 = trim(r1); r2 = trim(r2);
        if (r1 != r2) {
            cerr << "MISMATCH on shape " << N <<"x"<<M<<"x"<<H << " V="<<V<<"\n";
            cerr << "std=" << r1 << " brute=" << r2 << "\n";
            cerr << "INPUT:\n" << in;
            return 1;
        }
        cerr << "[" << it+1 << "/" << iters << "] " << N<<"x"<<M<<"x"<<H << " ans=" << r1 << "\n";
    }
    cerr << "ALL OK\n";
    return 0;
}
