#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

// 存储炮的信息
struct Point {
    int x, y, id;
};

void solve() {
    int n;
    cin >> n;
    vector<Point> pts(n);
    for (int i = 0; i < n; i++) {
        cin >> pts[i].x >> pts[i].y;
        pts[i].id = i;
    }

    // 邻接表和入度数组
    vector<vector<int>> adj(n);
    vector<int> in_degree(n, 0);

    // 1. 处理列（X坐标相同）
    // 按照 X 优先，Y 其次进行排序
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    });

    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && pts[j].x == pts[i].x) {
            j++;
        }
        int len = j - i;
        if (len >= 4) {
            // 单列 >= 4，死锁，绝对无解
            cout << "NO\n";
            return;
        }
        if (len == 3) {
            // 单列 = 3，中间的炮架必须先于两端被摧毁
            int mid = pts[i + 1].id;
            int end1 = pts[i].id;
            int end2 = pts[i + 2].id;
            adj[mid].push_back(end1);
            adj[mid].push_back(end2);
            in_degree[end1]++;
            in_degree[end2]++;
        }
        i = j;
    }

    // 2. 处理行（Y坐标相同）
    // 按照 Y 优先，X 其次进行排序
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        if (a.y != b.y) return a.y < b.y;
        return a.x < b.x;
    });

    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && pts[j].y == pts[i].y) {
            j++;
        }
        int len = j - i;
        if (len >= 4) {
            // 单行 >= 4，死锁，绝对无解
            cout << "NO\n";
            return;
        }
        if (len == 3) {
            // 单行 = 3，中间的炮架必须先于两端被摧毁
            int mid = pts[i + 1].id;
            int end1 = pts[i].id;
            int end2 = pts[i + 2].id;
            adj[mid].push_back(end1);
            adj[mid].push_back(end2);
            in_degree[end1]++;
            in_degree[end2]++;
        }
        i = j;
    }

    // 3. 拓扑排序 (Topological Sort)
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    int processed = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        processed++;
        
        // 撤走炮架后，解除对其两端的约束
        for (int v : adj[u]) {
            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    // 如果所有炮都能被顺利移除，则输出 YES
    if (processed == n) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

int main() {
    // 优化标准 I/O 速度
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    if (cin >> t) {
        while (t--) {
            solve();
        }
    }
    return 0;
}