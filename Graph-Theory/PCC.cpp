/*
    点双连通分量
*/
int dfn[N], low[N], num, cut[N], q[N], t;
void tarjan(int x) {
    dfn[x] = low[x] = ++num, q[++t] = x;
    for (int i = g[x]; i; i = nxt[i])
        if (!dfn[v[i]]) {
            int y = v[i];
            tarjan(y);
            if (low[x] > low[y]) low[x] = low[y];
            if (dfn[x] <= low[y]) {  // x是割点，接下来一行输出所有该点双连通分量内的点
                cut[x] = 1;
                while (q[t] != y) printf("%d ", q[t--]);
                printf("%d ", q[t--]);
                printf("%d\n", x);
            }
        } else if (low[x] > dfn[v[i]])
            low[x] = dfn[v[i]];
}