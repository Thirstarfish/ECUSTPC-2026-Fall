#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

// 方向定义: 0: 上, 1: 右, 2: 下, 3: 左
const int dr[4] = {-1, 0, 1, 0};
const int dc[4] = {0, 1, 0, -1};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<string> grid(n);
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
    }

    char d1, d2;
    int p1, p2;
    cin >> d1 >> p1 >> d2 >> p2;

    // 解析初始光线的状态: 所在的第一个格子和初始运动方向
    int sr = 0, sc = 0, sd = 0;
    if (d1 == 'U') { sr = 0; sc = p1 - 1; sd = 2; }
    else if (d1 == 'D') { sr = n - 1; sc = p1 - 1; sd = 0; }
    else if (d1 == 'L') { sr = p1 - 1; sc = 0; sd = 1; }
    else if (d1 == 'R') { sr = p1 - 1; sc = m - 1; sd = 3; }

    int total_states = 4 * n * m;
    // dist 记录到达某状态的步数，-1 表示未访问
    vector<int> dist(total_states, -1);
    // prv 记录上一个状态，用于回溯
    vector<int> prv(total_states, -1);
    // prv_ch 记录从上一个状态转移过来时，当前格子所填入的字符
    vector<char> prv_ch(total_states, 0);

    queue<int> q;
    int start_state = (sr * m + sc) * 4 + sd;
    dist[start_state] = 0;
    q.push(start_state);

    bool found = false;
    int end_r = -1, end_c = -1, end_d = -1;
    char end_ch = 0;

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        int r = (curr / 4) / m;
        int c = (curr / 4) % m;
        int d = curr % 4;

        // 当前格子允许填充的镜子类型
        string choices;
        if (grid[r][c] == '.') choices = "./\\"; // 空格子有 3 种选择
        else if (grid[r][c] == '/') choices = "/";
        else if (grid[r][c] == '\\') choices = "\\";

        for (char ch : choices) {
            int nd = d;
            if (ch == '/') nd = d ^ 1;
            else if (ch == '\\') nd = d ^ 3;

            int nr = r + dr[nd];
            int nc = c + dc[nd];

            // 检查光线是否出界（射出网格）
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) {
                bool match = false;
                if (nr == -1 && nd == 0 && d2 == 'U' && p2 == nc + 1) match = true;
                if (nr == n && nd == 2 && d2 == 'D' && p2 == nc + 1) match = true;
                if (nc == -1 && nd == 3 && d2 == 'L' && p2 == nr + 1) match = true;
                if (nc == m && nd == 1 && d2 == 'R' && p2 == nr + 1) match = true;

                if (match) {
                    found = true;
                    end_r = r;
                    end_c = c;
                    end_d = d;
                    end_ch = ch;
                    break;
                }
            } else { // 光线仍然在网格内
                int next_state = (nr * m + nc) * 4 + nd;
                if (dist[next_state] == -1) {
                    dist[next_state] = dist[curr] + 1;
                    prv[next_state] = curr;
                    prv_ch[next_state] = ch; // 记录状态变迁时使用的字符
                    q.push(next_state);
                }
            }
        }
        if (found) break;
    }

    if (found) {
        vector<string> ans = grid;
        int curr_r = end_r, curr_c = end_c, curr_d = end_d;
        char curr_ch = end_ch;

        // 不断往前回溯，涂改经过的空格子
        while (true) {
            ans[curr_r][curr_c] = curr_ch;
            int state = (curr_r * m + curr_c) * 4 + curr_d;
            int p_state = prv[state];
            if (p_state == -1) break;

            curr_r = (p_state / 4) / m;
            curr_c = (p_state / 4) % m;
            curr_d = p_state % 4;
            curr_ch = prv_ch[state];
        }

        // 输出填充好的网格
        for (int i = 0; i < n; ++i) {
            cout << ans[i] << "\n";
        }
    } else {
        cout << "IMPOSSIBLE\n";
    }
}

int main() {
    // 优化标准 I/O 的读写速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int t;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}