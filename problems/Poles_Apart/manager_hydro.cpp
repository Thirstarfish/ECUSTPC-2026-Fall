#include <ext/stdio_filebuf.h>
#include <fcntl.h>
#include <algorithm>
#include <array>
#include <csignal>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <unistd.h>

using namespace std;
using Ring = array<int, 4>;
using Triple = array<int, 3>;

static int open_pipe(const string& s, bool input) {
    if (!s.empty() && s[0] >= '0' && s[0] <= '9') return stoi(s);
    int fd = open(s.c_str(), input ? O_RDONLY : O_WRONLY);
    return fd;
}

int main(int argc, char** argv) {
#ifdef SIGPIPE
    signal(SIGPIPE, SIG_IGN);
#endif
    if (argc < 5) { cerr << "expected four pipe descriptors\n"; cout << "0\n"; return 0; }
    int fd[4];
    for (int i = 0; i < 4; ++i) {
        fd[i] = open_pipe(argv[i + 1], i % 2 == 0);
        if (fd[i] < 0) { cerr << "cannot open pipe\n"; cout << "0\n"; return 0; }
    }
    __gnu_cxx::stdio_filebuf<char> ainb(fd[0], ios::in), aoutb(fd[1], ios::out);
    __gnu_cxx::stdio_filebuf<char> binb(fd[2], ios::in), boutb(fd[3], ios::out);
    istream first_in(&ainb), second_in(&binb);
    ostream first_out(&aoutb), second_out(&boutb);

    int run, n, seed, V, E;
    if (!(cin >> run >> n >> seed >> V >> E) || run != 0 || n != 200000 || V != 2000 || E != 43600) {
        cerr << "invalid test input header\n"; cout << "0\n"; return 0;
    }
    vector<pair<int,int>> edges(E);
    vector<vector<unsigned char>> graph(V + 1, vector<unsigned char>(V + 1));
    for (auto& [u, v] : edges) {
        if (!(cin >> u >> v) || u < 1 || u > V || v < 1 || v > V || u == v || graph[u][v]) {
            cerr << "invalid edge\n"; cout << "0\n"; return 0;
        }
        graph[u][v] = graph[v][u] = 1;
    }

    first_out << "send\n" << n << '\n' << V << ' ' << E << '\n';
    for (auto [u, v] : edges) first_out << u << ' ' << v << '\n';
    first_out.flush();

    vector<Ring> rings(n);
    bool valid = true;
    for (Ring& r : rings) {
        for (int& x : r) if (!(first_in >> x)) valid = false;
        if (!valid) break;
        bool distinct = true;
        for (int i = 0; i < 4; ++i) {
            if (r[i] < 1 || r[i] > V) distinct = false;
            for (int j = 0; j < i; ++j) if (r[i] == r[j]) distinct = false;
        }
        int degree[4] = {};
        for (int i = 0; i < 4; ++i) for (int j = i + 1; j < 4; ++j)
            if (distinct && graph[r[i]][r[j]]) ++degree[i], ++degree[j];
        for (int i = 0; i < 4; ++i) if (!distinct || degree[i] != 2) valid = false;
    }
    if (!valid) { cerr << "first process produced an invalid induced 4-cycle\n"; cout << "0\n"; return 0; }

    vector<pair<Triple, int>> queries;
    queries.reserve(4 * n);
    for (int i = 0; i < n; ++i) for (int removed = 0; removed < 4; ++removed) {
        Triple t{}; int p = 0;
        for (int j = 0; j < 4; ++j) if (j != removed) t[p++] = rings[i][j];
        queries.push_back({t, i + 1});
    }
    mt19937 rng(static_cast<unsigned>(seed));
    shuffle(queries.begin(), queries.end(), rng);

    // Read answers concurrently: the receiver may output while still reading
    // queries. Writing all queries first can fill both pipes and deadlock.
    // Sending one query and waiting for its answer also breaks batch solutions.
    thread writer([&] {
        second_out << "receive\n" << V << ' ' << E << '\n';
        for (auto [u, v] : edges) second_out << u << ' ' << v << '\n';
        second_out << queries.size() << '\n';
        for (const auto& q : queries) {
            second_out << q.first[0] << ' ' << q.first[1] << ' ' << q.first[2] << '\n';
            if (!second_out) break;
        }
        second_out.flush();
    });

    int correct = 0, answer;
    for (auto& q : queries) {
        if (!(second_in >> answer)) break;
        if (answer == q.second) ++correct;
    }
    writer.join();
    double score = static_cast<double>(correct) / queries.size();
    // Preserve the original checker's all-or-nothing scoring.
    cout << (correct == static_cast<int>(queries.size()) && second_out ? 1 : 0) << '\n';
    cerr << "Correct: " << correct << '/' << queries.size() << " (" << score * 100.0 << "%)\n";
    return 0;
}
