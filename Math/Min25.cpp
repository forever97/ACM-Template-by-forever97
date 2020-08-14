/*
    Min25筛
    S(x,y): ∑F(i)[i<=x&&i的最小质因子不小于P_y]
    pi>n时S(n,i)=0
    pi<=n时S(n,i)=\sum_{j>=i}^{(p_j)^2<=n}\sum_{c=1}^{(p_j)^{c+1}}S(n/p_i,j+1)*F(p_i^c)+F(p_i^{c+1})
    +\sum_{x是质数且x<=n}F(x)-\sum_{j=1}^{i-1}F(p_i)
    F(i)的前缀和答案为S(n,1)+F(1)
*/
/*
    Example1:
        欧拉函数前缀和&&莫比乌斯函数前缀和
*/
#include <bits/stdc++.h>
typedef long long ll;
const int N = 500000 << 1; // sqrt(n)*2
int n, m, Sqr, cnt, P[N >> 2], h[N], w[N];
ll sp[N], g[N];
inline int ID(int x) { return x <= Sqr ? x : m - n / x + 1; }
// g:前缀质数和 
// h:前缀质数个数
void Init(int n) {
    Sqr = sqrt(n);
    cnt = m = 0;
    for (ll i = 1; i <= n; i = w[m] + 1ll)
        w[++m] = n / (n / i), g[m] = (1ll * w[m] * ((ll)w[m] + 1) >> 1) - 1,
        h[m] = w[m] - 1; 
    for (int i = 2; i <= Sqr; ++i)
        if (h[i] != h[i - 1]) {
            P[++cnt] = i, sp[cnt] = sp[cnt - 1] + i;
            int lim = i * i;
            for (int j = m; lim <= w[j]; --j) {
                int k = ID(w[j] / i);
                g[j] -= 1ll * i * (g[k] - sp[cnt - 1]);  
                h[j] -= h[k] - cnt + 1;
            }
        }
}
// 欧拉函数前缀和
ll S_Phi(int x, int y) {
    if (x <= 1 || P[y] > x) return 0;
    ll res = g[ID(x)] - h[ID(x)] - sp[y - 1] + y - 1; // g-h
    for (int i = y; i <= cnt && P[i] * P[i] <= x; ++i)
        for (ll p = P[i], p1 = p, t = p - 1; 1ll * p1 * p <= x; p1 *= p, t *= p)
            res += 1ll * (S_Phi(x / p1, i + 1) + p) * t;
    return res;
}
// 莫比乌斯函数前缀和
int S_Mu(int x, int y) {
    if (x <= 1 || P[y] > x) return 0;
    int res = -h[ID(x)] + y - 1; // h
    for (int i = y; i <= cnt && P[i] * P[i] <= x; ++i)
        res -= S_Mu(x / P[i], i + 1);
    return res;
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        if (!n)
            puts("0 0");
        else {
            Init(n);
            printf("%lld %d\n", S_Phi(n, 1) + 1, S_Mu(n, 1) + 1);
        }
    }
    return 0;
}
/*
    Example2:
        F(1)=1
        F(p^c)=p xor c(p的c次的函数值为p和c的异或值)
        F(ab)=F(a)F(b) gcd(a,b)==1
        求F前缀和(n<=10^10)
    Solution:
        除了2之外，p xor 1等于p-1，
        因此我们可以用质数常函数前缀g和质数前缀个数函数h
        来计算∑_{x是质数且x<=n}F(x)
*/
#include <bits/stdc++.h>
typedef long long ll;
const int N = 100000 << 1 | 1;  // sqrt(n)*2
const int P = 1e9 + 7;
int m, cnt, pri[N >> 2], h[N];
ll n, Sqr, sp[N], g[N], w[N];
inline int ID(ll x) { return x <= Sqr ? x : m - n / x + 1; }
// g:前缀质数和
// h:前缀质数个数
void Init(ll n) {
    Sqr = sqrt(n);
    cnt = m = 0;
    for (ll i = 1; i <= n; i = w[m] + 1) {
        w[++m] = n / (n / i);
        ll t = w[m] % P;
        g[m] = (t * (t + 1) >> 1) % P - 1;
        h[m] = w[m] - 1;
    }
    for (int i = 2; i <= Sqr; ++i){
        if (h[i] != h[i - 1]) {
            pri[++cnt] = i, sp[cnt] = sp[cnt - 1] + i;
            ll lim = 1ll * i * i;
            for (int j = m; lim <= w[j]; --j) {
                int k = ID(w[j] / i);
                g[j] -= i * (g[k] - sp[cnt - 1]);
                h[j] -= h[k] - cnt + 1;
            }
        }
        g[i] %= P;
	}
}
ll S(ll x, int y) {
    if (x <= 1 || pri[y] > x) return 0;
    ll res = g[ID(x)] - g[ID(pri[y - 1])] + P;
    for (int i = y; i <= cnt && 1ll * pri[i] * pri[i] <= x; ++i) {
        for (ll p = pri[i], p1 = p, j = 1; p1 * p <= x; p1 *= p, ++j)
            res += S(x / p1, i + 1) * (pri[i] ^ j) + (pri[i] ^ (j + 1));
    }
    return res % P;
}
int main() {
    scanf("%lld", &n);
    Init(n);
    for (int i = 1; i <= m; ++i) g[i] = (g[i] - h[i] + (i > 1) * 2 + P) % P;
    printf("%lld\n", (S(n, 1) + 1) % P);
    return 0;
}
/*
    Example3:
        对任意p为素数，
            p%4==1时有f(p^c)=3*c+1
            p%4!=1时f(p^c)=1
        对于非质数有
            f(a*b)=f(a)*f(b) gcd(a,b)==1
    Solution:
        处理g[n][r]表示前缀%4==r的质数个数，可以通过Min25预处理
        设G[n]等于4*g[n][1]+g[n][3]，对于唯一素数2特殊处理，n>1时G[n]++
        G[n]=∑F(x)_x为小于n的质数，G[pri[i-1]]=∑_{j=1}^{i-1}F(p_i)
        套Min25得到f前缀和即可
*/
#include <bits/stdc++.h>
typedef long long ll;
const int N = 50000 << 1 | 1;  // sqrt(n)*2
int n, m, Sqr, pnum, pri[N >> 2], w[N];
ll g[N][4], sp[N][4], G[N];
inline int ID(int x) { return x <= Sqr ? x : m - n / x + 1; }
int F(int p, int k) {
    if (p % 4 == 1) return 3 * k + 1;
    return 1;
}
bool notp[N];
void sieve() {
    memset(notp, 0, sizeof(notp));
    notp[0] = notp[1] = 1;
    pnum = 0;
    for (int i = 2; i < N; i++) {
        if (!notp[i]) {
            pri[++pnum] = i;
            for (int r = 0; r < 4; ++r)
                sp[pnum][r] = sp[pnum - 1][r] + (i % 4 == r);
        }
        for (int j = 1; j <= pnum && pri[j] * i < N; j++) {
            notp[pri[j] * i] = 1;
            if (i % pri[j] == 0) break;
        }
    }
}
void Init(int n) {
    Sqr = sqrt(n);
    m = 0;
    ll j;
    for (ll i = 1; i <= n; i = w[m] + 1ll) {
        w[++m] = n / (n / i);
        for (int r = 0; r < 4; r++) {
            if (r == 0)
                g[m][r] = w[m] / 4;
            else
                g[m][r] = w[m] / 4 + (w[m] % 4 >= r);
            if (r == 1) g[m][r]--;
        }
    }
    for (int i = 1; i <= pnum; ++i) {
        ll lim = 1ll * pri[i] * pri[i];
        for (int j = m; lim <= w[j]; --j) {
            int k = ID(w[j] / pri[i]);
            for (int r = 0; r < 4; ++r) {
                g[j][r * pri[i] % 4] -= g[k][r] - sp[i - 1][r];
            }
        }
    }
    for (int i = 1; i <= m; i++) G[i] = g[i][1] * 4 + g[i][3] + (i > 1);
}
ll S(ll x, int y) {
    if (x <= 1 || pri[y] > x) return 0;
    ll res = G[ID(x)] - G[pri[y - 1]];
    for (int i = y; i <= pnum && 1ll * pri[i] * pri[i] <= x; ++i) {
        for (ll p = pri[i], p1 = p, j = 1; p1 * p <= x; p1 *= p, ++j)
            res += S(x / p1, i + 1) * F(pri[i], j) + F(pri[i], j + 1);
    }
    return res;
}
int T;
int main() {
    sieve();
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        Init(n);
        printf("%lld\n", S(n, 1) + 1);
    }
    return 0;
}

