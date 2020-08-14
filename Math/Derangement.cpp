/*
    错位排列
    求长度为n序列m位恰好在原来位置的方案数
*/
typedef long long LL;
const int N = 1000010;
const LL mod = 1e9 + 7;
LL fac[N], inv[N], f[N];
int n, m, T;
int main() {
    fac[0] = inv[0] = inv[1] = 1;
    for (int i = 1; i < N; i++) fac[i] = fac[i - 1] * i % mod;
    for (int i = 2; i < N; i++) inv[i] = (mod - mod / i) * inv[mod % i] % mod;
    for (int i = 2; i < N; i++) inv[i] = inv[i - 1] * inv[i] % mod;
    f[f[0] = f[2] = 1] = 0;
    for (int i = 3; i < N; i++) f[i] = (f[i - 1] + f[i - 2]) * (i - 1) % mod;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        printf("%lld\n",
               fac[n] * inv[m] % mod * inv[n - m] % mod * f[n - m] % mod);
    }
    return 0;
}