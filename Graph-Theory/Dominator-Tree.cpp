/* Dominator Tree
 * dfn[x]：x的DFS序。
 * id[x]：DFS序第x个点的id。
 * gd[x]：DFS序第x个点在Dominator Tree上的孩子列表。
 * idom[x]：DFS序第x个点在Dominator Tree上的父亲。
 * sd[x]：DFS序第x个点的半必经点。
 * id[idom[dfn[x]]]：x的最近必经点
 */
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 100100, M = 200010;
typedef long long LL;
int n, m, i, x, y;
int g1[N], g2[N], gd[N], v[M * 3 + N], nxt[M * 3 + N], ed;
int cnt, dfn[N], id[N], fa[N], f[N], mn[N], sd[N], idom[N];
void add(int* g, int x, int y) {
    v[++ed] = y;
    nxt[ed] = g[x];
    g[x] = ed;
}
int F(int x) {
    if (f[x] == x) return x;
    int y = F(f[x]);
    if (sd[mn[x]] > sd[mn[f[x]]]) mn[x] = mn[f[x]];
    return f[x] = y;
}
void dfs(int x) {
    id[dfn[x] = ++cnt] = x;
    for (int i = g1[x]; i; i = nxt[i])
        if (!dfn[v[i]]) dfs(v[i]), fa[dfn[v[i]]] = dfn[x];
}
void tarjan(int S) {
    int i, j, k, x;
    for (cnt = 0, i = 1; i <= n; i++)
        gd[i] = dfn[i] = id[i] = fa[i] = idom[i] = 0, f[i] = sd[i] = mn[i] = i;
    dfs(S);
    for (i = cnt; i > 1; i--) {
        for (j = g2[id[i]]; j; j = nxt[j])
            if (dfn[v[j]])
                F(k = dfn[v[j]]), sd[i] = sd[i] < sd[mn[k]] ? sd[i] : sd[mn[k]];
        add(gd, sd[i], i);
        for (j = gd[f[i] = x = fa[i]]; j; j = nxt[j])
            F(k = v[j]), idom[k] = sd[mn[k]] < x ? mn[k] : x;
        gd[x] = 0;
    }
    for (i = 2; i <= cnt; add(gd, idom[i], i), i++)
        if (idom[i] != sd[i]) idom[i] = idom[idom[i]];
}
/*
 * 给出一张有向图，n点为消息源，求每个点i如果要接受n点的信息的必经点权id和
 * 在Dominator Tree上做dfs即可，计算id和
 */
LL ans[N];
void Get_ans(int x) {
    ans[id[x]] += id[x];
    for (int i = gd[x]; i; i = nxt[i]) {
        ans[id[v[i]]] += ans[id[x]];
        Get_ans(v[i]);
    }
}
int main() {
    while (~scanf("%d%d", &n, &m)) {
        for (ed = 0, i = 1; i <= n; i++) g1[i] = g2[i] = 0;
        while (m--) scanf("%d%d", &x, &y), add(g1, x, y), add(g2, y, x);
        tarjan(n);
        memset(ans, 0, sizeof(ans));
        Get_ans(1);
        for (int i = 1; i <= n; i++) printf("%lld%c", ans[i], " \n"[i == n]);
    }
    return 0;
}
