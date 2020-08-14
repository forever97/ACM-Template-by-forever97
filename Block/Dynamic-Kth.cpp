/*
    区间修改区间第k大
    二分答案+分块
    Example：
        给出一棵树，可以增加一条边的边长（询问给出一个点，修改的边为其和父节点之间的边）
        或者查询一个子树中深度第k小的点
*/
typedef pair<int, int> P;
const int N = 100010, K = 12, M = (N >> K) + 5;
inline char nc() {
    static char buf[100000], *p1 = buf, *p2 = buf;
    if (p1 == p2) {
        p2 = (p1 = buf) + fread(buf, 1, 100000, stdin);
        if (p1 == p2) return EOF;
    }
    return *p1++;
}
inline void read(int &x) {
    char c = nc(), b = 1;
    for (; !(c >= '0' && c <= '9'); c = nc())
        if (c == '-') b = -1;
    for (x = 0; c >= '0' && c <= '9'; x = x * 10 + c - '0', c = nc())
        ;
    x *= b;
}
int g[N], w[N], nxt[N], stq[N], enq[N], dfn;
P a[N << 1];
void add(int x, int y, int z) {
    w[y] = z;
    nxt[y] = g[x];
    g[x] = y;
}
void dfs(int x, int y) {
    stq[x] = ++dfn;
    a[dfn] = P(y, dfn);
    for (int i = g[x]; i; i = nxt[i]) dfs(i, y + w[i]);
    enq[x] = dfn;
}
int n, m, lim, op, x, y;
int block, st[M], en[M], tag[M];
inline void change(int o, int l, int r, int p) {
    static P A[N], B[N];
    int ca = 0, cb = 0, i, j, k = st[o];
    for (i = st[o]; i <= en[o]; i++) {
        if (a[i].second < l || a[i].second > r)
            A[++ca] = a[i];
        else
            B[++cb] = a[i], B[cb].first += p;
    }
    i = j = 1;
    while (i <= ca && j <= cb) a[k++] = A[i] < B[j] ? A[i++] : B[j++];
    while (i <= ca) a[k++] = A[i++];
    while (j <= cb) a[k++] = B[j++];
}
inline void modify(int x, int y, int p) {
    int X = x >> K, Y = y >> K, i;
    for (i = X + 1; i < Y; i++) tag[i] += p;
    change(X, x, y, p);
    if (X < Y) change(Y, x, y, p);
}
inline int ask(int o, int p) {
    int l = st[o], r = en[o], mid, t = l - 1;
    if (l > r) return 0;
    p -= tag[o];
    while (l <= r) {
        if (a[mid = (l + r) >> 1].first <= p)
            l = (t = mid) + 1;
        else
            r = mid - 1;
    }
    return t - st[o] + 1;
}
inline int kth(int x, int y, int k) {
    if (k > y - x + 1) return -1;
    int X = x >> K, Y = y >> K, i, s = n + 1, e = n;
    tag[block + 1] = tag[X];
    for (i = st[X]; i <= en[X]; i++)
        if (a[i].second >= x && a[i].second <= y) a[++e] = a[i];
    st[block + 1] = s, en[block + 1] = e;
    s = e + 1;
    if (X < Y) {
        tag[block + 2] = tag[Y];
        for (i = st[Y]; i <= en[Y]; i++)
            if (a[i].second <= y) a[++e] = a[i];
    }
    st[block + 2] = s, en[block + 2] = e;
    int l = 0, r = lim, mid, t, ans;
    while (l <= r) {
        mid = (l + r) >> 1;
        t = ask(block + 1, mid) + ask(block + 2, mid);
        for (i = X + 1; i < Y && t < k; i++) t += ask(i, mid);
        if (t >= k)
            r = (ans = mid) - 1;
        else
            l = mid + 1;
    }
    return ans;
}
int main() {
    read(n), read(m), read(x);
    for (int i = 2; i <= n; i++) read(x), read(y), add(x, i, y), lim += y;
    dfs(1, 0);
    block = n >> K;
    for (int i = 1; i <= n; i++) en[i >> K] = i;
    for (int i = n; i; i--) st[i >> K] = i;
    for (int i = 0; i <= block; i++) sort(a + st[i], a + en[i] + 1);
    while (m--) {
        read(op), read(x), read(y);
        if (op == 1)
            printf("%d\n", kth(stq[x], enq[x], y));
        else
            modify(stq[x], enq[x], y), lim += y;
    }
    return 0;
}
