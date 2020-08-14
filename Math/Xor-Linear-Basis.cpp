/*
    异或线性基
*/
typedef long long LL;
struct L_B {
    LL d[61], p[61];
    int cnt, zero;
    L_B() {
        memset(d, 0, sizeof(d));
        memset(p, 0, sizeof(p));
        cnt = zero = 0;
    }
    void ins(LL val) {
        for (int i = 60; i >= 0; i--)
            if (val & (1LL << i)) {
                if (!d[i]) {
                    d[i] = val;
                    cnt++;
                    return;
                }
                val ^= d[i];
            }
        zero = 1;
    }
    LL query_max(LL x) {
        LL ret = x;
        for (int i = 60; i >= 0; i--)
            if ((ret ^ d[i]) > ret) ret ^= d[i];
        return ret;
    }
    LL query_min() {
        for (int i = 0; i <= 60; i++)
            if (d[i]) return d[i];
        return 0;
    }
    void rebuild() {
        cnt = 0;
        for (int i = 60; i >= 0; i--)
            for (int j = i - 1; j >= 0; j--)
                if (d[i] & (1LL << j)) d[i] ^= d[j];
        for (int i = 0; i <= 60; i++)
            if (d[i]) p[cnt++] = d[i];
    }
    LL kth(LL k) {  //求第k小之前需要rebuild
        int ret = 0;
        if (k >= (1LL << cnt)) return -1;
        for (int i = 60; i >= 0; i--)
            if (k & (1LL << i)) ret ^= p[i];
        return ret;
    }
    int rnk(LL x) {
        int rnk = 0, bit = 0;
        for (int i = 0; i <= 60; i++)
            if (d[i]) {
                if (x >> i & 1) rnk += 1 << bit;
                bit++;
            }
        return rnk;
    }
};
L_B merge(const L_B &n1, const L_B &n2) {
    L_B ret = n1;
    for (int i = 60; i >= 0; i--)
        if (n2.d[i]) ret.ins(n1.d[i]);
    return ret;
}
/*
    Example: 求第k小的子集异或值
*/
int main() {
    int T, n, q;
    LL x;
    scanf("%d", &T);
    for (int cas = 1; cas <= T; cas++) {
        L_B b;
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) {
            scanf("%lld", &x);
            b.ins(x);
        }
        printf("Case #%d:\n", cas);
        b.rebuild();
        scanf("%d", &q);
        while (q--) {
            scanf("%lld", &x);
            if (b.zero)
                x--;  //线性基包含线性相关向量组，能异或得到0，向量个数为2^cnt
            printf("%lld\n", b.kth(x));
        }
    }
    return 0;
}
/*
    Example；给出一张无向边权图，有重边和自环，求一条从1到n的路径，使得异或和最大
    Solution：等价于任意找一条1到n的路径，然后用图中的环去异或这个值，找出最大值，
    因为可以到达环并原路返回消掉。我们将图中所有环的异或值求出来，加入异或线性基，
    用1到n的路径值去线性基中求异或最大值即可
*/
L_B b;
bool vis[N];
void dfs(int x, int fx) {
    vis[x] = 1;
    for (int i = 0; i < v[x].size(); i++) {
        int y = v[x][i];
        LL z = w[x][i];
        if (y != fx) {
            if (!vis[y]) {
                d[y] = d[x] ^ z;
                dfs(y, x);
            } else
                b.ins(d[x] ^ d[y] ^ z);
        }
    }
}
int n, m;
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++) {
        int x, y;
        LL z;
        scanf("%d%d%lld", &x, &y, &z);
        v[x].push_back(y);
        w[x].push_back(z);
        v[y].push_back(x);
        w[y].push_back(z);
    }
    dfs(1, -1);
    printf("%lld\n", b.query_max(d[n]));
    return 0;
}
/*
    Example: 给定n个数，将其所有子集（可以为空）的异或值从小到大排序得到序列B，
    请问某数Q在B中第一次出现的下标是多少？保证Q在B中出现。
    Solution: 通过二进制拆分可以查询非重情况下的rank，
    而子集xor所有值中每种数出现次数都是一样的，可以直接用快速幂处理
*/
int n;
LL x;
LL pow(int a, int b) {
    int t = 1;
    while (b) {
        if (b & 1) t = t * a % P;
        a = a * a % P;
        b >>= 1;
    }
    return t;
}
int main() {
    L_B b;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &x);
        b.ins(x);
    }
    scanf("%lld", &x);
    printf("%d\n", (b.rnk(x) % P * pow(2, n - b.cnt) % P + 1) % P);
    return 0;
}
