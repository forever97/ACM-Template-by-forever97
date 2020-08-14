/*
    稳定k短路
    左偏树优化
    允许重复走
*/
#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> P;
const int N = 10100, M = 100010, inf = ~0U >> 1;
int n, m, i, S, T, K, x, y, z;
int g[N], v[M], u[M], w[M], nxt[M], d[N], f[N], h[N], tot;
bool is[M], vis[N];
struct Node {
    int l, r, d;
    P v;
    Node() {}
    Node(int _l, int _r, int _d, P _v) { l = _l, r = _r, d = _d, v = _v; }
} pool[2000010];
int build(P v) {
    pool[++tot] = Node(0, 0, 0, v);
    return tot;
}
int merge(int a, int b) {
    if (!a || !b) return a + b;
    if (pool[a].v > pool[b].v) swap(a, b);
    int x = ++tot;
    pool[x] = pool[a];
    pool[x].r = merge(pool[a].r, b);
    if (pool[pool[x].l].d < pool[pool[x].r].d) swap(pool[x].l, pool[x].r);
    pool[x].d = pool[x].r ? pool[pool[x].r].d + 1 : 0;
    return x;
}
void getdis() {
    int i, x;
    priority_queue<P, vector<P>, greater<P> > q;
    for (i = 1; i <= n; i++) d[i] = inf, f[i] = 0;
    q.push(P(d[T] = 0, T));
    while (!q.empty()) {
        P t = q.top();
        q.pop();
        if (t.first > d[x = t.second]) continue;
        for (i = g[x]; i; i = nxt[i])
            if (d[v[i]] > d[x] + w[i]) {
                f[v[i]] = i;
                q.push(P(d[v[i]] = d[x] + w[i], v[i]));
            }
    }
}
void dfs(int x) {
    if (!f[x] || vis[x]) return;
    vis[x] = 1;
    dfs(u[f[x]]);
    h[x] = merge(h[x], h[u[f[x]]]);
}
void add(int x, int y, int z) {
    v[++m] = x;
    u[m] = y;
    w[m] = z;
    nxt[m] = g[y];
    g[y] = m;
}
int tim;
int solve() {
    int mm = m;
    scanf("%d%d%d%d", &S, &T, &K, &tim);
    for (m = 0, i = 1; i <= n; i++) g[i] = 0;
    while (mm--) scanf("%d%d%d", &x, &y, &z), add(x, y, z);
    if (S == T) K++;
    getdis();
    if (d[S] == inf) return -1;
    if (K == 1) return d[S];
    K--;
    for (i = 1; i <= m; i++) is[i] = 0;
    for (tot = 0, i = 1; i <= n; i++) is[f[i]] = 1, h[i] = vis[i] = 0;
    for (i = 1; i <= m; i++)
        if (!is[i] && d[u[i]] < inf)
            h[v[i]] = merge(h[v[i]], build(P(w[i] - d[v[i]] + d[u[i]], u[i])));
    for (i = 1; i <= n; i++) dfs(i);
    priority_queue<P, vector<P>, greater<P> > q;
    int ans, x, y;
    y = h[S];
    if (y) q.push(P(d[S] + pool[y].v.first, y));
    while (!q.empty() && K) {
        K--;
        P t = q.top();
        q.pop();
        ans = t.first;
        x = t.second, y = pool[x].l;
        if (y) q.push(P(ans - pool[x].v.first + pool[y].v.first, y));
        y = pool[x].r;
        if (y) q.push(P(ans - pool[x].v.first + pool[y].v.first, y));
        y = h[pool[x].v.second];
        if (y) q.push(P(ans + pool[y].v.first, y));
    }
    return K ? -1 : ans;
}
int main() {
    while (~scanf("%d%d", &n, &m)) {
        int res = solve();
        if (res != -1 && res <= tim)
            puts("yareyaredawa");
        else
            puts("Whitesnake!");
    }
    return 0;
}
