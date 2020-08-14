/*
    求k短路
    Astar算法
*/
int n, m, s, t, k, dis[MAXN];
struct node {
    int v, c;
    node(int v, int c) : v(v), c(c) {}
    //用于优先队列先出的条件
    inline bool operator<(const node &b) const {
        return c + dis[v] > b.c + dis[b.v];
    }
};
vector<node> map1[MAXN];  //用于dijkstra算法
vector<node> map2[MAXN];  //用于A_star算法
void dijkstra() {
    int i, find[MAXN], v;
    for (i = 1; i <= n; i++) dis[i] = INF;
    memset(find, 0, sizeof(find));
    priority_queue<node> heap;
    dis[t] = 0;
    heap.push(node(t, 0));
    while (!heap.empty()) {
        v = heap.top().v;
        heap.pop();
        if (find[v]) continue;
        find[v] = 1;
        for (i = 0; i < map1[v].size(); i++)
            if (!find[map1[v][i].v] &&
                dis[v] + map1[v][i].c < dis[map1[v][i].v]) {
                dis[map1[v][i].v] = dis[v] + map1[v][i].c;
                heap.push(node(map1[v][i].v, dis[map1[v][i].v]));
            }
    }
}
int A_star() {
    int i, cnt[MAXN], v, g;
    if (dis[s] == INF) return -1;
    priority_queue<node> heap;
    memset(cnt, 0, sizeof(cnt));
    heap.push(node(s, 0));  // 0是g(x)
    while (!heap.empty()) {
        v = heap.top().v;
        g = heap.top().c;
        heap.pop();
        cnt[v]++;
        if (cnt[t] == k) return g;
        if (cnt[v] > k) continue;
        for (i = 0; i < map2[v].size(); i++)
            heap.push(node(map2[v][i].v, g + map2[v][i].c));
    }
    return -1;
}
int main() {
    int i, u, v, c;
    scanf("%d%d", &n, &m);
    for (i = 0; i < m; i++) {
        scanf("%d%d%d", &u, &v, &c);
        map2[u].push_back(node(v, c));
        map1[v].push_back(node(u, c));  //反向储存求各节点到目标节点的最短距离
    }
    scanf("%d%d%d", &s, &t, &k);
    if (s == t) k++;
    dijkstra();
    int ans = A_star();
    printf("%d\n", ans);
    return 0;
}