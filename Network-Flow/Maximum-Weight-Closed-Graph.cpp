/*
    最大权闭合图
    有向图点权可正可负。
    对于任意一条有向边i和j，选择了点i就必须选择点j，
    选择一些点使得得到权值最大，同时删去的点最少
*/
using namespace Dinic;
const int MAX_N = 5000;
const int MAX_M = 60000;
int N, M, w[MAX_N], a[MAX_M], b[MAX_M];
LL max_weight_closure(int s, int t) {
    LL W = 0;
    V = t;
    for (int i = 0; i <= V; i++) G[i].clear();
    for (int i = 0; i < N; i++) {
        if (w[i] > 0) W += w[i], add_edge(s, i, w[i]);
        if (w[i] < 0) add_edge(i, t, -w[i]);
    }
    for (int i = 0; i < M; i++) {
        add_edge(a[i] - 1, b[i] - 1, INF);
    }
    return W - max_flow(s, t);
}
int leftv, vis[MAX_V];
// 遍历残余网络
void cal_res_net(int v) {
    ++leftv;
    vis[v] = 1;
    for (int i = 0; i < G[v].size(); i++) {
        edge &e = G[v][i];
        if (e.cap > 0 && !vis[e.to]) cal_res_net(e.to);
    }
}
void init() {
    for (int i = 0; i < N; i++) scanf("%d", &w[i]);
    for (int i = 0; i < M; i++) scanf("%d%d", &a[i], &b[i]);
}
void solve() {
    int s = N, t = N + 1;
    LL max_profit = max_weight_closure(s, t);
    memset(vis, 0, sizeof(vis));
    leftv = 0;
    cal_res_net(s);
    printf("%d %lld\n", --leftv, max_profit);
}
int main() {
    while (~scanf("%d%d", &N, &M)) {
        init();
        solve();
    }
    return 0;
}