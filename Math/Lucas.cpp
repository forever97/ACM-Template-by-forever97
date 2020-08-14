/*
    Lucas定理
    lucas(n,m,P)返回C(n,m)对P取摸的结果
*/
namespace Lucas {
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
}  // namespace Lucas