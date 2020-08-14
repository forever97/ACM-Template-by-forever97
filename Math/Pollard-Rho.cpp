/*
    Pollard_Rho
    题目大意：
        给出最大公约数和最小公倍数，满足要求的x和y，且x+y最小
    题解：
        我们发现，(x/gcd)*(y/gcd)=lcm/gcd，并且x/gcd和y/gcd互质
        那么我们先利用把所有的质数求出来Pollard_Rho，将相同的质数合并
        现在的问题转变成把合并后的质数分为两堆，使得x+y最小
        我们考虑不等式a+b>=2sqrt(ab),在a趋向于sqrt(ab)的时候a+b越小
        所以我们通过搜索求出最逼近sqrt(ab)的值即可。
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
#define C 2730
#define S 3
using namespace std;
typedef long long ll;
ll n, m, s[1000], cnt, f[1000], cnf, ans;
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
ll mul(ll a, ll b, ll n) {
    return (a * b - (ll)(a / (long double)n * b + 1e-3) * n + n) % n;
}
ll pow(ll a, ll b, ll n) {
    ll d = 1;
    a %= n;
    while (b) {
        if (b & 1) d = mul(d, a, n);
        a = mul(a, a, n);
        b >>= 1;
    }
    return d;
}
bool check(ll a, ll n) {
    ll m = n - 1, x, y;
    int i, j = 0;
    while (!(m & 1)) m >>= 1, j++;
    x = pow(a, m, n);
    for (i = 1; i <= j; x = y, i++) {
        y = pow(x, 2, n);
        if ((y == 1) && (x != 1) && (x != n - 1)) return 1;
    }
    return y != 1;
}
bool miller_rabin(int times, ll n) {
    ll a;
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (!(n & 1)) return 0;
    while (times--)
        if (check(rand() % (n - 1) + 1, n)) return 0;
    return 1;
}
ll pollard_rho(ll n, int c) {
    ll i = 1, k = 2, x = rand() % n, y = x, d;
    while (1) {
        i++, x = (mul(x, x, n) + c) % n, d = gcd(y - x, n);
        if (d > 1 && d < n) return d;
        if (y == x) return n;
        if (i == k) y = x, k <<= 1;
    }
}
void findfac(ll n, int c) {
    if (n == 1) return;
    if (miller_rabin(S, n)) {
        s[cnt++] = n;
        return;
    }
    ll m = n;
    while (m == n) m = pollard_rho(n, c--);
    findfac(m, c), findfac(n / m, c);
}
void dfs(int pos, long long x, long long k) {
    if (pos > cnf) return;
    if (x > ans && x <= k) ans = x;
    dfs(pos + 1, x, k);
    x *= f[pos];
    if (x > ans && x <= k) ans = x;
    dfs(pos + 1, x, k);
}
int main() {
    while (~scanf("%lld%lld", &m, &n)) {
        if (n == m) {
            printf("%lld %lld\n", n, n);
            continue;
        }
        cnt = 0;
        long long k = n / m;
        findfac(k, C);
        sort(s, s + cnt);
        f[0] = s[0];
        cnf = 0;
        for (int i = 1; i < cnt; i++) {
            if (s[i] == s[i - 1])
                f[cnf] *= s[i];
            else
                f[++cnf] = s[i];
        }
        long long tmp = (long long)sqrt(1.0 * k);
        ans = 1;
        dfs(0, 1, tmp);
        printf("%lld %lld\n", m * ans, k / ans * m);
    }
    return 0;
}
