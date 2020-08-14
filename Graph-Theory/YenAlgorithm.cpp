/* YenAlgorithm
 * O(kn(m+nlogn))
 * 求出s到t的k短路并输出路径
 */
typedef vector<int> Path;
struct Cell {
    int len, pos;
    Path path, revPath;
    vector<vector<int> > forbiddenList;
    Cell() {}
    Cell(int _len, int _pos, Path _path) : len(_len), pos(_pos), path(_path) {
        revPath = path;
        reverse(path.begin(), path.end());
    }
    bool operator<(const Cell &t) const {
        return len > t.len || len == t.len && revPath > t.revPath;
    }
};
#define RESET()                                              \
    memset(pre, 0, sizeof(pre)), memset(d, 0x3f, sizeof(d)), \
        memset(flag, 0, sizeof(flag)), d[S] = 0, flag[S] = 1
const int INF = 0x3f3f3f3f;
int n, m, K, s, t;
int g[55][55];
int pre[55], d[55];
int flag[55];
int filter[55][55];
priority_queue<Cell> q;
Cell dijkstra(int S, int T) {
    int now = S;
    while (1) {
        flag[now] = 1;
        for (int i = 1; i <= n; i++)
            if (!filter[now][i] && g[now][i] < INF) {
                if (g[now][i] + d[now] < d[i])
                    d[i] = d[now] + g[now][i], pre[i] = now;
                else if (g[now][i] + d[now] == d[i] && now < pre[i])
                    pre[i] = now;
            }
        now = 0;
        for (int i = 1; i <= n; i++)
            if (!flag[i] && d[i] < d[now]) now = i;
        if (!now) break;
    }
    if (d[T] == INF) return Cell(0, 0, Path());
    int fork_p, cnt = 0;
    Path tmp;
    for (int p = T; p; p = pre[p], cnt++)
        tmp.push_back(p), pre[p] == S && (fork_p = cnt);
    return Cell(d[T], tmp.size() - 1 - fork_p, tmp);
}
void modify(const Cell &pre, Cell &now) {
    for (int i = 0; i < now.path.size(); i++)
        now.forbiddenList.push_back(vector<int>());
    for (int i = 0; i < now.path.size() - 1; i++)
        now.forbiddenList[i].push_back(now.path[i + 1]);
    if (pre.path.empty()) return;
    now.forbiddenList[now.pos - 1] = pre.forbiddenList[now.pos - 1];
    now.forbiddenList[now.pos - 1].push_back(now.path[now.pos]);
}
void printPath(Path &path) {
    if (!path.size())
        puts("No");
    else {
        printf("%d", path[0]);
        for (int i = 1; i < path.size(); i++) printf("-%d", path[i]);
        puts("");
    }
}
Path yenAlgorithm(int S, int T, int K) {
    RESET();
    Cell now = dijkstra(S, T);
    if (now.path.empty()) return Path();
    modify(Cell(), now);
    for (int i = 1; i < K; i++) {
        Path nowP = now.path;
        int pos = now.pos;
        for (int j = pos - 1; j < nowP.size() - 1; j++) {
            RESET();
            for (int k = 1; k <= j; k++)
                d[nowP[k]] =
                    d[pre[nowP[k]] = nowP[k - 1]] + g[nowP[k - 1]][nowP[k]],
                flag[nowP[k]] = 1;
            memset(filter, 0, sizeof(filter));
            for (int k = 0; k < now.forbiddenList[j].size(); k++)
                filter[nowP[j]][now.forbiddenList[j][k]] = 1;
            Cell newOne = dijkstra(nowP[j], T);
            if (newOne.path.empty()) continue;
            modify(now, newOne);
            q.push(newOne);
        }
        if (q.empty()) return Path();
        now = q.top();
        q.pop();
    }
    return now.revPath;
}
int main() {
    scanf("%d%d%d%d%d", &n, &m, &K, &s, &t);
    memset(g, 0x3f, sizeof(g));
    while (m--) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        g[v][u] = w;
    }
    Path path = yenAlgorithm(t, s, K);
    printPath(path);
    return 0;
}