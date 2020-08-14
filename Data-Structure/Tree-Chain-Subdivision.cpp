/*
    树链剖分
*/
int seq[N];  // seq记录的是dfs序，方便用线段树维护data[seq[i]]
namespace Tree_Chain_Subdivision {
int ed, root, d[N], num[N], v[N << 1], vis[N], f[N], g[N << 1];
int nxt[N << 1], size[N], son[N], st[N], en[N], dfn, top[N];
void add_edge(int x, int y) {
    v[++ed] = y;
    nxt[ed] = g[x];
    g[x] = ed;
}
void dfs(int x) {
    size[x] = 1;
    for (int i = g[x]; i; i = nxt[i])
        if (v[i] != f[x]) {
            f[v[i]] = x, d[v[i]] = d[x] + 1;
            dfs(v[i]), size[x] += size[v[i]];
            if (size[v[i]] > size[son[x]]) son[x] = v[i];
        }
}
void dfs2(int x, int y) {
    if (x == -1) return;
    st[x] = ++dfn;
    seq[dfn] = x;
    top[x] = y;
    if (son[x]) dfs2(son[x], y);
    for (int i = g[x]; i; i = nxt[i])
        if (v[i] != son[x] && v[i] != f[x]) dfs2(v[i], v[i]);
    en[x] = dfn;
}
//查询x,y两点的lca
int lca(int x, int y) {
    for (; top[x] != top[y]; x = f[top[x]])
        if (d[top[x]] < d[top[y]]) {
            int z = x;
            x = y;
            y = z;
        }
    return d[x] < d[y] ? x : y;
}
// x是y的祖先，查询x到y方向的第一个点
int lca2(int x, int y) {
    int t;
    while (top[x] != top[y]) t = top[y], y = f[top[y]];
    return x == y ? t : son[x];
}
//以root为根对x的子树操作
int subtree(int x, int n) {
    if (x == root) {
        return Segment_Tree::query(1, 1, n);
    }
    if (st[x] > st[root] || en[x] < en[root]) {
        return Segment_Tree::query(1, st[x], en[x]);
    }
    int y = lca2(x, root);
    return min(Segment_Tree::query(1, 1, st[y] - 1),
               Segment_Tree::query(1, en[y] + 1, n));
}
//对x到y路径上的点进行操作
void chain(int x, int y, int p) {
    for (; top[x] != top[y]; x = f[top[x]]) {
        if (d[top[x]] < d[top[y]]) {
            int z = x;
            x = y;
            y = z;
        }
        Segment_Tree::change(1, st[top[x]], st[x], p);
    }
    if (d[x] < d[y]) {
        int z = x;
        x = y;
        y = z;
    }
    Segment_Tree::change(1, st[y], st[x], p);
    // 如果是边权转点权，则为change(st[y]+1,st[x])
}
void Initialize() {
    memset(g, dfn = ed = 0, sizeof(g));
    memset(v, 0, sizeof(v));
    memset(nxt, 0, sizeof(nxt));
    memset(son, -1, sizeof(son));
}
}  // namespace Tree_Chain_Subdivision