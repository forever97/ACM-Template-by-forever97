/*
    上下界费用流
    Initialize()
    add_edge(x,y,下界,上界,cost)
    Rebuild()
    Run()
    ans=-1表示无解
*/
namespace ULB_Min_Cost_Max_Flow {
const int INF = 0x3f3f3f3f;
int V, SS, TT, S, T, cnt, ans, D[N], d[N], from[N], g[N], flow;
bool in[N];
struct edge {
    int from, to, nxt, c, v;
} e[M];
void add(int u, int v, int w, int c) {
    e[++cnt].from = u;
    e[cnt].to = v;
    e[cnt].nxt = g[u];
    g[u] = cnt;
    e[cnt].c = c;
    e[cnt].v = w;
}
void add_edge(int u, int v, int wl, int wr, int c) {
    D[u] -= wl;
    D[v] += wl;
    add(u, v, wr - wl, c);
    add(v, u, 0, -c);
}
void Rebuild() {
    SS = V + 1;
    TT = V + 2;
    for (int i = 1; i <= V; i++) {
        if (D[i] < 0) add(i, TT, -D[i], 0), add(TT, i, 0, 0);
        if (D[i] > 0) add(SS, i, D[i], 0), add(i, SS, 0, 0);
    }
}
bool spfa() {
    memset(d, INF, sizeof(d));
    d[SS] = 0;
    memset(from, 0, sizeof(from));
    queue<int> q;
    q.push(SS);
    while (!q.empty()) {
        int now = q.front();
        q.pop();
        for (int i = g[now]; i; i = e[i].nxt) {
            if (e[i].v && d[e[i].to] > d[now] + e[i].c) {
                d[e[i].to] = d[now] + e[i].c;
                from[e[i].to] = i;
                q.push(e[i].to);
            }
        }
    }
    return (d[TT] != INF);
}
void mcf() {
    int x = INF;
    for (int i = from[TT]; i; i = from[e[i].from]) x = min(x, e[i].v);
    flow += x;
    for (int i = from[TT]; i; i = from[e[i].from]) {
        e[i].v -= x;
        e[i ^ 1].v += x;
        ans += e[i].c * x;
    }
}
void Initialize() {
    memset(g, 0, sizeof(g));
    memset(e, 0, sizeof(e));
    memset(D, 0, sizeof(D));
    ans = flow = 0;
    cnt = 1;
    V = 0;
    S = ++V;
    T = ++V;
    add_edge(T, S, 0, INF, 0);
}
void Run() {
    while (spfa()) mcf();
    for (int i = g[SS]; i; i = e[i].nxt)
        if (e[i].v) ans = -1;
}
}  // namespace ULB_Min_Cost_Max_Flow