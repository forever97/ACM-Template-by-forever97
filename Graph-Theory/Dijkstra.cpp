/*
    Dijkstra+优先队列
    O(nlogn)
*/
using namespace std;
const int N = 150100, M = 300010;
const LL INF = 1LL << 60;
typedef long long LL;
typedef pair<LL, int> seg;
priority_queue<seg, vector<seg>, greater<seg> > q;
int head[N], u[M], v[M], nxt[M];  // 双向边记得开[M<<1]
LL w[M], d[N];
bool vis[N];
int n, m, ed;
void init() {
    ed = 0;
    memset(head, -1, sizeof(head));
}
void add(int a, int b, LL c) {
    u[++ed] = a, v[ed] = b, w[ed] = c;
    nxt[ed] = head[u[ed]];
    head[u[ed]] = ed;
}
void Dijkstra(int src, LL d[]) {
    memset(vis, 0, sizeof(vis));
    for (int i = 0; i <= n; i++) d[i] = INF;
    d[src] = 0;
    q.push(make_pair(d[src], src));
    while (!q.empty()) {
        seg now = q.top();
        q.pop();
        int x = now.second;
        if (vis[x]) continue;
        vis[x] = true;
        for (int e = head[x]; e != -1; e = nxt[e])
            if (d[v[e]] > d[x] + w[e]) {
                d[v[e]] = d[x] + w[e];
                q.push(make_pair(d[v[e]], v[e]));
            }
    }
}
/*
    Test
    给出m个条件A-B<=C,求Xn-X1的最大值
    我们从无穷开始向小约束一定能得到最大值，所以我们从A向B连C的边，求一遍最短路即可
*/
int main() {
    while (~scanf("%d%d", &n, &m)) {
        int A, B, C;
        init();
        while (m--) {
            scanf("%d%d%lld", &A, &B, &C);
            add(A, B, C);
        }
        Dijkstra(1, d);
        printf("%lld\n", d[n]);
    }
    return 0;
}
/*
    Notice
    在多源Dijkstra中，不要建立超级源点，而是直接把点放入队列中，
    可以减少queue开支防止段错误，同时节省时间
*/
inline void Dijkstra() {
    while (!q.empty()) {
        seg t = q.top();
        q.pop();
        if (d[t.second] < t.first) continue;
        for (int i = head[t.second]; ~i; i = nxt[i]) {
            if (d[v[i]] > t.first + w[i])
                q.push(seg(d[v[i]] = t.first + w[i], v[i]));
        }
    }
}
int main() {
    scanf("%d%d", &n, &m);
    init();
    for (int i = 1; i <= n; i++) {
        int x;
        scanf("%d", &x);
        if (x == 1)
            q.push(seg(0, i)), d[i] = 0;
        else
            d[i] = INF;
    }
    /*[some other code]*/
}