/*
    BIT
    区间修改区间查询
    仅限可加类数据
*/
struct BIT {
    int n, s[N], a[N];
    ll b[N];
    void init(int x) {
        n = x;
        for (int i = 1; i <= n; i++) a[i] = b[i] = s[i] = 0;
    }
    void modify(int x, int p) {
        for (int i = x; i <= n; i += i & -i) a[i] += p, b[i] += p * s[x - 1];
    }
    ll ask(int x) {
        int t0 = 0;
        ll t1 = 0;
        for (int i = x; i; i -= i & -i) t0 += a[i], t1 += b[i];
        return 1LL * s[x] * t0 - t1;
    }
} T;
int n, m, op, x, y;
int main() {
    scanf("%d", &n);
    init(n);
    for (int i = 1; i <= n; i++) scanf("%d", &T.s[i]), T.s[i] += T.s[i - 1];
    scanf("%d", &m);
    while (m--) {
        scanf("%d%d%d", &op, &x, &y);
        if (op == 1)
            T.modify(x, 1), T.modify(y + 1, -1);
        else
            printf("%lld\n", T.ask(y) - T.ask(x - 1));
    }
    return 0;
}