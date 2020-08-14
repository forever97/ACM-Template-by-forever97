/*
    树点分治
*/
int ans, sum, root, size[N], dp[N], vis[N];
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
void solve(int x) {
    cal(x);
    vis[x] = 1;
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]]) {
            root = 0;
            dp[0] = sum = size[v[i]];
            getroot(v[i], 0);
            solve(root);
        }
    }
}
void doit() {
    memset(vis, 0, sizeof(vis));
    dp[root = 0] = sum = n;
    getroot(1, 0);
    solve(root);
}
/*
    Example
    查询树上距离不超过K的点对数
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
typedef long long LL;
const int N = 100100;
// Treap
struct node {
    LL val;
    int cnt, sum, p;
    node *l, *r;
    node() {
        val = cnt = sum = p = 0;
        l = r = NULL;
    }
    inline void up() { sum = cnt + l->sum + r->sum; }
} *blank = new (node), *T, pool[N], *cur = pool;
inline void Rotatel(node *&x) {
    node *y = x->r;
    x->r = y->l;
    x->up();
    y->l = x;
    y->up();
    x = y;
}
inline void Rotater(node *&x) {
    node *y = x->l;
    x->l = y->r;
    x->up();
    y->r = x;
    y->up();
    x = y;
}
inline void Ins(node *&x, int p) {
    if (x == blank) {
        x = cur++;
        x->val = p;
        x->l = x->r = blank;
        x->cnt = x->sum = 1;
        x->p = std::rand();
        return;
    }
    x->sum++;
    if (p == x->val) {
        x->cnt++;
        return;
    }
    if (p < x->val) {
        Ins(x->l, p);
        if (x->l->p > x->p) Rotater(x);
    } else {
        Ins(x->r, p);
        if (x->r->p > x->p) Rotatel(x);
    }
}
inline int Ask(node *x, LL p) {
    if (x == blank) return 0;
    if (p == x->val) return x->l->sum + x->cnt;
    if (p > x->val) return x->cnt + x->l->sum + Ask(x->r, p);
    return Ask(x->l, p);
}
// 邻接表
LL w[N << 1];
int tot, g[N], nxt[N << 1], v[N << 1];
void add_edge(int x, int y, int z) {
    v[tot] = y, w[tot] = z, nxt[tot] = g[x], g[x] = tot++;
}
// 树分治
LL K;
int ans, sum, root, size[N], dp[N], vis[N];
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
inline void dfscal(int x, LL l, int pre) {
    if (l > K) return;
    ans += Ask(T, K - l);
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]] && v[i] != pre) dfscal(v[i], l + w[i], x);
    }
}
inline void dfsadd(int x, LL l, int pre) {
    if (l > K) return;
    Ins(T, l);
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]] && v[i] != pre) dfsadd(v[i], l + w[i], x);
    }
}
void cal(int x) {
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]]) dfscal(v[i], w[i], x), dfsadd(v[i], w[i], x);
    }
}
void solve(int x) {
    cur = pool;
    Ins(T = blank, 0);
    cal(x);
    vis[x] = 1;
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]]) {
            root = 0;
            dp[0] = sum = size[v[i]];
            getroot(v[i], 0);
            solve(root);
        }
    }
}
int n, m;
int main() {
    blank->l = blank->r = blank;
    while (~scanf("%d%d", &n, &m)) {
        memset(g, -1, sizeof(g));
        memset(vis, 0, sizeof(vis));
        ans = tot = 0;
        char str[10];
        int a, b;
        LL c;
        for (int i = 0; i < m; i++) {
            scanf("%d%d%lld%s", &a, &b, &c, str);
            add_edge(a, b, c);
            add_edge(b, a, c);
        }
        scanf("%lld", &K);
        ans = K ? 0 : n;
        dp[root = 0] = sum = n;
        getroot(1, 0);
        solve(root);
        printf("%d\n", ans);
    }
    return 0;
}
/*
    Example
    在点权树上查询点权相乘对P取模为K的链，要求输出起点终点字典序最小的链(不能是单点)
    维护hash保存某个链权值的最小端点，沿dfs查询即可，
    注意点权的特殊处理方式，存值时起始点要包含重心，查询时不包含
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int P = 1000003;
const int N = 1000010;
const int INF = 0x3f3f3f3f;
int inv[N], mp[N];
void pre_inv() {
    inv[1] = 1;
    for (int i = 2; i < P; i++) {
        int a = P / i, b = P % i;
        inv[i] = (1LL * inv[b] * (-a) % P + P) % P;
    }
}
// 邻接表
int tot, g[N], nxt[N << 1], v[N << 1];
void init() {
    memset(g, -1, sizeof(g));
    tot = 0;
}
void add_edge(int x, int y) { v[tot] = y, nxt[tot] = g[x], g[x] = tot++; }
// 树分治
int K;
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
int ans1, ans2, val[N];
void Insert(int x, int L) {
    int now = mp[L];
    if (!now || x < now) mp[L] = x;
}
void Query(int x, int L) {
    L = 1LL * inv[L] * K % P;
    int y = mp[L];
    if (y == 0) return;
    if (y < x) swap(y, x);
    if (x < ans1 || (x == ans1 && y < ans2)) ans1 = x, ans2 = y;
}
// 三个相似的函数注意不要写串
void dfsadd(int x, int w, int pre) {
    Insert(x, w);
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]] && v[i] != pre)
            dfsadd(v[i], (1LL * w * val[v[i]]) % P, x);
    }
}
void dfscal(int x, int w, int pre) {
    Query(x, w);
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]] && v[i] != pre)
            dfscal(v[i], (1LL * w * val[v[i]]) % P, x);
    }
}
void dfsdel(int x, int w, int pre) {
    mp[w] = 0;
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]] && v[i] != pre)
            dfsdel(v[i], (1LL * w * val[v[i]]) % P, x);
    }
}
void cal(int x) {
    mp[val[x]] = x;  // 将重心加入查询
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]])
            dfscal(v[i], val[v[i]], x),
                dfsadd(v[i], 1LL * val[v[i]] * val[x] % P, x);
    }
    for (int i = g[x]; ~i; i = nxt[i])
        if (!vis[v[i]]) dfsdel(v[i], 1LL * val[v[i]] * val[x] % P, x);
    mp[val[x]] = 0;  // 注意重心的删除
}
void solve(int x) {
    cal(x);
    vis[x] = 1;
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]]) {
            root = 0;
            dp[0] = sum = size[v[i]];
            getroot(v[i], 0);
            solve(root);
        }
    }
}
void read(int &x) {
    x = 0;
    char ch = getchar();
    while (ch < '0' || ch > '9') ch = getchar();
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
}
int n;
int main() {
    pre_inv();
    while (~scanf("%d%d", &n, &K)) {
        init();
        for (int i = 1; i <= n; i++) read(val[i]);
        for (int i = 1; i < n; i++) {
            int x, y;
            read(x);
            read(y);
            add_edge(x, y);
            add_edge(y, x);
        }
        ans1 = ans2 = INF;
        memset(vis, 0, sizeof(vis));
        dp[root = 0] = sum = n;
        getroot(1, 0);
        solve(root);
        if (ans1 == INF)
            puts("No solution");
        else
            printf("%d %d\n", ans1, ans2);
    }
    return 0;
}
