/*
    SPFA
    SLF优化
*/
const int N = 66000;
int v[M << 1], w[M << 1], g[N], d[N], nxt[M << 1], ed;
int q[N];
unsigned short h, t;
void add_edge(int x, int y, int z) {
    v[++ed] = y;
    w[ed] = z;
    nxt[ed] = g[x];
    g[x] = ed;
}
void add(int x, int y) {
    if (y >= d[x]) return;
    d[x] = y;
    if (!in[x]) {
        in[x] = 1;
        if (y < d[q[h]])
            q[--h] = x;
        else
            q[++t] = x;  // SLF优化
    }
}
void spfa(int S) {  // S为源点
    int i, x;
    for (i = h = 1; i <= n; i++) d[i] = inf, in[i] = 0;
    add(S, t = 0);
    while (h != t + 1)
        for (i = g[x = q[h++]], in [x] = 0; i; i = nxt[i])
            add(v[i], d[x] + w[i]);
}