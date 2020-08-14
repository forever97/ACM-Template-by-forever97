/*
    Problem:
        已知x_0，x_1
        x_i=a*x_{i-1}+b*x_{i-2}
        求x_n，n<=10^(10^6)
    Solution:
        广义斐波那契数列存在模数循环节
        为(p+1)*(p-1)*p^(c-1)的乘积，其中p为模数素因子
        将n对循环节取模，然后特征根加速递推即可    
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int M = 3, N = 1000000 + 10;
ll P, m, i, j, x, w, b, t, a[M], c[M], v[M], u[M << 1], ans;
char s[N];
bool notp[N];
int prime[N], pnum;
void sieve() {
    memset(notp, 0, sizeof(notp));
    notp[0] = notp[1] = 1;
    pnum = 0;
    for (int i = 2; i < N; i++) {
        if (!notp[i]) prime[++pnum] = i;
        for (int j = 1; j <= pnum && prime[j] * i < N; j++) {
            notp[prime[j] * i] = 1;
            if (i % prime[j] == 0) break;
        }
    }
}
ll getmod(ll x) {
    ll ans1 = 1, ans2 = 1, y = x;
    for (int i = 1; i <= pnum; i++) {
        if (1ll * prime[i] * prime[i] > x) break;
        if (x % prime[i] == 0) {
            ans1 *= (prime[i] - 1) * (prime[i] + 1);
            ans2 *= prime[i];
            while (x % prime[i] == 0) x /= prime[i];
        }
    }
    if (x > 1) {
        ans1 *= (x - 1) * (x + 1);
        ans2 *= x;
    }
    return y / ans2 * ans1;
}
ll mul(ll x, ll y, ll P) {
    ll tmp = (x * y - (ll)((long double)x / P * y + 1.0e-8) * P);
    return tmp < 0 ? tmp + P : tmp;
}
int main() {
    sieve();
    m = 2;
    for (i = 0; i < m; i++) scanf("%lld", &a[i]);
    for (i = m - 1; ~i; i--) scanf("%lld", &c[i]);
    for (i = 0; i < m; i++) v[i] = 1;
    scanf("%s", s + 1);
    scanf("%lld", &P);
    for (i = 0; i < m; i++) a[i] = a[i] % P;
    for (i = 0; i < m; i++) c[i] = c[i] % P;
    ll mod = getmod(P);
    int len = strlen(s + 1);
    ll n = 0;
    for (int i = 1; i <= len; i++) {
        n = mul(n, 10, mod);
        n = (n + s[i] - '0') % mod;
    }
    for (w = !!n, i = n; i > 1; i >>= 1) w <<= 1;
    for (x = 0; w; copy(u, u + m, v), w >>= 1, x <<= 1) {
        fill_n(u, m << 1, 0), b = !!(n & w), x |= b;
        if (x < m)
            u[x] = 1;
        else {
            for (i = 0; i < m; i++)
                for (j = 0, t = i + b; j < m; j++, t++)
                    u[t] = ((ll)v[i] * v[j] + u[t]) % P;
            for (i = (m << 1) - 1; i >= m; i--)
                for (j = 0, t = i - m; j < m; j++, t++)
                    u[t] = ((ll)c[j] * u[i] + u[t]) % P;
        }
    }
    for (i = 0; i < m; i++) ans = ((ll)v[i] * a[i] + ans) % P;
    printf("%lld\n", (ans + P) % P);
    return 0;
}
