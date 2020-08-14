/*
    分块求和
*/
ll solve(int n, int m) {
    if (n > m) swap(n, m);
    ll res = 0;
    for (int i = 1, last; i <= n; i = last + 1) {
        last = min(n / (n / i), m / (m / i));
        res += (f[last] - f[i - 1]) * (n / i) * (m / i);
    }
    return res;
}
/*
    Example1：
        求∑∑((n%i)*(m%j))其中1<=i<=n,1<=j<=m,i≠j。
    Solution：
        sum_{i=1}^{n}sum_{j=1}^{m}((n%i)*(m%j))(i≠j)
        =sum_{i=1}^{n}sum_{j=1}^{m}(n-(n/i)*i)*(m-(m/j)*j)
            -sum_{i=1}^{min(n,m)}(n-(n/i)*i)*(m-(m/i)*i)
        =sum_{i=1}^{n}(n-n/i)*sum_{i=1}^{m}(m-m/i)
            -sum_{i=1}^{min(n,m)}n*m-n*(m/i)*i
            -m*(n/i)*i+(n/i)(m/i)*i*i
        我们对于n/i分段统计即可。
*/
#include <algorithm>
#include <cstdio>
using namespace std;
typedef long long LL;
const LL inv6 = 3323403;
const LL mod = 19940417;
LL n, m, ans;
LL sum(LL a, LL b) { return (b - a + 1) * (a + b) / 2 % mod; }
LL sum2(LL x) { return x * (x + 1) % mod * (2 * x + 1) % mod * inv6 % mod; }
LL cal(LL n) {
    LL res = 0;
    for (LL l = 1, r; l <= n; l = r + 1) {
        r = n / (n / l);
        res = (res + n * (r - l + 1) % mod - sum(l, r) * (n / l)) % mod;
    }
    return (res + mod) % mod;
}
int main() {
    while (~scanf("%lld%lld", &n, &m)) {
        ans = cal(n) * cal(m) % mod;
        if (n > m) swap(n, m);
        for (int l = 1, r; l <= n; l = r + 1) {
            r = min(n / (n / l), m / (m / l));
            LL s1 = n * m % mod * (r - l + 1) % mod;
            LL s2 =
                (n / l) * (m / l) % mod * (sum2(r) - sum2(l - 1) + mod) % mod;
            LL s3 = (n / l * m + m / l * n) % mod * sum(l, r) % mod;
            ans = (ans - (s1 + s2 - s3) % mod + mod) % mod;
        }
        printf("%lld\n", ans);
    }
    return 0;
}
/*
    Example2:
        求∑gcd(x^a−1,x^b−1) (1≤a,b≤n)
    Solution:
        ∑gcd(x^a−1,x^b−1)=∑(x^gcd(a,b)-1)
        枚举gcd，题目变为求∑((x^k-1)*(2*∑phi(n/k)-1))
        n/k的取值个数有限，我们对n/k数值分块即可
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N = 1000000 + 10;
const int P = 1e9 + 7;
bool notp[N];
int phi[N], prime[N], pnum;
ll F[N];
void sieve() {
    memset(notp, 0, sizeof(notp));
    F[1] = phi[1] = notp[0] = notp[1] = 1;
    pnum = 0;
    for (int i = 2; i < N; i++) {
        if (!notp[i]) prime[++pnum] = i, phi[i] = i - 1;
        for (int j = 1; j <= pnum && prime[j] * i < N; j++) {
            notp[prime[j] * i] = 1;
            if (i % prime[j] == 0) {
                phi[i * prime[j]] = phi[i] * prime[j];
                break;
            }
            phi[i * prime[j]] = phi[i] * (prime[j] - 1);
        }
        F[i] = (F[i - 1] + phi[i]) % P;
    }
}
ll pow(ll a, ll b, ll p) {
    ll t = 1;
    for (a %= p; b; b >>= 1ll, a = a * a % p)
        if (b & 1ll) t = t * a % p;
    return t;
}
ll solve(int x, int n) {
    ll res = 0;
    for (int i = 1, last; i <= n; i = last + 1) {
        last = n / (n / i);
        ll t = pow(x, last + 1, P) - pow(x, i, P);
        t = t * pow(x - 1, P - 2, P) % P - last + i - 1;
        res += (t + P) % P * ((F[n / i] << 1) - 1) % P;
        res %= P;
    }
    return res;
}
int T, n, x;
int main() {
    scanf("%d", &T);
    sieve();
    while (T--) {
        scanf("%d%d", &x, &n);
        if (x == 1) {
            puts("0");
            continue;
        }
        printf("%lld\n", (solve(x, n) + P) % P);
    }
    return 0;
}
/*
    Example3：
        给出一个数列每个位置可以取到的最大值，
        问这个可以构造多少个数列，使得他们的最大公约数大于1
    Solution：
        我们可以枚举最大公约数k，对于k来说，
        他对答案的贡献为∏[ai/k]，我们将数列中的数字转化为权值数组
        ∏_{i=1}^{100000}[i/k]，对于求解i/k的部分我们可以进行数值分块，
        j*k-1~j*k+k-1的数值除k得到的结果都是相同的，因此可以直接求这个结果的幂次，
        这时候只要再加一个权值数组的前缀和，问题就迎刃而解了。
        数值分块计算的复杂度为n+n/2+n/3+n/4+n/5+……+n/n=nlogn。
        对于计算结果，我们需要进行容斥，奇数次素数乘的系数为1，偶数次素数乘的系数为-1，
        对于出现素数幂的合数其系数为0，
        我们发现这个容斥恰好是莫比乌斯函数的相反数，因此我们取反即可。
        这有个小小的优化，对于系数为0的情况，我们可以直接跳过，不进行计算。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 200010;
typedef long long LL;
const LL mod = 1000000007;
int T, n, a[N], b[N], cnt[N], cas = 1, p[N];
LL ans = 0;
int tot, miu[N], sum[N], v[N];
void read(int& a) {
    char ch;
    while (!((ch = getchar()) >= '0') && (ch <= '9'))
        ;
    a = ch - '0';
    while (((ch = getchar()) >= '0') && (ch <= '9')) a *= 10, a += ch - '0';
}
void mobius(int n) {
    int i, j;
    for (miu[1] = 1, i = 2; i <= n; i++) {
        if (!v[i]) p[tot++] = i, miu[i] = -1;
        for (j = 0; j < tot && i * p[j] <= n; j++) {
            v[i * p[j]] = 1;
            if (i % p[j])
                miu[i * p[j]] = -miu[i];
            else
                break;
        }
    }
    for (i = 1; i < n; i++) sum[i] = sum[i - 1] + miu[i];
}
LL pow(LL a, LL b, LL p) {
    if (b == 0) return 1;
    LL t = 1;
    for (a %= p; b; b >>= 1LL, a = a * a % p)
        if (b & 1LL) t = t * a % p;
    return t;
}
int main() {
    read(T);
    mobius(100000);
    while (T--) {
        read(n);
        ans = 0;
        int mn = ~0U >> 1, mx = 0;
        memset(cnt, 0, sizeof(cnt));
        for (int i = 1; i <= n; i++)
            read(a[i]), mn = min(a[i], mn), mx = max(a[i], mx), cnt[a[i]]++;
        for (int i = 1; i <= 200000; i++) cnt[i] += cnt[i - 1];
        for (int i = 2; i <= mn; i++) {
            if (!miu[i]) continue;
            LL tmp = 1;
            for (int j = 1; i * j <= 100000; j++)
                tmp = tmp * pow(j, cnt[i * j + i - 1] - cnt[i * j - 1], mod) %
                      mod;
            // j<=100000/i -> i*j<=100000 : TLE -> AC
            ans = (ans - tmp * miu[i] + mod) % mod;
        }
        printf("Case #%d: %lld\n", cas++, ans);
    }
    return 0;
}


