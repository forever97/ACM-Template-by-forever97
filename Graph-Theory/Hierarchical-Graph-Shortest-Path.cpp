/*
    分层图最短路
    (有k条路可以无代价通过)
*/
const int N = 200100;
const int INF = ~0U >> 2;
typedef pair<int, int> seg;
priority_queue<seg, vector<seg>, greater<seg> > q;
int p[N], d[N][25], head[N], u[N], v[N], w[N], nxt[N], n, m, a, b, c, k,
    ed = 0, H, x[N], y[N];
bool vis[N][25];
void add(int a, int b, int c) {
    u[++ed] = a, v[ed] = b, w[ed] = c;
    nxt[ed] = head[u[ed]];
    head[u[ed]] = ed;
}
void Dijkstra(int src) {
    memset(vis, 0, sizeof(vis));
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= k; j++) d[i][j] = INF;
    d[src][0] = 0;
    q.push(make_pair(d[src][0], src));
    while (!q.empty()) {
        seg now = q.top();
        q.pop();
        int deep = now.second / (n + 1), x = now.second % (n + 1);
        if (vis[x][deep]) continue;
        vis[x][deep] = true;
        for (int e = head[x]; e != -1; e = nxt[e]) {
            if (d[v[e]][deep] > d[x][deep] + w[e]) {
                d[v[e]][deep] = d[x][deep] + w[e];
                q.push(make_pair(d[v[e]][deep], deep * (n + 1) + v[e]));
            }
            if (deep == k) continue;
            if (d[x][deep] < d[v[e]][deep + 1]) {
                d[v[e]][deep + 1] = d[x][deep];
                q.push(
                    make_pair(d[v[e]][deep + 1], (deep + 1) * (n + 1) + v[e]));
            }
        }
    }
}
void solve() {
    scanf("%d%d%d", &n, &m, &k);
    memset(head, -1, sizeof(head));
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d", &a, &b, &c);
        add(a, b, c);
        add(b, a, c);
    }
    Dijkstra(1);
    printf("%d\n", d[n][k]);
}