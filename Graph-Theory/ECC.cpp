/*
    边双连通分量
    cut[i]表示输入的第i条边是否是桥
    cnt表示边双连通分量的个数
    from[i]表示i所属的边双连通分量编号
*/
using namespace std;
const int N = 5010, M = 10010;
int e[M][2], cut[M], g[N], v[M << 1], nxt[M << 1], ed = 1;
int f[N], dfn[N], low[N], num, cnt, from[N], d[N];
void add(int x, int y) {
    v[++ed] = y;
    nxt[ed] = g[x];
    g[x] = ed;
}
void tarjan(int x) {
    dfn[x] = low[x] = ++num;
    for (int i = g[x]; i; i = nxt[i])
        if (!dfn[v[i]]) {
            f[v[i]] = i >> 1, tarjan(v[i]);
            if (low[x] > low[v[i]]) low[x] = low[v[i]];
        } else if (f[x] != (i >> 1) && low[x] > dfn[v[i]])
            low[x] = dfn[v[i]];
    if (f[x] && low[x] == dfn[x]) cut[f[x]] = 1;
}
void dfs(int x, int y) {
    from[x] = y;
    for (int i = g[x]; i; i = nxt[i])
        if (!from[v[i]] && !cut[i >> 1]) dfs(v[i], y);
}
/*
    Example
    给出一张图，问增加几条边，
    使得整张图构成双连通分量
*/
int n, m;
int main() {
    while (~scanf("%d%d", &n, &m)) {
        memset(g, 0, sizeof(g));
        memset(d, 0, sizeof(d));
        memset(from, 0, sizeof(from));
        memset(f, 0, sizeof(f));
        memset(cut, 0, sizeof(cut));
        memset(dfn, 0, sizeof(dfn));
        num = 0;
        ed = 1;  // 求边双连通分量时，ed一定要为1
        for (int i = 1; i <= m; i++) {
            int u, v;
            scanf("%d%d", &u, &v);
            e[i][0] = u;
            e[i][1] = v;
            add(u, v);
            add(v, u);
        }
        tarjan(1);
        cnt = 0;
        for (int i = 1; i <= n; i++)
            if (!from[i]) dfs(i, ++cnt);
        for (int i = 1; i <= m; i++) {
            if (from[e[i][0]] != from[e[i][1]]) {
                d[from[e[i][0]]]++;
                d[from[e[i][1]]]++;
            }
        }
        int res = 0;
        for (int i = 1; i <= n; i++)
            if (d[i] == 1) res++;
        printf("%d\n", (res + 1) / 2);
    }
    return 0;
}