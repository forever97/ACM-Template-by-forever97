/*
    线段树优化建图+最短路
    给出一些星球，现在有一些传送枪，可以从一个星球到另一个星球，
    从一个星球到另一些星球，或者从一些星球到某个星球，
    每种传送枪使用一次要花费不同的价格
    地球是其中一个星球，问从地球到其它星球的最少花费是多少
*/
const int N = 100010;
const int V = N * 5;
int n, m, s;
vector<pair<int, int> > G[V];
void addedge(int u, int v, int c) { G[u].push_back(make_pair(v, c)); }
int id[2][N << 2], idx;
void build(int x, int l, int r, int k) {
    id[k][x] = ++idx;
    if (l == r) {
        if (k == 0)
            addedge(id[k][x], l, 0);
        else
            addedge(l, id[k][x], 0);
        return;
    }
    int mid = (l + r) >> 1;
    build(x << 1, l, mid, k);
    build(x << 1 | 1, mid + 1, r, k);
    if (k == 0) {
        addedge(id[k][x], id[k][x << 1], 0);
        addedge(id[k][x], id[k][x << 1 | 1], 0);
    } else {
        addedge(id[k][x << 1], id[k][x], 0);
        addedge(id[k][x << 1 | 1], id[k][x], 0);
    }
}
vector<int> vs;
void get(int x, int l, int r, int L, int R, int k) {
    if (L <= l && r <= R) {
        vs.push_back(id[k][x]);
        return;
    }
    int mid = (l + r) >> 1;
    if (L <= mid) get(x << 1, l, mid, L, R, k);
    if (R > mid) get(x << 1 | 1, mid + 1, r, L, R, k);
}
typedef long long LL;
const LL LLINF = 0x3f3f3f3f3f3f3f3fLL;
LL dis[V];
bool vis[V];
void Dijkstra(int s) {
    for (int i = 1; i <= 5 * n; i++) vis[i] = 0, dis[i] = LLINF;
    priority_queue<pair<LL, int> > q;
    q.push(make_pair(-0, s));
    dis[s] = 0;
    while (!q.empty()) {
        int u = q.top().second;
        q.pop();
        if (vis[u]) continue;
        vis[u] = 1;
        for (int i = 0; i < G[u].size(); i++) {
            int v = G[u][i].first, c = G[u][i].second;
            if (dis[v] > dis[u] + c) {
                dis[v] = dis[u] + c;
                q.push(make_pair(-dis[v], v));
            }
        }
    }
}
int main() {
    while (~scanf("%d%d%d", &n, &m, &s)) {
        for (int i = 0; i <= 5 * n; i++) G[i].clear();
        idx = n;
        build(1, 1, n, 0);
        build(1, 1, n, 1);
        while (m--) {
            int t, u;
            scanf("%d%d", &t, &u);
            if (t == 1) {
                int v, c;
                scanf("%d%d", &v, &c);
                addedge(u, v, c);
            } else if (t == 2) {
                vs.clear();
                int l, r, c;
                scanf("%d%d%d", &l, &r, &c);
                get(1, 1, n, l, r, 0);
                for (int i = 0; i < vs.size(); i++) addedge(u, vs[i], c);
            } else {
                vs.clear();
                int l, r, c;
                scanf("%d%d%d", &l, &r, &c);
                get(1, 1, n, l, r, 1);
                for (int i = 0; i < vs.size(); i++) addedge(vs[i], u, c);
            }
        }
        Dijkstra(s);
        for (int i = 1; i <= n; i++) {
            if (dis[i] == LLINF) dis[i] = -1;
            printf("%lld%c", dis[i], i == n ? '\n' : ' ');
        }
    }
    return 0;
}