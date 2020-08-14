/*
    链式Hash
    ASL=1.4
*/
namespace Hash_Table {
const int M = 262143;
struct E {
    int v;
    E* nxt;
} * g[M + 1], pool[N], *cur = pool, *p;
int vis[M + 1];
void ins(int v) {
    int u = v & M;
    if (vis[u] < T) vis[u] = T, g[u] = NULL;
    for (p = g[u]; p; p = p->nxt)
        if (p->v == v) return;
    p = cur++;
    p->v = v;
    p->nxt = g[u];
    g[u] = p;
}
int ask(int v) {
    int u = v & M;
    if (vis[u] < T) return 0;
    for (p = g[u]; p; p = p->nxt)
        if (p->v == v) return 1;
    return 0;
}
void init() { T++, cur = pool; }
}  // namespace Hash_Table