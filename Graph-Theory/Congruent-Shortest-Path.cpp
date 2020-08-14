/* 同余最短路
 * 给定一张n个点m条边的无向带权图，
 * 问是否存在一条从st号点到en号的长度为T的路径，不需要是简单路径。
 * 取一条与st相连的权值最小的边w
 * 设d[i][j]表示从st到i，路径长度模2w为j时，路径长度的最小值。
 * 用最短路算法求出所有d[i][j],然后检查d[en][T%2w]是否不超过T即可
 */
typedef long long LL;
const int N = 1000010;
const LL INF = 0x3f3f3f3f3f3f3f3f;
typedef pair<LL, int> seg;
priority_queue<seg, vector<seg>, greater<seg> > q;
int head[N], u[N], v[N], w[N], nxt[N], n, m, ed = 0;
LL d[5][N], W;
void add(int a, int b, int c) {
    u[++ed] = a, v[ed] = b, w[ed] = c;
    nxt[ed] = head[u[ed]];
    head[u[ed]] = ed;
}
void Dijkstra(int src) {
    for (int i = 0; i <= n; i++)
        for (int j = 0; j < W; j++) d[i][j] = INF;
    q.push(make_pair(0, src));
    while (!q.empty()) {
        seg now = q.top();
        q.pop();
        LL _w = now.first;
        int x = now.second;
        if (_w > d[x][_w % W]) continue;
        for (int e = head[x]; e != -1; e = nxt[e]) {
            LL nw = _w + w[e];
            if (d[v[e]][nw % W] > nw) {
                d[v[e]][nw % W] = nw;
                q.push(make_pair(nw, v[e]));
            }
        }
    }
}
/* 例题
 * 给出四个点1,2,3,4，1和2，2和3，3和4，4和1之间有路相连，
 * 现在从2点出发，最后回到2点，要求路径大于等于K，问路径长度最短是多少
 */
int T;
LL k, d1, d2, d3, d4;
int main() {
    scanf("%d", &T);
    n = 4;
    while (T--) {
        LL ans = INF;
        memset(head, -1, sizeof(head));
        ed = 0;
        scanf("%lld%lld%lld%lld%lld", &k, &d1, &d2, &d3, &d4);
        if (d2 < d1)
            W = 2 * d2;
        else
            W = 2 * d1;
        add(3, 4, d3);
        add(4, 3, d3);
        add(2, 1, d1);
        add(1, 2, d1);
        add(2, 3, d2);
        add(3, 2, d2);
        add(1, 4, d4);
        add(4, 1, d4);
        Dijkstra(2);
        for (int i = 0; i < W; i++) {
            if (k <= d[2][i])
                ans = min(ans, d[2][i]);
            else
                ans = min(ans, d[2][i] + ((k - d[2][i] + W - 1) / W) * W);
        }
        printf("%lld\n", ans);
    }
    return 0;
}