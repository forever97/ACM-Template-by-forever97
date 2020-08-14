/*
    莫比乌斯反演
    f(n)=∑[n|d]g(d)
    g(n)=∑[n|d]mu[d/n]f(d)
    ----------------------
    f(n)=∑[d|n]g(d)
    g(n)=∑[d|n]mu[n/d]f(d)=∑[d|n]mu[d]f(n/d)
*/
/*
    Example1:
        ∑(i,a,b)∑(j,c,d)[gcd(i,j)==k]
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N = 100010;
bool notp[N];
int prime[N], pnum, mu[N];
void sieve() {
    memset(notp, 0, sizeof(notp));
    notp[0] = notp[1] = mu[1] = 1;
    for (int i = 2; i < N; i++) {
        if (!notp[i]) {
            prime[++pnum] = i;
            mu[i] = -1;
        }
        for (int j = 1; j <= pnum && prime[j] * i < N; j++) {
            notp[prime[j] * i] = 1;
            if (i % prime[j] == 0) {
                mu[prime[j] * i] = 0;
                break;
            }
            mu[prime[j] * i] = -mu[i];
        }
    }
}
ll f[N];
ll solve(int n, int m) {
    if (n > m) swap(n, m);
    ll res = 0;
    for (int i = 1, last; i <= n; i = last + 1) {
        last = min(n / (n / i), m / (m / i));
        res += (f[last] - f[i - 1]) * (n / i) * (m / i);
    }
    return res;
}
int T, a, b, c, d, k;
int main() {
    sieve();
    for (int i = 1; i < N; i++) f[i] = f[i - 1] + mu[i];
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d%d%d%d", &a, &b, &c, &d, &k);
        printf("%lld\n", solve(b / k, d / k) - solve(b / k, (c - 1) / k) -
                             solve((a - 1) / k, d / k) +
                             solve((a - 1) / k, (c - 1) / k));
    }
    return 0;
}
/*
    Example2：
        ∑∑[gcd(i,j)素因子个数<=P] 
*/
#include <bits/stdc++.h>
using namespace std;
using namespace std;
typedef long long ll;
const int N = 500010;
bool notp[N];
int prime[N], pnum, mu[N], d[N];
void sieve() {
    memset(notp, 0, sizeof(notp));
    notp[0] = notp[1] = mu[1] = 1;
    for (int i = 2; i < N; i++) {
        if (!notp[i]) {
            prime[++pnum] = i;
            mu[i] = -1;
            d[i] = 1;
        }
        for (int j = 1; j <= pnum && prime[j] * i < N; j++) {
            notp[prime[j] * i] = 1;
            d[prime[j] * i] = d[i] + 1;
            if (i % prime[j] == 0) {
                mu[prime[j] * i] = 0;
                break;
            }
            mu[prime[j] * i] = -mu[i];
        }
    }
}
int f[N][21];
void calF() {
    for (int i = 1; i < N; i++)
        for (int j = i; j < N; j += i) f[j][d[i]] += mu[j / i];
    for (int i = 1; i < N; i++)
        for (int j = 1; j <= 20; j++) f[i][j] += f[i][j - 1];
    for (int i = 1; i < N; i++)
        for (int j = 0; j <= 20; j++) f[i][j] += f[i - 1][j];
}
ll solve(int n, int m, int p) {
    if (n > m) swap(n, m);
    ll res = 0;
    for (int i = 1, last; i <= n; i = last + 1) {
        last = min(n / (n / i), m / (m / i));
        res += (ll)(f[last][p] - f[i - 1][p]) * (n / i) * (m / i);
    }
    return res;
}
int T, n, m, p;
int main() {
    sieve();
    calF();
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d%d", &n, &m, &p);
        p = min(p, 20);
        printf("%lld\n", solve(n, m, p));
    }
    return 0;
}
