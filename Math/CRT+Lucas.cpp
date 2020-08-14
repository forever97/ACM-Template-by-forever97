/*
    CRT+Lucas
    计算模为多个素数乘积的组合数
    模以m个素数形式给出
*/
namespace CRT_Lucas {
LL f[N], rf[N];
LL mul(LL x, LL y, LL P) {
    return (x * y - (LL)(x / (long double)P * y + 1e-3) * P + P) % P;
}
LL pow(LL a, LL b, LL P) {
    LL t = 1;
    for (; b; b >>= 1, a = mul(a, a, P))
        if (b & 1) t = mul(t, a, P);
    return t;
}
void Initialize(int n) {
    f[0] = 1;
    for (int i = 1; i < n; i++) f[i] = f[i - 1] * i % n;
    rf[n - 1] = pow(f[n - 1], n - 2, n);
    for (int i = n - 1; i; i--) rf[i - 1] = rf[i] * i % n;
}
LL C(int n, int m, int mod) {
    if (m > n || m < 0 || n < 0) return 0;
    return f[n] * rf[m] % mod * rf[n - m] % mod;
}
LL lucas(LL n, LL m, LL P) {
    if (n < m) return 0;
    if (!m || n == m) return 1;
    return C(n % P, m % P, P) * lucas(n / P, m / P, P) % P;
}
LL exgcd(LL a, LL b, LL& x, LL& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    LL d = exgcd(b, a % b, y, x);
    y -= x * (a / b);
    return d;
}
LL CRT(LL* a, LL* b, int n) {
    LL P = 1, d, y, x = 0;
    for (int i = 0; i < n; i++) P *= a[i];
    for (int i = 0; i < n; i++) {
        LL w = P / a[i];
        exgcd(a[i], w, d, y);
        y = (y % P + P) % P;
        x = (x + mul(mul(y, w, P), b[i], P));
    }
    return (x + P) % P;
}
LL a[K], p[K];
LL Cal(LL n, LL m, int k) {
    for (int i = 0; i < k; i++) {
        scanf("%lld", &p[i]);
        Initialize(p[i]);
        a[i] = lucas(n, m, p[i]);
    }
    return CRT(p, a, k);
}
}  // namespace CRT_Lucas