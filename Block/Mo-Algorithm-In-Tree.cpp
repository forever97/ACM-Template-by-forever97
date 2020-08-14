/*
    树上莫队，注意查询时额外考虑lca位置
    查询树链上不同权值的数量
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
const int N = 100010;
const int K = 17;
using namespace std;
struct P {
    int l, r, z, id;
} Q[N];
int lim, pos[N << 1], l, r, c[N], g[N], v[N << 1], nxt[N << 1], ed;
int n, m, x, y, z, loc[N << 1], dfn, st[N], en[N], d[N], f[N][18];
int ans[N], cnt[N], sum;
bool vis[N];
bool cmp(const P& a, const P& b) {
    return pos[a.l] == pos[b.l] ? a.r < b.r : pos[a.l] < pos[b.l];
}
void add(int x, int y) {
    v[++ed] = y;
    nxt[ed] = g[x];
    g[x] = ed;
}
void dfs(int x) {
    for (int i = vis[loc[st[x] = ++dfn] = x] = 1; i <= K; i++)
        f[x][i] = f[f[x][i - 1]][i - 1];
    for (int i = g[x]; i; i = nxt[i])
        if (!vis[v[i]]) d[v[i]] = d[f[v[i]][0] = x] + 1, dfs(v[i]);
    loc[en[x] = ++dfn] = x;
}
int lca(int x, int y) {
    if (x == y) return x;
    if (d[x] < d[y]) swap(x, y);
    for (int i = K; ~i; i--)
        if (d[f[x][i]] >= d[y]) x = f[x][i];
    if (x == y) return x;
    for (int i = K; ~i; i--)
        if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
    return f[x][0];
}
void deal(int x) {
    if (!vis[x]) {
        if (!(--cnt[c[x]])) sum--;
    } else if (!(cnt[c[x]]++))
        sum++;
    vis[x] ^= 1;
}
void read(int& x) {
    int f = 1;
    x = 0;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if ('-' == ch) f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = (x << 3) + (x << 1) + ch - '0';
        ch = getchar();
    }
}
int b[N];
int main() {
    read(n), read(m);
    for (int i = 1; i <= n; i++) read(c[i]), b[i] = c[i];
    sort(b + 1, b + n + 1);
    int siz = unique(b + 1, b + n + 1) - (b + 1);
    for (int i = 1; i <= n; i++)
        c[i] = lower_bound(b + 1, b + siz + 1, c[i]) - b;
    for (int i = 1; i < n; i++) read(x), read(y), add(x, y), add(y, x);
    dfs(d[1] = 1), lim = (int)sqrt(n * 2 + 0.5);
    for (int i = 1; i <= dfn; i++) pos[i] = (i - 1) / lim + 1;
    for (int i = 1; i <= m; i++) {
        read(x), read(y);
        Q[i].id = i;
        if (st[x] > st[y]) swap(x, y);
        z = lca(x, y);
        if (z == x)
            Q[i].l = st[x], Q[i].r = st[y];
        else
            Q[i].l = en[x], Q[i].r = st[y], Q[i].z = z;
    }
    sort(Q + 1, Q + m + 1, cmp);
    for (int i = 1, l = 1, r = 0; i <= m; i++) {
        if (r < Q[i].r) {
            for (r++; r <= Q[i].r; r++) deal(loc[r]);
            r--;
        }
        if (r > Q[i].r)
            for (; r > Q[i].r; r--) deal(loc[r]);
        if (l < Q[i].l)
            for (; l < Q[i].l; l++) deal(loc[l]);
        if (l > Q[i].l) {
            for (l--; l >= Q[i].l; l--) deal(loc[l]);
            l++;
        }
        if (Q[i].z) deal(Q[i].z);
        ans[Q[i].id] = sum;
        if (Q[i].z) deal(Q[i].z);
    }
    for (int i = 1; i <= m; i++) printf("%d\n", ans[i]);
    return 0;
}