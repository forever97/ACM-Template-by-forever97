/*
    拉格朗日插值法
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int P = 1e9 + 7;
ll Pow(ll x, ll y) {
    ll ret = 1;
    while (y) {
        if (y & 1) ret = ret * x % P;
        y /= 2;
        x = x * x % P;
    }
    return ret;
}
ll inv(ll x) { return Pow(x, P - 2); }
struct polynomial {
    static const int mxN = 1000005;
    ll f[mxN];
    ll fac[mxN];
    ll ul[mxN];
    ll ur[mxN];
    int n;
    //丢进n个数产生最高次为n-1的多项式
    //丢进的必须是f(0)到f(n-1)
    void init(const vector<ll> &V) {
        n = V.size();
        fac[0] = 1;
        for (int i = 1; i < n; i++) fac[i] = fac[i - 1] * i % P;
        for (int i = 0; i < n; i++) f[i] = V[i];
    }
    //获取f(x)
    ll get(ll x) {
        ul[0] = 1;
        for (int i = 1; i < n; i++) ul[i] = ul[i - 1] * (x - (i - 1)) % P;
        ur[n - 1] = 1;
        for (int i = n - 2; i >= 0; i--) ur[i] = ur[i + 1] * (x - (i + 1)) % P;
        ll ret = 0;
        for (int i = 0; i < n; i++) {
            ll tmp1 = ul[i] * ur[i] % P;
            ll tmp2 = inv(fac[i] * fac[n - 1 - i] % P);
            ll tmp3 = f[i];
            if ((n - 1 - i) & 1) tmp3 *= -1;
            ret = (ret + tmp1 * tmp2 % P * tmp3 % P) % P;
        }
        return (ret + P) % P;
    }
} lgr;
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    vector<ll> f(k + 2);
    f[0] = 0;
    for (int i = 1; i < k + 2; i++) f[i] = (f[i - 1] + Pow(i, k)) % P;
    lgr.init(f);
    printf("%lld\n", lgr.get(n));
}
/*
    拉格朗日插值法
    1^k+2^k+……+n^k
*/
typedef long long LL;
const int MOD = 1e9 + 7;
const int MX = 1e6 + 10;
struct Lagrange {
    static const int M = 1e6 + 10;
    short factor[M];
    int P[M], S[M], ar[M], inv[M];
    inline LL power(LL a, LL b) {
        LL res = 1;
        while (b) {
            if (b & 1) res = res * a % MOD;
            a = a * a % MOD;
            b >>= 1;
        }
        return res;
    }
    int lagrange(LL n, int k) {
        if (!k) return n % MOD;
        int i, j, x, res = 0;
        if (!inv[0]) {
            for (i = 2, x = 1; i < MX; i++) x = (long long)x * i % MOD;
            inv[MX - 1] = power(x, MOD - 2);
            for (i = MX - 2; i >= 0; i--)
                inv[i] = ((long long)inv[i + 1] * (i + 1)) % MOD;
        }
        k++;
        for (i = 0; i <= k; i++) factor[i] = 0;
        for (i = 4; i <= k; i += 2) factor[i] = 2;
        for (i = 3; i * i <= k; i += 2) {
            if (!factor[i]) {
                for (j = (i * i), x = i << 1; j <= k; j += x) factor[j] = i;
            }
        }
        for (ar[1] = 1, ar[0] = 0, i = 2; i <= k; i++) {
            if (!factor[i])
                ar[i] = power(i, k - 1);
            else
                ar[i] = ((LL)ar[factor[i]] * ar[i / factor[i]]) % MOD;
        }
        for (i = 1; i <= k; i++) {
            ar[i] += ar[i - 1];
            if (ar[i] >= MOD) ar[i] -= MOD;
        }
        if (n <= k) return ar[n];
        P[0] = 1, S[k] = 1;
        for (i = 1; i <= k; i++)
            P[i] = ((LL)P[i - 1] * ((n - i + 1) % MOD)) % MOD;
        for (i = k - 1; i >= 0; i--)
            S[i] = ((LL)S[i + 1] * ((n - i - 1) % MOD)) % MOD;
        for (i = 0; i <= k; i++) {
            x = (LL)ar[i] * P[i] % MOD * S[i] % MOD * inv[k - i] % MOD *
                inv[i] % MOD;
            if ((k - i) & 1) {
                res -= x;
                if (res < 0) res += MOD;
            } else {
                res += x;
                if (res >= MOD) res -= MOD;
            }
        }
        return res % MOD;
    }
} lgr;
