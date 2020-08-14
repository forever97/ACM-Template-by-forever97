/*
    Dinic算法
*/
namespace Dinic {
const int INF = 0x3f3f3f3f;
const int MAX_V = 500;
struct edge {
    int to, cap, rev;
};
vector<edge> G[MAX_V];
int V, level[MAX_V], iter[MAX_V];
void add_edge(int from, int to, int cap) {
    G[from].push_back((edge){to, cap, G[to].size()});
    G[to].push_back((edge){from, 0, G[from].size() - 1});
}
void bfs(int s) {
    memset(level, -1, sizeof(level));
    queue<int> que;
    level[s] = 0;
    que.push(s);
    while (!que.empty()) {
        int v = que.front();
        que.pop();
        for (int i = 0; i < G[v].size(); i++) {
            edge &e = G[v][i];
            if (e.cap > 0 && level[e.to] < 0) {
                level[e.to] = level[v] + 1;
                que.push(e.to);
            }
        }
    }
}
int dfs(int v, int t, int f) {
    if (v == t) return f;
    for (int &i = iter[v]; i < G[v].size(); i++) {
        edge &e = G[v][i];
        if (e.cap > 0 && level[v] < level[e.to]) {
            int d = dfs(e.to, t, min(f, e.cap));
            if (d > 0) {
                e.cap -= d;
                G[e.to][e.rev].cap += d;
                return d;
            }
        }
    }
    return 0;
}
int max_flow(int s, int t) {
    int flow = 0;
    for (;;) {
        bfs(s);
        if (level[t] < 0) return flow;
        memset(iter, 0, sizeof(iter));
        int f;
        while ((f = dfs(s, t, INF)) > 0) {
            flow += f;
        }
    }
}
void Initialize(int n) {
    V = n;
    for (int i = 0; i <= V; i++) G[i].clear();
}
}  // namespace Dinic
/*
    Example
    求最小割的最小边数
*/
int T, n, m, a, b, c, s, t;
int main() {
    using namespace Dinic;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        scanf("%d%d", &s, &t);
        Initialize(n);
        for (int i = 1; i <= m; i++) {
            scanf("%d%d%d", &a, &b, &c);
            add_edge(a, b, c * (m + 1) + 1);
        }
        printf("%d\n", max_flow(s, t) % (m + 1));
    }
    return 0;
}
/*
    Example
    方格取数
    相邻的数字不能选，要求点权和最大
    二分图最大独立集=总和-最小割集
*/
int n, a[30][30];
void solve() {
    using namespace Dinic;
    int sum = 0, s = n * n, t = n * n + 1;
    for (int i = 0; i <= t; i++) G[i].clear();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if ((i + j) % 2 == 0) {
                if (i + 1 < n) add_edge(i * n + j, (i + 1) * n + j, INF);
                if (j + 1 < n) add_edge(i * n + j, i * n + j + 1, INF);
                if (i > 0) add_edge(i * n + j, (i - 1) * n + j, INF);
                if (j > 0) add_edge(i * n + j, i * n + j - 1, INF);
                add_edge(s, i * n + j, a[i][j]);
            } else
                add_edge(i * n + j, t, a[i][j]);
            sum += a[i][j];
        }
    printf("%d\n", sum - max_flow(s, t));
}
int main() {
    while (~scanf("%d", &n)) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) scanf("%d", &a[i][j]);
        }
        solve();
    }
    return 0;
}