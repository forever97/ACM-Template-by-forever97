/*
    线段树-标记持久化
    题目大意：给出一棵树，要求维护两个操作：
    1.给一个树的节点加上x，他的子节点加上x-k，他的孙子节点加上x-2k，按层递推
    2.查询一个点的当前权值
    题解：对于一个节点来说，一次修改对节点u的影响为x+k*(d[changed]-d[u])
    x+k*d[changed]是一个子树修改值，通过维护dfs序可以转化为区间修改值，
    对于-k*d[u]来说，是一个查询时可以单独计算的值，
    我们维护x+k*d[changed]的区间加法，和k的区间加法，
    然后单点查询即可，因为只有区间可加操作和单点查询，因此我们可以对标记进行持久化
*/
const int N = 3000010;
const int mod = 1e9 + 7;
int T[N], K[N], st[N], en[N], dfn, d[N];
vector<int> v[N];
void dfs(int x, int fx) {
    st[x] = ++dfn;
    d[x] = d[fx] + 1;
    for (int i = 0; i < (int)v[x].size(); i++) dfs(v[x][i], x);
    en[x] = dfn;
}
void modify(int x, int l, int r, int L, int R, int root, int p, int k) {
    if (L <= l && r <= R) {
        K[x] = (K[x] + k) % mod;
        T[x] = ((T[x] + p) % mod + 1LL * d[root] * k % mod) % mod;
        return;
    }
    int mid = (l + r) >> 1;
    if (L <= mid) modify(x << 1, l, mid, L, R, root, p, k);
    if (mid < R) modify(x << 1 | 1, mid + 1, r, L, R, root, p, k);
}
int query(int x, int l, int r, int pos, int p) {
    if (l == r) return (T[x] - 1LL * K[x] * d[p] % mod + mod) % mod;
    int mid = (l + r) >> 1;
    if (pos <= mid)
        return ((query(x << 1, l, mid, pos, p) + T[x]) % mod -
                1LL * K[x] * d[p] % mod + mod) %
               mod;
    else
        return ((query(x << 1 | 1, mid + 1, r, pos, p) + T[x]) % mod -
                1LL * K[x] * d[p] % mod + mod) %
               mod;
}
int cas, n, q, op, u, x, k;
int main() {
    scanf("%d", &cas);
    while (cas--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) v[i].clear();
        memset(d, 0, sizeof(d));
        memset(T, 0, sizeof(T));
        memset(K, 0, sizeof(K));
        for (int i = 2; i <= n; i++) {
            scanf("%d", &x);
            v[x].push_back(i);
        }
        dfn = 0;
        dfs(1, 0);
        scanf("%d", &q);
        while (q--) {
            scanf("%d", &op);
            if (op == 1) {
                scanf("%d%d%d", &u, &x, &k);
                modify(1, 1, n, st[u], en[u], u, x, k);
            } else {
                scanf("%d", &u);
                printf("%d\n", query(1, 1, n, st[u], u));
            }
        }
    }
    return 0;
}
