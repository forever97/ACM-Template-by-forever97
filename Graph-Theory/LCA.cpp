/*
    最近公共祖先
    要保证一整棵树连通
    以1为根节点(注意dph[1]=1，否则深度判定会出错)
*/
namespace LCA {
const int N = 50010, LEV = 20;
int dph[N];
int fa[N][LEV];
vector<int> G[N];
void add(int a, int b) {
    G[a].push_back(b);
    G[b].push_back(a);
}
void dfs(int rt, int f) {
    for (int i = 1; i < LEV; i++) {
        if (dph[rt] - 1 < (1 << i)) break;
        fa[rt][i] = fa[fa[rt][i - 1]][i - 1];
    }
    for (int v : G[rt]) {
        if (v == f) continue;
        dph[v] = dph[rt] + 1;
        fa[v][0] = rt;
        dfs(v, rt);
    }
}
int lca(int a, int b) {
    if (dph[a] < dph[b]) swap(a, b);
    int t = dph[a] - dph[b];
    for (int i = 0; i < LEV; i++)
        if ((1 << i) & t) a = fa[a][i];
    for (int i = LEV - 1; i >= 0; i--) {
        if (fa[a][i] != fa[b][i]) a = fa[a][i], b = fa[b][i];
    }
    if (a != b) return fa[a][0];
    return a;
}
// 从节点x往上走l个节点的位置
int GoUp(int x, int l) {
    for (int i = 0; i < LEV; i++) {
        if ((1 << i) & l) x = fa[x][i];
    }
    return x;
}
void Initialize(int n) {
    for (int i = 1; i <= n; i++) G[i].clear();
    memset(fa, 0, sizeof(fa));
    dph[1] = 1;
}
}  // namespace LCA
/*
    查询路径最大值
    (以下模板不用保证dph[1]=1)
*/
int head[M << 1], u[M << 1], v[M << 1], nxt[M << 1];
inline void init() {
    ed = 0;
    memset(head, -1, sizeof(head));
}
inline void add(int a, int b, LL c) {
    u[++ed] = a, v[ed] = b, w[ed] = c;
    nxt[ed] = head[u[ed]];
    head[u[ed]] = ed;
}
const int LEV = 20;
LL fw[N][LEV];
int fa[N][LEV], dph[N];
void dfs(int rt, int fx) {
    vis[rt] = 1;
    for (int i = 1; i < LEV; i++) {
        fa[rt][i] = fa[fa[rt][i - 1]][i - 1];
        fw[rt][i] = max(fw[rt][i - 1], fw[fa[rt][i - 1]][i - 1]);
    }
    for (int i = head[rt]; ~i; i = nxt[i]) {
        if (v[i] == fx) continue;
        fa[v[i]][0] = rt;
        fw[v[i]][0] = w[i];
        dph[v[i]] = dph[rt] + 1;
        dfs(v[i], rt);
    }
}
LL Cal(int a, int b) {
    LL res = 0;
    if (dph[a] < dph[b]) swap(a, b);
    for (int i = LEV - 1; i >= 0; i--) {
        if (dph[fa[a][i]] >= dph[b]) {
            res = max(res, fw[a][i]);
            a = fa[a][i];
        }
    }
    for (int i = LEV - 1; i >= 0; i--) {
        if (fa[a][i] != fa[b][i]) {
            res = max(res, max(fw[a][i], fw[b][i]));
            a = fa[a][i];
            b = fa[b][i];
        }
    }
    if (a != b) res = max(res, max(fw[a][0], fw[b][0]));
    return res;
}
/*
    对于森林的处理方式
*/
void build() {
    memset(vis, 0, sizeof(vis));
    for (int i = 1; i <= n; i++) {
        if (!vis[i]) dfs(i, 0);
    }
}
