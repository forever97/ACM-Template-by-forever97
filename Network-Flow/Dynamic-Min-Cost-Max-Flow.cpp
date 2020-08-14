/*
    动态加边网络流
    Example
        修车问题
        n种型号的车由m个工人修理需要不同的时间t[i][j]，
        第i种型号的车有c[i]辆。
        问最少的总等待时间
    Solution
        对工人进行拆点，
        点[(i-1)*tot+j]表示第i个工人修理自己负责的倒数第j辆车
        型号为k的车代价为t[k][i]*(j-1)
        考虑数据规模过大，进行动态加边
        某位工人在修理倒数第j辆车时
        再把他修理倒数第j+1辆车的边加上
*/
const int INF = 0x7fffffff, N = 100010, M = 3000010;
namespace Dynamic_Min_Cost_Max_Flow {
int n, m, S = 0, T = 100001, flow, tot, cnt, ans, t[45][105];
int c[45], d[N], q[N], from[N], g[N];
bool inq[N];
struct edge {
    int from, to, nxt, c, v;
} e[M];
void Initialize() {
    memset(g, 0, sizeof(g));
    memset(e, 0, sizeof(e));
    ans = flow = 0;
    cnt = 1;
}
void add(int u, int v, int w, int c) {
    cnt++;
    e[cnt].from = u;
    e[cnt].to = v;
    e[cnt].nxt = g[u];
    g[u] = cnt;
    e[cnt].c = c;
    e[cnt].v = w;
}
void add_edge(int u, int v, int w, int c) {
    add(u, v, w, c);
    add(v, u, 0, -c);
}
bool spfa() {
    for (int i = 0; i <= T; i++) d[i] = INF;
    int t = 0, w = 1;
    d[S] = 0;
    inq[0] = 1;
    q[S] = 0;
    while (t != w) {
        int now = q[t];
        t++;
        if (t == T) t = 0;
        for (int i = g[now]; i; i = e[i].nxt)
            if (e[i].v && d[e[i].to] > d[now] + e[i].c) {
                d[e[i].to] = d[now] + e[i].c;
                from[e[i].to] = i;
                if (!inq[e[i].to]) {
                    inq[e[i].to] = 1;
                    q[w++] = e[i].to;
                    if (w == T) w = 0;
                }
            }
        inq[now] = 0;
    }
    if (d[T] == INF) return 0;
    return 1;
}
void mcf() {
    int x = INF, a, b, y;
    for (int i = from[T]; i; i = from[e[i].from]) {
        x = min(x, e[i].v);
        flow += x;
        if (e[i].from == 0) {
            y = e[i].to;
            a = (y - 1) / tot + 1;
            b = y % tot + 1;
        }
    }
    for (int i = from[T]; i; i = from[e[i].from]) {
        e[i].v -= x;
        e[i ^ 1].v += x;
        ans += e[i].c * x;
    }
    for (int i = 1; i <= n; i++)
        add_edge((a - 1) * tot + b, m * tot + i, 1, b * t[i][a]);
}
}  // namespace Dynamic_Min_Cost_Max_Flow
using namespace Dynamic_Min_Cost_Max_Flow;
int main() {
    Initialize();
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &c[i]);
        tot += c[i];
    }
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) scanf("%d", &t[i][j]);
    for (int i = 1; i <= m * tot; i++) add_edge(S, i, 1, 0);
    for (int i = 1; i <= n; i++) add_edge(m * tot + i, T, c[i], 0);
    for (int i = 1; i <= m; i++)
        for (int k = 1; k <= n; k++)
            add_edge((i - 1) * tot + 1, m * tot + k, 1, t[k][i]);
    while (spfa()) mcf();
    printf("%d", ans);
    return 0;
}