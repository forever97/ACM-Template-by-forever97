/*
    ISAP
*/
namespace ISAP {
const int N = 30010, inf = ~0U >> 2;
struct edge {
    int t, f;
    edge *nxt, *pair;
} * g[N], *d[N], pool[4 * N], *cur = pool;
int v[N], x, cnt, i, S, T, h[N], gap[N], maxflow;
void Initialize() {
    for (cur = pool, i = 0; i <= sum; i++)
        g[i] = d[i] = NULL, h[i] = gap[i] = 0;
    maxflow = 0;
}
void add(int s, int t, int w) {
    edge* p = cur++;
    p->t = t;
    p->f = w;
    p->nxt = g[s];
    g[s] = p;
    p = cur++;
    p->t = s;
    p->f = 0;
    p->nxt = g[t];
    g[t] = p;
    g[s]->pair = g[t];
    g[t]->pair = g[s];
}
int sap(int v, int flow) {
    if (v == T) return flow;
    int rec = 0;
    for (edge* p = d[v]; p; p = p->nxt)
        if (h[v] == h[p->t] + 1 && p->f) {
            int ret = sap(p->t, min(flow - rec, p->f));
            p->f -= ret;
            p->pair->f += ret;
            d[v] = p;
            if ((rec += ret) == flow) return flow;
        }
    if (!(--gap[h[v]])) h[S] = T;
    gap[++h[v]]++;
    d[v] = g[v];
    return rec;
}
/*
    残量网络
*/
void dfs(int x) {
    v[x] = 1;
    for (edge* p = g[x]; p; p = p->nxt)
        if (p->f && !v[p->t]) dfs(p->t);
}
void doit() {
    for (gap[0] = T, i = 1; i <= T; i++) d[i] = g[i];
    while (h[S] < T) maxflow += sap(S, inf);
}
}  // namespace ISAP
/*
    给出一个二分图
    左边点和右边点存在矛盾关系的不可共存
    选择一些点，使得其点权和最大
    不能共存的连INF，其余点权分别连源汇
    选择点权和最大=总权值-建图后边权最小割=总权值-最大流
    残量网络=最小割后的点=选择的点
*/
using namespace ISAP;
int sum, ss, t1, t2, n, m, s, tmp1[N], tmp2[N], u, e, cost;
int main() {
    while (~scanf("%d%d%d", &n, &m, &s)) {
        memset(v, 0, sizeof(v));
        ss = 0;
        sum = n + m + 2;
        S = sum - 1;
        T = sum;
        for (i = 1; i <= n; i++) {
            scanf("%d", &x);
            add(S, i, x);
            ss += x;
        }
        for (i = 1; i <= m; i++) {
            scanf("%d", &x);
            add(i + n, T, x);
            ss += x;
        }
        for (i = 1; i <= s; i++) {
            scanf("%d%d", &u, &e);
            add(u, e + n, inf);
        }
        doit();
        printf("%d ", ss - maxflow);
        dfs(S);
        for (t1 = 0, i = 1; i <= n; i++)
            if (v[i]) tmp1[t1++] = i;
        for (t2 = 0, i = n + 1; i <= n + m; i++)
            if (!v[i]) tmp2[t2++] = i - n;
        printf("%d %d\n", t1, t2);
        for (i = 0; i < t1 - 1; i++) printf("%d ", tmp1[i]);
        if (t1) printf("%d\n", tmp1[t1 - 1]);
        for (i = 0; i < t2 - 1; i++) printf("%d ", tmp2[i]);
        if (t2) printf("%d\n", tmp2[t2 - 1]);
    }
    return 0;
}
