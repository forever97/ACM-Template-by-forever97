/*
    最小费用最大流
    限制：S<T
*/
namespace Min_Cost_Max_Flow {
const int INF = 0x3f3f3f3f;
int S, T, cnt, ans, d[N], from[N], g[N], flow;
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
void add_edge(int u, int v, int w, int c) {
    add(u, v, w, c);
    add(v, u, 0, -c);
}
bool spfa() {
    memset(d, INF, sizeof(d));
    d[S] = 0;
    memset(from, 0, sizeof(from));
    queue<int> q;
    q.push(S);
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
    return (d[T] != INF);
    // 改为return(d[T]<=0)即为最小费用可行流
}
void mcf() {
    int x = INF;
    for (int i = from[T]; i; i = from[e[i].from]) x = min(x, e[i].v);
    flow += x;
    for (int i = from[T]; i; i = from[e[i].from]) {
        e[i].v -= x;
        e[i ^ 1].v += x;
        ans += e[i].c * x;
    }
}
void Initialize() {
    memset(g, 0, sizeof(g));
    memset(e, 0, sizeof(e));
    ans = flow = 0;
    cnt = 1;
}
void doit() {
    while (spfa()) mcf();
}
}  // namespace Min_Cost_Max_Flow
/*
    纸巾模型：
        每天对毛巾都有一定的需求ni，每天可以花f价值每条购买毛巾，
        当天用完的毛巾可以花费fA价值每条通过快消毒在A天之后得到一条可用的，
        也可以通过花费fB价值每条，通过慢消毒在B天之后获得可用的
        问满足每天需求所用的最小花费。
    Solution：
        首先，我们建立X集合，表示每天用完之后需要消毒的毛巾，显然第i个点值为ni，
        建立Y集合表示每天需要的毛巾数量，第i个点为ni，
        对于X中每个点，向A天后对应Y中的每个点连流量为INF，费用为fA的边
        同时向B题后对应Y中的每个点连流量为INF，费用为fB的边
        源点向X中第i个点连ni流量0费用的边，Y中第i个点向汇点连ni流量0费用的边，
        对于购买新毛巾的操作，我们从源点向Y中每个点连INF流量f费用的边
        那么求该图的最小费用最大流就是答案。
        但是我们发现按照以上方式建图边数量非常的庞大。
        考虑建图优化，我们对于X中每个点i向i+1连边，
        而对于X向Y的连边，我们只要连到有效区间的起点即可，
        这样就等价于X中每个点，向A(B)天后对应Y中的每个点连流量为INF，费用为fA(fB)的边
*/
void solve() {
    namespace Min_Cost_Max_Flow;
    scanf("%d%d%d%d%d%d", &n, &A, &B, &f, &fA, &fB) Initialize();
    S = 0;
    T = 2 * n + 1;
    for (int i = 1; i <= n; i++) {
        int x;
        scanf("%d", &x);
        add_edge(S, i, x, 0);
        add_edge(i + n, T, x, 0);
    }
    for (int i = 1; i < n; i++) add_edge(i, i + 1, INF, 0);
    for (int i = 1; i <= n; i++) add_edge(S, i + n, INF, f);
    for (int i = 1; i + A + 1 <= n; i++) add_edge(i, i + A + 1 + n, INF, fA);
    for (int i = 1; i + B + 1 <= n; i++) add_edge(i, i + B + 1 + n, INF, fB);
    doit();
    printf("%d\n", ans);
}
