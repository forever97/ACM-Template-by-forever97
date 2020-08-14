/*
    无向图：
        欧拉回路：每个顶点的度数都是偶数，则存在欧拉回路。
        欧拉路：当且仅当该图所有顶点的度数为偶数，
                或者除了两个度数为奇数外其余的全是偶数。
    有向图：
        欧拉回路：每个顶点的入度=出度，则存在欧拉回路。
        欧拉路：当且仅当该图所有顶点出度=入度
                或者一个顶点出度=入度+1，另一个顶点入度=出度+1，
                其他顶点出度=入度
     O(n+m)求欧拉回路
     n为点数，m为边数，
     若有解则依次输出经过的边的编号，
     若是无向图，则正数表示x到y，负数表示y到x
*/
namespace UndirectedGraph {
int n, m, i, x, y, d[N], g[N], v[M << 1], w[M << 1], vis[M << 1], nxt[M << 1],
    ed;
int ans[M], cnt;
void add(int x, int y, int z) {
    d[x]++;
    v[++ed] = y;
    w[ed] = z;
    nxt[ed] = g[x];
    g[x] = ed;
}
void dfs(int x) {
    for (int& i = g[x]; i;) {
        if (vis[i]) {
            i = nxt[i];
            continue;
        }
        vis[i] = vis[i ^ 1] = 1;
        int j = w[i];
        dfs(v[i]);
        ans[++cnt] = j;
    }
}
void solve() {
    scanf("%d%d", &n, &m);
    for (i = ed = 1; i <= m; i++)
        scanf("%d%d", &x, &y), add(x, y, i), add(y, x, -i);
    for (i = 1; i <= n; i++)
        if (d[i] & 1) {
            puts("NO");
            return;
        }
    for (i = 1; i <= n; i++)
        if (g[i]) {
            dfs(i);
            break;
        }
    for (i = 1; i <= n; i++)
        if (g[i]) {
            puts("NO");
            return;
        }
    puts("YES");
    for (i = m; i; i--) printf("%d ", ans[i]);
}
}  // namespace UndirectedGraph
namespace DirectedGraph {
int n, m, i, x, y, d[N], g[N], v[M], vis[M], nxt[M], ed;
int ans[M], cnt;
void add(int x, int y) {
    d[x]++;
    d[y]--;
    v[++ed] = y;
    nxt[ed] = g[x];
    g[x] = ed;
}
void dfs(int x) {
    for (int& i = g[x]; i;) {
        if (vis[i]) {
            i = nxt[i];
            continue;
        }
        vis[i] = 1;
        int j = i;
        dfs(v[i]);
        ans[++cnt] = j;
    }
}
void solve() {
    scanf("%d%d", &n, &m);
    for (i = 1; i <= m; i++) scanf("%d%d", &x, &y), add(x, y);
    for (i = 1; i <= n; i++)
        if (d[i]) {
            puts("NO");
            return;
        }
    for (i = 1; i <= n; i++)
        if (g[i]) {
            dfs(i);
            break;
        }
    for (i = 1; i <= n; i++)
        if (g[i]) {
            puts("NO");
            return;
        }
    puts("YES");
    for (i = m; i; i--) printf("%d ", ans[i]);
}
}  // namespace DirectedGraph