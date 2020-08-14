/* 虚树
 * 每次询问只与树上关键点有关
 * dp部分计算出最小边割集使得1和所有关键点不连通
 */
namespace Virtual_Tree {
const LL INF = 1LL << 60;
LL mn[N];
int ed, d[N], vis[N], f[N], size[N], son[N], w[N << 1];
int nxt[N << 1], g[N], v[N << 1], st[N], en[N], dfn, top[N];
void add_edge(int x, int y, int z) {
    v[++ed] = y;
    w[ed] = z;
    nxt[ed] = g[x];
    g[x] = ed;
}
void dfs(int x) {
    size[x] = 1;
    for (int i = g[x]; i; i = nxt[i])
        if (v[i] != f[x]) {
            mn[v[i]] = min(mn[x], (LL)w[i]);
            f[v[i]] = x, d[v[i]] = d[x] + 1;
            dfs(v[i]), size[x] += size[v[i]];
            if (size[v[i]] > size[son[x]]) son[x] = v[i];
        }
}
void dfs2(int x, int y) {
    if (x == -1) return;
    st[x] = ++dfn;
    top[x] = y;
    if (son[x]) dfs2(son[x], y);
    for (int i = g[x]; i; i = nxt[i])
        if (v[i] != son[x] && v[i] != f[x]) dfs2(v[i], v[i]);
    en[x] = dfn;
}
int lca(int x, int y) {
    for (; top[x] != top[y]; x = f[top[x]])
        if (d[top[x]] < d[top[y]]) {
            int z = x;
            x = y;
            y = z;
        }
    return d[x] < d[y] ? x : y;
}
void Initialize() {
    mn[1] = INF;
    memset(g, dfn = ed = 0, sizeof(g));
    memset(v, 0, sizeof(v));
    memset(nxt, 0, sizeof(nxt));
    memset(son, -1, sizeof(son));
}
int V[N << 1], NXT[N << 1], G[N], ED, a[N], q[N], mark[N];
void ADD_edge(int x, int y) {
    V[++ED] = y;
    NXT[ED] = G[x];
    G[x] = ED;
}
bool cmp(int a, int b) { return st[a] < st[b]; }
LL dp[N];
void DP(int x) {
    dp[x] = mn[x];
    LL tmp = 0;
    for (int i = G[x]; i; i = NXT[i]) {
        DP(V[i]);
        if (mark[V[i]])
            tmp += mn[V[i]];
        else
            tmp += dp[V[i]];
    }
    G[x] = 0;
    if (tmp == 0)
        dp[x] = mn[x];
    else if (tmp <= dp[x])
        dp[x] = tmp;
}
void build(int m) {
    int tot, t, i, x;
    ED = 0;
    vis[1] = a[1] = 1;
    for (tot = ++m, i = 2; i <= m; i++)
        a[i] = read(), mark[a[i]] = vis[a[i]] = 1;
    // 读入一组关键点a，mark用于标记关键点
    sort(a + 1, a + m + 1, cmp);
    for (i = 1; i < m; i++)
        if (!vis[x = lca(a[i], a[i + 1])]) vis[a[++tot] = x] = 1;
    m = tot, sort(a + 1, a + m + 1, cmp);
    for (q[t = 1] = 1, i = 2; i <= m; q[++t] = a[i++]) {
        while (st[a[i]] < st[q[t]] || en[a[i]] > en[q[t]]) t--;
        ADD_edge(q[t], a[i]);
    }
    DP(1);
    for (i = 1; i <= m; i++) mark[a[i]] = vis[a[i]] = 0;
}
}  // namespace Virtual_Tree