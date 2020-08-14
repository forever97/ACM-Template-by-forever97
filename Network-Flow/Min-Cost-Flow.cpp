/*
    V=点数+1
    add_edge(from,to,cap,cost)
    min_cost_flow(s,t,f)：从s到t的流量为f的流的最小费用
*/
namespace Min_Cost_flow {
const int INF = 0x3f3f3f3f;
struct edge {
    int to, cap, cost, rev;
    edge(int to, int cap, int cost, int rev)
        : to(to), cap(cap), cost(cost), rev(rev) {}
};
const int MAX_V = 10010;
int V, dist[MAX_V], prevv[MAX_V], preve[MAX_V];
vector<edge> G[MAX_V];
void add_edge(int from, int to, int cap, int cost) {
    G[from].push_back(edge(to, cap, cost, G[to].size()));
    G[to].push_back(edge(from, 0, -cost, G[from].size() - 1));
}
int min_cost_flow(int s, int t, int f) {
    int res = 0;
    while (f > 0) {
        fill(dist, dist + V, INF);
        dist[s] = 0;
        bool update = 1;
        while (update) {
            update = 0;
            for (int v = 0; v < V; v++) {
                if (dist[v] == INF) continue;
                for (int i = 0; i < G[v].size(); i++) {
                    edge &e = G[v][i];
                    if (e.cap > 0 && dist[e.to] > dist[v] + e.cost) {
                        dist[e.to] = dist[v] + e.cost;
                        prevv[e.to] = v;
                        preve[e.to] = i;
                        update = 1;
                    }
                }
            }
        }
        if (dist[t] == INF) return -1;
        int d = f;
        for (int v = t; v != s; v = prevv[v]) {
            d = min(d, G[prevv[v]][preve[v]].cap);
        }
        f -= d;
        res += d * dist[t];
        for (int v = t; v != s; v = prevv[v]) {
            edge &e = G[prevv[v]][preve[v]];
            e.cap -= d;
            G[v][e.rev].cap += d;
        }
    }
    return res;
}
void Initialize() {
    for (int i = 0; i <= V; i++) G[i].clear();
}
}  // namespace Min_Cost_flow