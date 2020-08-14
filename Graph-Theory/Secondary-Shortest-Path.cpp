/*
    次短路
*/
typedef long long LL;
const int MAX_V = 100010;
struct edge {
    int to;
    LL cost;
    edge() {}
    edge(int to, LL cost) : to(to), cost(cost) {}
};
typedef pair<LL, int> P;
vector<edge> G[MAX_V];
LL d1[MAX_V], d2[MAX_V];
void dijkstra(int s) {
    priority_queue<P, vector<P>, greater<P> > que;
    memset(d1, 0x3f, sizeof(d1));
    memset(d2, 0x3f, sizeof(d2));
    que.push(P(d1[s] = 0, s));
    while (!que.empty()) {
        P p = que.top();
        que.pop();
        int v = p.second, u;
        LL d;
        if (d2[v] < p.first) continue;
        for (int i = 0; i < G[v].size(); i++) {
            u = G[v][i].to;
            d = p.first + G[v][i].cost;
            if (d1[u] > d) {
                d2[u] = d1[u];
                d1[u] = d;
                que.push(P(d, u));
            } else if (d2[u] > d) {
                d2[u] = d;
                que.push(P(d, u));
            }
        }
    }
}
int T, n, m;
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++) G[i].clear();
        while (m--) {
            int x, y, z;
            scanf("%d%d%d", &x, &y, &z);
            G[x].push_back(edge(y, z));
            G[y].push_back(edge(x, z));
        }
        dijkstra(1);
        printf("%lld\n", d2[n]);
    }
    return 0;
}