/*
    重心树
    将分治层的重心和父重心之间连边构树
    题目大意：给出一棵点权树，允许修改点权，询问与点x距离差小于等于d的点的点权和
    题解：对于原树建立重心树，对于每个点维护两棵权值线段树，
    分别表示以该点为重心的分治层中到这个点的距离的点集以及到这个点重心树父亲距离的点集
    在统计x节点的时候只要统计其到重心树根的链上每个重心的贡献值累加即可，
    每个重心的贡献要计算折算后的距离在第一棵线段树中的答案，
    减去子重心的父重心在第二棵线段树的统计答案，即删去Fx和x之间的不该被统计的点权。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 200010, LEV = 20;
int dph[N];
int fa[N][LEV];
vector<int> G[N];
inline void add(int a, int b) {
    G[a].push_back(b);
    G[b].push_back(a);
}
void dfs(int rt, int f) {
    for (int i = 1; i < LEV; i++) {
        if (dph[rt] - 1 < (1 << i)) break;
        fa[rt][i] = fa[fa[rt][i - 1]][i - 1];
    }
    for (int v : G[rt]) {
        if (v == f) continue;
        dph[v] = dph[rt] + 1;
        fa[v][0] = rt;
        dfs(v, rt);
    }
}
inline int lca(int a, int b) {
    if (dph[a] < dph[b]) swap(a, b);
    int t = dph[a] - dph[b];
    for (int i = 0; i < LEV; i++)
        if ((1 << i) & t) a = fa[a][i];
    for (int i = LEV - 1; i >= 0; i--) {
        if (fa[a][i] != fa[b][i]) a = fa[a][i], b = fa[b][i];
    }
    if (a != b) return fa[a][0];
    return a;
}
inline int dis(int x, int y) { return dph[x] + dph[y] - 2 * dph[lca(x, y)]; }
void Initialize(int n) {
    for (int i = 1; i <= n; i++) G[i].clear();
    memset(fa, 0, sizeof(fa));
    dph[1] = 1;
}
int K;
int sum, root, size[N], dp[N], vis[N];
void getroot(int x, int fx) {
    size[x] = 1;
    dp[x] = 0;
    for (int v : G[x]) {
        if (!vis[v] && v != fx) {
            getroot(v, x);
            size[x] += size[v];
            dp[x] = max(dp[x], size[v]);
        }
    }
    dp[x] = max(dp[x], sum - size[x]);
    if (dp[x] < dp[root]) root = x;
}
// 建立重心树
int belong[N];
void build(int x, int fx) {
    belong[x] = fx;
    vis[x] = 1;
    for (int v : G[x])
        if (!vis[v]) {
            root = 0;
            sum = size[v];
            getroot(v, x);
            build(root, x);
        }
}
int n, m, ans, tot, rt[N], rtc[N];
struct data {
    int l, r, sum;
} T[N * 60];
inline void up(int x) { T[x].sum = T[T[x].l].sum + T[T[x].r].sum; }
void insert(int &rt, int l, int r, int x, int val) {
    if (!rt) {
        rt = ++tot;
        T[rt].l = T[rt].r = T[rt].sum = 0;
    }
    if (l == r) {
        T[rt].sum += val;
        return;
    }
    int mid = (l + r) >> 1;
    if (x <= mid)
        insert(T[rt].l, l, mid, x, val);
    else
        insert(T[rt].r, mid + 1, r, x, val);
    up(rt);
}
int query(int x, int l, int r, int L, int R) {
    if (L > R) return 0;
    if (L <= l && r <= R) return T[x].sum;
    int mid = (l + r) >> 1, res = 0;
    if (L <= mid) res += query(T[x].l, l, mid, L, R);
    if (R > mid) res += query(T[x].r, mid + 1, r, L, R);
    return res;
}
void addval(int fx, int x, int val) {
    int D = dis(fx, x);
    insert(rt[fx], 0, n, D, val);
    if (!belong[fx]) return;
    int Fx = belong[fx];
    D = dis(Fx, x);
    insert(rtc[fx], 0, n, D, val);
    addval(Fx, x, val);
}
void ask(int x, int son, int v, int d) {
    if (!x) return;
    if (x == son)
        ans += query(rt[x], 0, n, 0, d);
    else {
        int D = dis(x, v);
        ans += query(rt[x], 0, n, 0, d - D);
        ans -= query(rtc[son], 0, n, 0, d - D);
    }
    ask(belong[x], x, v, d);
}
int a[N];
int main() {
    while (~scanf("%d%d", &n, &m)) {
        tot = 0;
        Initialize(n);
        memset(rt, 0, sizeof(rt));
        memset(rtc, 0, sizeof(rtc));
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        for (int i = 1; i < n; i++) {
            int x, y;
            scanf("%d%d", &x, &y);
            add(x, y);
        }
        dfs(1, 0);
        memset(vis, 0, sizeof(vis));
        dp[root = 0] = sum = n;
        getroot(1, 0);
        build(root, 0);
        for (int i = 1; i <= n; i++) addval(i, i, a[i]);
        while (m--) {
            char op[10];
            int x, y;
            ans = 0;
            scanf("%s%d%d", op, &x, &y);
            if (op[0] == '!')
                addval(x, x, y - a[x]), a[x] = y;
            else {
                ask(x, x, x, y);
                printf("%d\n", ans);
            }
        }
    }
    return 0;
}
