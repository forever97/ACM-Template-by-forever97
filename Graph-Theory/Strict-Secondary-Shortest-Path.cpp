/*
    严格次短路
*/
const int N = 200100;
const int INF = ~0U >> 2;
typedef pair<int, int> seg;
priority_queue<seg, vector<seg>, greater<seg> > q;
int p[N], d[2][N], head[N], u[N], v[N], w[N], nxt[N], n, m, a[N], b[N], c[N],
    ed = 0, H, x[N], y[N];
bool vis[N];
void add(int a, int b, int c) {
    u[++ed] = a, v[ed] = b, w[ed] = c;
    nxt[ed] = head[u[ed]];
    head[u[ed]] = ed;
}
void Dijkstra(int src, int t) {
    memset(vis, 0, sizeof(vis));
    for (int i = 0; i <= n + 1; i++) d[t][i] = INF;
    d[t][src] = 0;
    q.push(make_pair(d[t][src], src));
    while (!q.empty()) {
        seg now = q.top();
        q.pop();
        int x = now.second;
        if (vis[x]) continue;
        vis[x] = true;
        for (int e = head[x]; e != -1; e = nxt[e])
            if (d[t][v[e]] > d[t][x] + w[e]) {
                d[t][v[e]] = d[t][x] + w[e];
                q.push(make_pair(d[t][v[e]], v[e]));
            }
    }
}
void Solve() {
    scanf("%d%d", &n, &m);
    memset(head, -1, sizeof(head));
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d", &a[i], &b[i], &c[i]);
        add(a[i], b[i], c[i]);
        add(b[i], a[i], c[i]);
    }
    Dijkstra(1, 0);
    Dijkstra(n, 1);
    for (int i = 1; i <= 2 * m; i++) {
        p[2 * i - 1] = c[i] + d[0][a[i]] + d[1][b[i]];
        p[2 * i] = c[i] + d[1][a[i]] + d[0][b[i]];
    }
    sort(p + 1, p + 2 * m + 1);
    for (int i = 2; i <= m; i++)
        if (p[i] != p[i - 1]) {
            printf("%d\n", p[i]);
            return;
        }
}