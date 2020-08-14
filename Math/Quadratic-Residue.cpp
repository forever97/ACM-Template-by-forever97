/*
    二次剩余
    sqrt(n)%P
    Example
        (x+y)%P=b
        (x*y)%P=c
        求x和y
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const LL P = 1e9 + 7;
const LL inv2 = (P + 1) / 2;
int T;
LL b, c;
LL mul(LL x, LL y, LL P) {
    return (x * y - (LL)(x / (long double)P * y + 1e-3) * P + P) % P;
}
LL pow_mod(LL a, LL b, LL P) {
    LL t = 1;
    for (; b; b >>= 1, a = mul(a, a, P))
        if (b & 1) t = mul(t, a, P);
    return t;
}
// 求sqrt(n)%P
LL ToneLLi_Shanks(LL n, LL p) {
    if (p == 2) return (n & 1) ? 1 : -1;
    if (pow_mod(n, p >> 1, p) + 1 == p) return -1;
    if (p & 2) return pow_mod(n, p + 1 >> 2, p);
    int s = __builtin_ctzll(p ^ 1);
    LL q = p >> s, z = 2;
    for (; pow_mod(z, p >> 1, p) == 1; ++z)
        ;
    LL c = pow_mod(z, q, p);
    LL r = pow_mod(n, q + 1 >> 1, p);
    LL t = pow_mod(n, q, p), tmp;
    for (int m = s, i; t != 1;) {
        for (i = 0, tmp = t; tmp != 1; ++i) tmp = tmp * tmp % p;
        for (; i < --m;) c = c * c % p;
        r = r * c % p;
        c = c * c % p;
        t = t * c % p;
    }
    return r;
}
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%lld%lld", &b, &c);
        LL x = ToneLLi_Shanks(((b * b - 4 * c) % P + P) % P, P);
        if (x == -1)
            puts("-1 -1");
        else {
            LL u1 = (b - x + P) * inv2 % P;
            LL u2 = (b + x) * inv2 % P;
            if (u1 > u2) swap(u1, u2);
            printf("%lld %lld\n", u1, u2);
        }
    }
    return 0;
}
