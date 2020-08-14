// 有向图最小环
void solve() {
#define rep(i, n) for (int i = 1; i <= n; i++)
    rep(i, n) rep(j, n) d[i][j] = INF;
    while (m--) {
        scanf("%d%d%d", &x, &y, &z);
        if (z < d[x][y]) d[x][y] = z;
    }
    rep(k, n) rep(i, n) rep(j, n) d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    rep(i, n) ans = min(ans, d[i][i]);
}
// 无向图最小环
void solve() {
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) g[i][j] = d[i][j] = inf;
    while (m--) {
        scanf("%d%d%d", &x, &y, &z);
        if (z < g[x][y]) g[x][y] = g[y][x] = d[x][y] = d[y][x] = z;
    }
    for (ans = inf, k = 1; k <= n; k++) {
        for (i = 1; i < k; i++)
            for (j = i + 1; j < k; j++)
                ans = min(ans, d[i][j] + g[i][k] + g[k][j]);
        for (i = 1; i <= n; i++)
            for (j = 1; j <= n; j++) d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    }
}