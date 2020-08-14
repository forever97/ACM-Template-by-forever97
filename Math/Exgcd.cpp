/*
    扩展欧几里得定理
*/
typedef long long ll;
ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (!b) return x = 1, y = 0, a;
    ll d = exgcd(b, a % b, x, y), t = x;
    return x = y, y = t - a / b * y, d;
}
ll A, B, C, k;
int main() {
    while (scanf("%lld%lld%lld%lld", &A, &B, &C, &k), A + B + C + k) {
        ll a = C, b = B - A, n = 1LL << k, x, y, d = exgcd(a, n, x, y);
        if (b % d)
            puts("FOREVER");
        else {
            x = (x * (b / d)) % n;  //方程ax=b(mod n)的最小解
            x = (x % (n / d) + n / d) % (n / d);  //方程ax=b(mod n)的最小整数解
            printf("%I64d\n", x);
        }
    }
    return 0;
}