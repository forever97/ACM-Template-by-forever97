/*
    树分治+容斥
    题目大意：给出一棵边权树，求链长小于等于W，链上边数小于等于L的链数量
    题解：我们求出树上重心，进行分治处理，对于当前分治层，
    计算出所有包含重心的链，记录链的长度和链上边数，我们将链按照长度排序，
    双指针逆序根据长度顺序将满足长度相加条件的链上边数加入树状数组维护，
    查询长度满足条件位置的统计和，因为这种计算多算了子树内部不过重心的答案，
    所以我们减去对于每个子树的以重心为起点的链计算出的答案，
    最后答案减去单点的链并去除偏序性
*/
const int N = 300010;
typedef pair<int, int> P;
long long ans;
int L, W;
// 树状数组 [注意0位置的处理]
int n, c[N];
void add(int x, int v) {
    if (!x)
        c[0] += v;
    else
        while (x <= n) c[x] += v, x += x & -x;
}
int query(int x) {
    int res = c[0];
    while (x) res += c[x], x -= x & -x;
    return res;
}
// 邻接表
int tot, g[N], nxt[N << 1], v[N << 1], w[N << 1];
void init() {
    memset(g, -1, sizeof(g));
    tot = 0;
}
void add_edge(int x, int y, int z) {
    v[tot] = y, w[tot] = z, nxt[tot] = g[x], g[x] = tot++;
}
// 树分治
int K;
int sum, top, root, size[N], dp[N], vis[N], len[N], dis[N];
P st[N];
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
void getdis(int x, int fx) {
    st[++top] = P(dis[x], len[x]);
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]] && v[i] != fx) {
            len[v[i]] = len[x] + 1;
            dis[v[i]] = dis[x] + w[i];
            getdis(v[i], x);
        }
    }
}
void cal(int x, int _len, int _dis, int f) {
    len[x] = _len;
    dis[x] = _dis;
    getdis(x, top = 0);
    sort(st + 1, st + top + 1);
    int p = 1;
    for (int i = top; i; i--) {
        while (p <= top && st[p].first + st[i].first <= W)
            add(st[p++].second, 1);
        if (L >= st[i].second) ans += f * query(L - st[i].second);
    }
    for (int i = 1; i < p; i++) add(st[i].second, -1);
}
void solve(int x) {
    cal(x, 0, 0, 1);
    vis[x] = 1;
    // 删去不过重心的分支内部组合
    for (int i = g[x]; ~i; i = nxt[i])
        if (!vis[v[i]]) cal(v[i], 1, w[i], -1);
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]]) {
            root = 0;
            dp[0] = sum = size[v[i]];
            getroot(v[i], 0);
            solve(root);
        }
    }
}
int main() {
    while (~scanf("%d%d%d", &n, &L, &W)) {
        init();
        for (int i = 2; i <= n; i++) {
            int x, y;
            scanf("%d%d", &x, &y);
            add_edge(i, x, y);
            add_edge(x, i, y);
        }
        ans = 0;
        memset(vis, 0, sizeof(vis));
        dp[root = 0] = sum = n;
        getroot(1, 0);
        solve(root);
        // 去除自己到自己的组合并消除答案的偏序性
        printf("%lld\n", (ans - n) / 2);
    }
    return 0;
}
/*
    题目大意：给出一个城市的道路连通情况，城镇之间连接呈树状，
    有些城镇上有超市有些没有，居民想去超市会去离其最近的有超市的城市，
    如果一样近则会去编号小的，现在要开一家新超市，问开在哪里会有最高的人流量
    题解：我们先通过两遍dfs计算出每个城镇和最近的超市的距离d[x]和最小id，
    考虑树分治，对于每个分治层，计算点到重心的距离，
    统计一点过重心的链长是否小于另一点的d值，即dis[x]+dis[y]<d[y]
    我们在获取分治层信息时保存d[y]-dis[y]以及id[y]，
    那么计算对于x的贡献时只要在排序后的分治层信息中二分找到其能吸引的城镇数即可，
    由于计算的时候会计算同分支子树的信息组合，所以要分别计算分支信息并从答案中减去
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 100010;
int d[N], id[N], c[N];
int tot, g[N], nxt[N << 1], v[N << 1], w[N << 1];
void init() {
    memset(g, -1, sizeof(g));
    tot = 0;
}
void add_edge(int x, int y, int z) {
    v[tot] = y, w[tot] = z, nxt[tot] = g[x], g[x] = tot++;
}
void dfs(int x, int fx) {
    if (c[x]) {
        id[x] = x;
        d[x] = 0;
    }
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (v[i] == fx) continue;
        dfs(v[i], x);
        if (d[v[i]] + w[i] < d[x])
            d[x] = d[v[i]] + w[i], id[x] = id[v[i]];
        else if (d[v[i]] + w[i] == d[x])
            id[x] = min(id[x], id[v[i]]);
    }
}
void dfs2(int x, int fx) {
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (v[i] == fx) continue;
        if (d[x] + w[i] < d[v[i]])
            d[v[i]] = d[x] + w[i], id[v[i]] = id[x];
        else if (d[x] + w[i] == d[v[i]])
            id[v[i]] = min(id[x], id[v[i]]);
        dfs2(v[i], x);
    }
}
int sum, root, size[N], dp[N], vis[N], ans[N];
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
void getsize(int x, int fx) {
    size[x] = 1;
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]] && v[i] != fx) {
            getsize(v[i], x);
            size[x] += size[v[i]];
        }
    }
}
int top, dis[N];
struct data {
    int d, id;
} st[N];
bool cmp(data x, data y) { return x.d < y.d || (x.d == y.d && x.id < y.id); }
void getdis(int x, int fx, int l) {
    dis[x] = l;
    st[++top] = data{d[x] - l, id[x]};
    for (int i = g[x]; ~i; i = nxt[i])
        if (!vis[v[i]] && v[i] != fx) getdis(v[i], x, l + w[i]);
}
int BinarySearch(int D, int ID) {
    int l = 1, r = top, ans = 0;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (st[mid].d < D || (st[mid].d == D && st[mid].id <= ID))
            l = mid + 1, ans = mid;
        else
            r = mid - 1;
    }
    return ans;
}
void getans(int x, int fx, int f, int s) {
    ans[x] += f * (s - BinarySearch(dis[x], x));
    for (int i = g[x]; ~i; i = nxt[i])
        if (!vis[v[i]] && v[i] != fx) getans(v[i], x, f, s);
}
void cal(int x, int l, int f) {
    top = 0;
    getdis(x, x, l);
    sort(st + 1, st + top + 1, cmp);
    getans(x, x, f, size[x]);
}
void solve(int x) {
    getsize(x, x);
    cal(x, 0, 1);
    vis[x] = 1;
    for (int i = g[x]; ~i; i = nxt[i]) {
        if (!vis[v[i]]) {
            cal(v[i], w[i], -1);
            root = 0;
            dp[0] = sum = size[v[i]];
            getroot(v[i], 0);
            solve(root);
        }
    }
}
int n;
int main() {
    while (~scanf("%d", &n)) {
        init();
        memset(d, 0x3f, sizeof(d));
        memset(id, 0, sizeof(id));
        for (int i = 1; i < n; i++) {
            int x, y, z;
            scanf("%d%d%d", &x, &y, &z);
            add_edge(x, y, z);
            add_edge(y, x, z);
        }
        for (int i = 1; i <= n; i++) scanf("%d", &c[i]);
        dfs(1, 1);
        dfs2(1, 1);
        memset(ans, 0, sizeof(ans));
        memset(vis, 0, sizeof(vis));
        root = 0;
        dp[0] = sum = n;
        getroot(1, 0);
        solve(root);
        int res = 0;
        for (int i = 1; i <= n; i++)
            if (!c[i]) res = max(res, ans[i]);
        printf("%d\n", res);
    }
    return 0;
}