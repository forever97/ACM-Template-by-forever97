/*
    前k短路
    Astar算法
    允许被重复走
*/
#include <bits/stdc++.h>
using namespace std;
typedef pair<int, int> P;
const int N = 10100, M = 10010, inf = 1000000010;
int n, m, k, S, T, i, x, y, z;
int g[N], h[N], v[M << 1], w[M << 1], nxt[M << 1], ed, d[N], vis[N], ans[N];
void add(int x, int y, int z) {
    v[++ed] = x;
    w[ed] = z;
    nxt[ed] = g[y];
    g[y] = ed;
    v[++ed] = y;
    w[ed] = z;
    nxt[ed] = h[x];
    h[x] = ed;
}
int main() {
    while (~scanf("%d%d", &n, &m)) {
        scanf("%d%d%d", &S, &T, &k);
        memset(g, ed = 0, sizeof(g));
        memset(h, 0, sizeof(h));
        memset(vis, 0, sizeof(vis));
        priority_queue<P, vector<P>, greater<P> > Q;
        for (i = 1; i <= k; i++) ans[i] = -1;
        while (m--) scanf("%d%d%d", &x, &y, &z), add(x, y, z);
        for (i = 1; i <= n; i++) d[i] = inf;
        Q.push(P(d[T] = 0, T));
        while (!Q.empty()) {
            P t = Q.top();
            Q.pop();
            if (d[t.second] < t.first) continue;
            for (i = g[x = t.second]; i; i = nxt[i])
                if (d[x] + w[i] < d[v[i]])
                    Q.push(P(d[v[i]] = d[x] + w[i], v[i]));
        }
        if (d[S] < inf) Q.push(P(d[S], S));
        while (!Q.empty()) {
            P t = Q.top();
            Q.pop();
            vis[x = t.second]++;
            if (x == T && vis[T] <= k) ans[vis[T]] = t.first;
            if (vis[T] > k) break;
            if (vis[x] <= k)
                for (i = h[x]; i; i = nxt[i])
                    Q.push(P(t.first - d[x] + d[v[i]] + w[i], v[i]));
        }
        for (int i = 1; i <= k; i++) printf("%d\n", ans[k]);
    }
    return 0;
}