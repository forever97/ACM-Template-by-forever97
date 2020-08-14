/*
    题目大意：给出一棵边权树，一开始所有点都是白点，
    要求维护两种操作：
        1.颜色取反（黑白互变）
        2.查询树上最远的两个白点之间的距离
    题解：我们维护重心树，对于每个重心维护分治层所有白点到其距离，
    将其保存在对应方向子重心的优先队列中，
    重心处另外维护一个队列保存该分治层每个子重心所产生的最大答案，即最大值加次大值，
    那么每个重心处产生的最大答案的最值就是答案，
    考虑修改问题，等价于在处理优先队列的删除问题，
    对于每个需要删除操作的优先队列，我们额外创建一个优先队列将删除元素加入其中，
    当两个队列top元素相同时同时删去即可。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;
const int N = 200010;
const int INF = 0x3f3f3f3f;
int tot, g[N], nxt[N << 1], v[N << 1], w[N << 1];
int TOT, G[N], NXT[N * 20], V[N * 20], W[N * 20];
void init() {
    memset(g, -1, sizeof(g));
    tot = 0;
    memset(G, -1, sizeof(G));
    TOT = 0;
}
void add_edge(int x, int y, int z) {
    v[tot] = y, w[tot] = z, nxt[tot] = g[x], g[x] = tot++;
}
void ADD_EDGE(int x, int y, int z) {
    V[TOT] = y, W[TOT] = z, NXT[TOT] = G[x], G[x] = TOT++;
}
int sum, root, size[N], dp[N], vis[N];
void getroot(int x, int fx) {
    size[x] = 1;
    dp[x] = 0;
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]] && v[i] != fx) {
            getroot(v[i], x);
            size[x] += size[v[i]];
            dp[x] = max(dp[x], size[v[i]]);
        }
    }
    dp[x] = max(dp[x], sum - size[x]);
    if (dp[x] < dp[root]) root = x;
}
struct Que {
    priority_queue<int> Q, D;
    void clear() {
        while (!Q.empty()) Q.pop();
        while (!D.empty()) D.pop();
    }
    void add(int x) { Q.push(x); }
    void del(int x) { D.push(x); }
    int top() {
        for (;;) {
            if (Q.empty())
                return -INF;
            else if (!D.empty() && Q.top() == D.top())
                Q.pop(), D.pop();
            else
                return Q.top();
        }
    }
    int toptwo() {
        int x = top();
        del(x);
        int y = top();
        add(x);
        if (y == -INF) return x == -INF ? x : 0;
        return max(x + y, 0);
    }
} P[N], Q[N], ans;
int dis[N], col[N];
void getdis(int rt, int x, int fx) {
    Q[rt].add(dis[x]);
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (v[i] == fx || vis[v[i]]) continue;
        dis[v[i]] = dis[x] + w[i];
        getdis(rt, v[i], x);
    }
}
void getdeep(int rt, int x, int fx) {
    ADD_EDGE(x, rt, dis[x]);
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (v[i] == fx || vis[v[i]]) continue;
        dis[v[i]] = dis[x] + w[i];
        getdeep(rt, v[i], x);
    }
}
int belong[N];
void build(int x, int fx, int from, int d) {
    belong[x] = fx;
    dis[x] = 0;
    getdeep(x, x, fx);
    P[x].add(0);
    if (fx) {
        dis[from] = d;
        getdis(x, from, fx);
        P[fx].add(Q[x].top());
    }
    vis[x] = 1;
    for (int i = g[x]; ~i; i = nxt[i])
        if (!vis[v[i]]) {
            root = 0;
            sum = size[v[i]];
            getroot(v[i], x);
            build(root, x, v[i], w[i]);
        }
    ans.add(P[x].toptwo());
}
void change(int rt) {
    int a = P[rt].toptwo();
    if (col[rt])
        P[rt].add(0);
    else
        P[rt].del(0);
    int b = P[rt].toptwo();
    if (a != b) ans.del(a), ans.add(b);
    int x = rt;
    for (int i = G[rt]; ~NXT[i]; i = NXT[i]) {
        int y = V[NXT[i]], len = W[NXT[i]];
        x = V[i];
        a = Q[x].top();
        if (col[rt])
            Q[x].add(len);
        else
            Q[x].del(len);
        b = Q[x].top();
        if (a != b) {
            int c = P[y].toptwo();
            P[y].del(a), P[y].add(b);
            int d = P[y].toptwo();
            if (c != d) ans.del(c), ans.add(d);
        }
    }
}
int n, m, a, b, c, x;
char op[10];
int main() {
    scanf("%d", &n);
    init();
    for (int i = 1; i <= n; i++) col[i] = 1;
    for (int i = 1; i < n; i++) {
        scanf("%d%d%d", &a, &b, &c);
        add_edge(a, b, c);
        add_edge(b, a, c);
    }
    memset(vis, 0, sizeof(vis));
    dp[root = 0] = sum = n;
    getroot(1, 0);
    build(root, 0, 0, 0);
    scanf("%d", &m);
    int cnt = n;
    while (m--) {
        scanf("%s", op);
        if (op[0] == 'A') {
            if (cnt)
                printf("%d\n", ans.top());
            else
                puts("They have disappeared.");
        } else {
            scanf("%d", &x);
            col[x] ^= 1;
            if (col[x])
                cnt++;
            else
                cnt--;
            change(x);
        }
    }
    return 0;
}