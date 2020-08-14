/*
    题目大意：给定一棵N个点的树，1号点为根，每个节点是白色或者黑色。
    双方轮流操作，每次选择一个白色节点，将从这个点到根的路径上的点全部染成黑色。
    无法操作者为败方。
    问先手是否必胜，以及第一步可选节点有哪些。
    题解：首先是博弈方面的分析。
    令SG[x]为，只考虑以x为根的子树时的SG值。
    令g[x]为，只考虑以x为根的子树时，所有后继局面的SG值的集合。那么SG[x]=mex{g[x]}。
    我们考虑怎么计算g[x]。假设x的儿子为v1,v2,...,vk，
    令sum[x]=SG[v1] xor SG[v2] xor .. xor SG[vk]。考虑两种情况：
        1、x为黑色。不难发现以x的每个儿子为根的子树是互相独立的。
        假设这一步选择了vi子树的某一个节点，
        那么转移到的局面的SG值就是sum[x] xor SG[vi] xor (在g[vi]中的某个值)。
        那么我们只需将每个g[vi]整体xor上sum[x] xor SG[vi]再合并到g[x]即可。
        2、x为白色。这时候我们多了一种选择，即选择x点。
        可以发现，选择x点之后x点变成黑色，所有子树仍然独立，
        而转移到的局面的SG值就是sum[x]。
        如果此时不选择x而是选择x子树里的某个白色节点，那么x一样会被染成黑色，
        所有子树依然独立。所以x为白色时只是要向g[x]中多插入一个值sum[x]。
    这样我们就有一个自底向上的DP了。朴素的复杂度是O(N^2)的。
    接下来再考虑第一步可选的节点。我们要考虑选择哪些节点之后整个局面的SG值会变成0。
    假设我们选择了x点，那么从x到根的路径都会被染黑，将原来的树分成了一堆森林。
    我们令up[x]为，不考虑以x为根的子树，将从x到根的路径染黑，剩下的子树的SG值的xor和。
    那么up[x]=up[fa[x]] xor sum[fa[x]] xor sg[x]，其中fa[x]为x的父亲节点编号。
    那么如果点x初始颜色为白色且up[x] xor
   sum[x]=0，那么这个点就是第一步可选的节点。
    剩下的就是优化求SG了。我们需要一个可以快速整体xor并合并的数据结构。
    整体xor可以用二进制Trie打标记实现，合并类似线段树合并。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long ll;
const int N = 100010, LOG = 17;
int tot, g[N], nxt[N << 1], v[N << 1];
void init() {
    memset(g, -1, sizeof(g));
    tot = 0;
}
void add_edge(int x, int y) { v[tot] = y, nxt[tot] = g[x], g[x] = tot++; }
struct Node {
    int l, r, tag;
    bool full;
} T[20 * N];
int root[N], tcnt = 0;
int n, col[N], sg[N], sum[N], up[N];
bool vis[N];
inline void update(int x) { T[x].full = T[T[x].l].full && T[T[x].r].full; }
inline void pb(int x) {
    if (T[x].tag) {
        if (T[x].l) T[T[x].l].tag ^= T[x].tag >> 1;
        if (T[x].r) T[T[x].r].tag ^= T[x].tag >> 1;
        if (T[x].tag & 1) swap(T[x].l, T[x].r);
        T[x].tag = 0;
    }
}
int merge(int a, int b) {
    if (!a || T[b].full) return b;
    if (!b || T[a].full) return a;
    pb(a), pb(b);
    int x = ++tcnt;
    T[x].l = merge(T[a].l, T[b].l);
    T[x].r = merge(T[a].r, T[b].r);
    update(x);
    return x;
}
inline int rev(int x) {
    int r = 0;
    for (int i = LOG; i; i--)
        if (x >> i - 1 & 1) r += 1 << LOG - i;
    return r;
}
void insert(int x, int v, int p) {
    pb(x);
    if (v >> p - 1 & 1) {
        if (!T[x].r) T[x].r = ++tcnt;
        if (p != 1)
            insert(T[x].r, v, p - 1);
        else
            T[T[x].r].full = 1;
    } else {
        if (!T[x].l) T[x].l = ++tcnt;
        if (p != 1)
            insert(T[x].l, v, p - 1);
        else
            T[T[x].l].full = 1;
    }
    update(x);
}
inline int mex(int x) {
    int r = 0;
    for (int i = LOG; x; i--) {
        pb(x);
        if (T[T[x].l].full)
            r += 1 << i - 1, x = T[x].r;
        else
            x = T[x].l;
    }
    return r;
}
void calc(int x) {
    vis[x] = 1;
    int xorsum = 0;
    for (int i = g[x]; ~i; i = nxt[i])
        if (!vis[v[i]]) {
            calc(v[i]);
            vis[v[i]] = 0;
            xorsum ^= sg[v[i]];
        }
    for (int i = g[x]; ~i; i = nxt[i])
        if (!vis[v[i]]) {
            T[root[v[i]]].tag ^= rev(xorsum ^ sg[v[i]]);
            root[x] = merge(root[x], root[v[i]]);
        }
    if (!col[x]) insert(root[x], xorsum, LOG);
    sg[x] = mex(root[x]);
    sum[x] = xorsum;
}
int ans[N], cnt = 0;
void find(int x) {
    vis[x] = 1;
    if ((up[x] ^ sum[x]) == 0 && col[x] == 0) ans[++cnt] = x;
    for (int i = g[x]; ~i; i = nxt[i])
        if (!vis[v[i]]) {
            up[v[i]] = up[x] ^ sum[x] ^ sg[v[i]];
            find(v[i]);
        }
}
int main() {
    scanf("%d", &n);
    init();
    for (int i = 1; i <= n; i++) scanf("%d", col + i);
    for (int i = 1; i < n; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        add_edge(x, y), add_edge(y, x);
    }
    for (int i = 1; i <= n; i++) root[i] = ++tcnt;
    calc(1);
    for (int i = 1; i <= n; i++) vis[i] = 0;
    find(1);
    if (!cnt)
        printf("-1\n");
    else {
        sort(ans + 1, ans + cnt + 1);
        for (int i = 1; i <= cnt; i++) printf("%d\n", ans[i]);
    }
    return 0;
}
