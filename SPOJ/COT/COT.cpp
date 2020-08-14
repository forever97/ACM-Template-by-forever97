/*
    题目大意：静态链上第k大
    题解：随意确立一个根，对每个节点建立一棵权值线段树保存从根节点到当前节点的权值集合，
    查询链x-y时用T[l[x]]+T[l[y]]-T[l[lca]]-T[l[flca]]做判断在树上二分即可，
    每次建树从父节点加链创建，复杂度logn，
    由于权值范围是int，所以需要权值映射。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 100010;
int l[N * 40], r[N * 40], T[N * 40], tot, root[N];
int change(int x, int a, int b, int c, int p) {
    int y = ++tot;
    T[y] = T[x] + p;
    if (a == b) return y;
    int mid = (a + b) >> 1;
    if (c <= mid)
        l[y] = change(l[x], a, mid, c, p), r[y] = r[x];
    else
        l[y] = l[x], r[y] = change(r[x], mid + 1, b, c, p);
    return y;
}
int query(int x, int y, int a, int b, int lca, int flca, int k) {
    if (a == b) return a;
    int mid = (a + b) >> 1;
    int cnt = T[l[x]] + T[l[y]] - T[l[lca]] - T[l[flca]];
    if (k <= cnt)
        return query(l[x], l[y], a, mid, l[lca], l[flca], k);
    else
        return query(r[x], r[y], mid + 1, b, r[lca], r[flca], k - cnt);
}
void read(int& x) {
    int f = 1;
    x = 0;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if ('-' == ch) f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = (x << 3) + (x << 1) + ch - '0';
        ch = getchar();
    }
}
int siz;         // 离散化之后的大小
int w[N], b[N];  // 原值和映射
int ed, g[N << 1], v[N << 1], nxt[N << 1];
int f[N], size[N], son[N], dfn, top[N], d[N];
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
    top[x] = y;
    root[x] =
        change(root[f[x]], 1, siz, w[x], 1);  // dfs的同时对每个节点建立线段树
    if (son[x]) dfs2(son[x], y);
    for (int i = g[x]; i; i = nxt[i])
        if (v[i] != son[x] && v[i] != f[x]) dfs2(v[i], v[i]);
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
    root[0] = 0;
    tot = 0;
    memset(g, dfn = ed = 0, sizeof(g));
    memset(v, 0, sizeof(v));
    memset(nxt, 0, sizeof(nxt));
    memset(son, -1, sizeof(son));
}
int n, m;
int main() {
    Initialize();
    read(n);
    read(m);
    for (int i = 1; i <= n; i++) read(w[i]), b[i] = w[i];
    for (int i = 1; i < n; i++) {
        int x, y;
        read(x);
        read(y);
        add_edge(x, y);
        add_edge(y, x);
    }
    sort(b + 1, b + n + 1);
    siz = unique(b + 1, b + n + 1) - (b + 1);
    for (int i = 1; i <= n; i++)
        w[i] = lower_bound(b + 1, b + siz + 1, w[i]) - b;
    dfs(1);
    dfs2(1, 1);
    while (m--) {
        int x, y, k;
        read(x), read(y), read(k);
        int LCA = lca(x, y);
        int kth = query(root[x], root[y], 1, siz, root[LCA], root[f[LCA]], k);
        printf("%d\n", b[kth]);
    }
    return 0;
}
