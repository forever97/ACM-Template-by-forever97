/*
    题目大意：维护一棵树，允许修改边权以及查询链上最大值
    题解：我们将边权转为点权，标记在深度较深的点上，树链剖分后用线段树处理即可
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 20010;
int top[N], nxt[N], v[N], g[N], d[N], son[N], f[N], size[N], st[N], en[N], dfn,
    ed;
void add(int x, int y) {
    v[++ed] = y;
    nxt[ed] = g[x];
    g[x] = ed;
}
void init() {
    memset(g, dfn = ed = 0, sizeof(g));
    memset(v, 0, sizeof(v));
    memset(nxt, 0, sizeof(nxt));
    memset(son, -1, sizeof(son));
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
    st[x] = ++dfn;
    top[x] = y;
    if (son[x]) dfs2(son[x], y);
    for (int i = g[x]; i; i = nxt[i])
        if (v[i] != son[x] && v[i] != f[x]) dfs2(v[i], v[i]);
    en[x] = dfn;
}
struct Node {
    int l, r;
    int Max;
} Tree[N * 3];
void build(int x, int l, int r) {
    Tree[x].l = l;
    Tree[x].r = r;
    Tree[x].Max = 0;
    if (l == r) return;
    int mid = (l + r) >> 1;
    build(x << 1, l, mid);
    build((x << 1) | 1, mid + 1, r);
}
void up(int x) { Tree[x].Max = max(Tree[x << 1].Max, Tree[(x << 1) | 1].Max); }
void update(int x, int k, int val) {
    if (Tree[x].l == k && Tree[x].r == k) {
        Tree[x].Max = val;
        return;
    }
    int mid = (Tree[x].l + Tree[x].r) >> 1;
    if (k <= mid)
        update(x << 1, k, val);
    else
        update((x << 1) | 1, k, val);
    up(x);
}
int query(int x, int l, int r) {
    if (Tree[x].l == l && Tree[x].r == r) return Tree[x].Max;
    int mid = (Tree[x].l + Tree[x].r) >> 1;
    if (r <= mid)
        return query(x << 1, l, r);
    else if (l > mid)
        return query((x << 1) | 1, l, r);
    else
        return max(query(x << 1, l, mid), query((x << 1) | 1, mid + 1, r));
}
int find(int x, int y) {
    int tmp = 0;
    for (; top[x] != top[y]; x = f[top[x]]) {
        if (d[top[x]] < d[top[y]]) {
            int z = x;
            x = y;
            y = z;
        }
        tmp = max(tmp, query(1, st[top[x]], st[x]));
    }
    if (x == y) return tmp;
    if (d[x] > d[y]) {
        int z = x;
        x = y;
        y = z;
    }
    return max(tmp, query(1, st[son[x]], st[y]));
}
int e[N][3];
int main(int n, int T) {
    scanf("%d", &T);
    while (T--) {
        init();
        scanf("%d", &n);
        for (int i = 0; i < n - 1; i++) {
            scanf("%d%d%d", &e[i][0], &e[i][1], &e[i][2]);
            add(e[i][0], e[i][1]);
            add(e[i][1], e[i][0]);
        }
        dfs(1);
        dfs2(1, 1);
        build(1, 1, dfn);
        for (int i = 0; i < n - 1; i++) {
            if (d[e[i][0]] > d[e[i][1]]) swap(e[i][0], e[i][1]);
            update(1, st[e[i][1]], e[i][2]);
        }
        char op[10];
        int u, w;
        while (scanf("%s", op) == 1) {
            if (op[0] == 'D') break;
            scanf("%d%d", &u, &w);
            if (op[0] == 'Q')
                printf("%d\n", find(u, w));
            else
                update(1, st[e[u - 1][1]], w);
        }
    }
    return 0;
}
