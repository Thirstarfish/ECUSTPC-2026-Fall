#include <ext/stdio_filebuf.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;

static bool valid_code(const string& s) {
    if (s.size() != 13) return false;
    for (char c : s) if (c < 'a' || c > 'z') return false;
    return true;
}

int main(int argc, char** argv) {
    if (argc < 5) { cerr << "expected four pipe descriptors\n"; cout << "0\n"; return 0; }
    int fd[4];
    // Hydro normally passes paths such as /proc/self/fd/3.  Some local
    // runners pass the descriptor numbers themselves, so accept both forms.
    for (int i = 0; i < 4; ++i) {
        string arg = argv[i + 1];
        try {
            if (!arg.empty() && arg[0] >= '0' && arg[0] <= '9') {
                size_t used = 0;
                fd[i] = stoi(arg, &used);
                if (used != arg.size()) throw invalid_argument("trailing text");
            } else {
                int flags = (i % 2 == 0) ? O_RDONLY : O_WRONLY;
                fd[i] = open(arg.c_str(), flags);
                if (fd[i] < 0) throw invalid_argument("open failed");
            }
        } catch (...) {
            cerr << "invalid pipe descriptor: " << arg << '\n';
            cout << "0\n";
            return 0;
        }
    }
    __gnu_cxx::stdio_filebuf<char> ainb(fd[0], ios::in), aoutb(fd[1], ios::out);
    __gnu_cxx::stdio_filebuf<char> binb(fd[2], ios::in), boutb(fd[3], ios::out);
    istream ain(&ainb), bin(&binb); ostream aout(&aoutb), bout(&boutb);
    long long run, x, answer; string code;
    // The supplied generator/validator use the interactor-compatible format:
    //   0\n<x>\n  (the run flag is always zero for this two-process manager).
    if (!(cin >> run >> x) || run != 0 || x < 0 || x > 1000000000000000000LL) {
        cout << "0\n"; return 0;
    }
    aout << "send\n" << x << '\n' << flush;
    if (!(ain >> code) || !valid_code(code)) { cerr << "invalid code\n"; cout << "0\n"; return 0; }
    bout << "receive\n" << code << '\n' << flush;
    if (!(bin >> answer)) { cerr << "no answer\n"; cout << "0\n"; return 0; }
    bool ok = answer == x;
    cout << (ok ? "1" : "0") << '\n';
    cerr << (ok ? "Accepted" : "Wrong Answer") << ": expected " << x << ", received " << answer << '\n';
    return 0;
}
