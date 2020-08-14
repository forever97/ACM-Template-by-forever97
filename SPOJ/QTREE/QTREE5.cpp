/*
    题目大意：给出一棵树，边权均为1，一开始所有点都是白点，
    要求维护两种操作：
        1.颜色取反（黑白互变）
        2.查询树上某点距离最近的白点与其距离
    题解：我们维护重心树，对于每个重心维护分治层所有白点到其距离，
    将其保存在对应方向子重心的优先队列中，
    重心处另外维护一个队列表示每个子重心的最小答案，
    考虑修改问题，等价于在处理优先队列的删除问题，
    对于每个需要删除操作的优先队列，我们额外创建一个优先队列将删除元素加入其中，
    当两个队列top元素相同时同时删去即可。
    对于查询操作，我们沿着重心链用经过每个重心的最优值更新答案
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
    priority_queue<int, vector<int>, greater<int> > Q, D;
    void clear() {
        while (!Q.empty()) Q.pop();
        while (!D.empty()) D.pop();
    }
    void add(int x) { Q.push(x); }
    void del(int x) { D.push(x); }
    int top() {
        for (;;) {
            if (Q.empty())
                return INF;
            else if (!D.empty() && Q.top() == D.top())
                Q.pop(), D.pop();
            else
                return Q.top();
        }
    }
} P[N], Q[N];
int dis[N], col[N];
void getdis(int rt, int x, int fx) {
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
    if (fx) {
        dis[from] = d;
        getdis(x, from, fx);
    }
    vis[x] = 1;
    for (int i = g[x]; ~i; i = nxt[i])
        if (!vis[v[i]]) {
            root = 0;
            sum = size[v[i]];
            getroot(v[i], x);
            build(root, x, v[i], w[i]);
        }
}
void change(int rt) {
    if (col[rt])
        P[rt].add(0);
    else
        P[rt].del(0);
    int x = rt;
    for (int i = G[rt]; ~NXT[i]; i = NXT[i]) {
        int y = V[NXT[i]], len = W[NXT[i]];
        x = V[i];
        int a = Q[x].top();
        if (col[rt])
            Q[x].add(len);
        else
            Q[x].del(len);
        int b = Q[x].top();
        if (a != b) P[y].del(a), P[y].add(b);
    }
}
int ask(int rt) {
    int ans = INF;
    for (int i = G[rt]; ~i; i = NXT[i]) {
        int x = V[i], len = W[i];
        int a = P[x].top();
        if (a + len < ans) ans = a + len;
    }
    return ans;
}
int n, m, a, b, c, x, op;
int main() {
    scanf("%d", &n);
    init();
    for (int i = 1; i <= n; i++) col[i] = 0;
    for (int i = 1; i < n; i++) {
        scanf("%d%d", &a, &b);
        add_edge(a, b, 1);
        add_edge(b, a, 1);
    }
    memset(vis, 0, sizeof(vis));
    dp[root = 0] = sum = n;
    getroot(1, 0);
    build(root, 0, 0, 0);
    scanf("%d", &m);
    int cnt = 0;
    while (m--) {
        scanf("%d%d", &op, &x);
        if (op == 1) {
            if (cnt)
                printf("%d\n", ask(x));
            else
                puts("-1");
        } else {
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
