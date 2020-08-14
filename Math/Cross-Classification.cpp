/*
    容斥原理
*/
/*
    Example
       给出一些数，求取出一些数，当他们的GCD大于0时，将数量乘GCD累加到答案上，求累加和。
    Solution
        我们枚举GCD，统计为其倍数的数字数量，先假定其能组成的集合数为贡献，
        但是我们发现在统计的过程中有多余统计的部分，比如4和8是2的倍数，
        然而它们的GCD等于4，所以我们对于每个集合数的贡献要减去所有其倍数的集合数的贡献，
        倒着容斥即可。
*/
const int N = 1000010, MOD = 1e9 + 7;
int n, ans = 0, w[N], dp[N], pw[N], mx;
int main() {
    scanf("%d", &n);
    for (int i = pw[0] = 1; i <= n; i++) pw[i] = 2 * pw[i - 1] % MOD;
    for (int i = 1, x; i <= n; i++) scanf("%d", &x), mx = max(x, mx), w[x]++;
    for (int i = mx; i > 1; i--) {
        int t = 0;
        for (int j = i; j <= mx; j += i) t += w[j];
        if (!t) continue;
        dp[i] = 1LL * t * pw[t - 1] % MOD;
        for (int j = i + i; j <= mx; j += i)
            dp[i] = (dp[i] - dp[j] + MOD) % MOD;
        ans = (1LL * dp[i] * i + ans) % MOD;
    }
    printf("%d\n", ans);
    return 0;
}
/*
    Example
        m个石子围成一圈，标号为0~m-1，现在有n只青蛙，每只每次跳a[i]个石子，
        问能被青蛙跳到的石子一共有几个
    Solution
        我们发现k*gcd(m,a[i])的位置均可以被跳到，那么我们首先筛出m的约数，
        判断其是否被覆盖到，不考虑重复的情况下，
        每个被覆盖到的约数的贡献为x*((m-1)/x)*((m-1)/x+1)/2，
        但是约数的倍数也为约数的情况被重复计算，因此我们按约数从大到小容斥计算答案。
*/
typedef long long LL;
int T, n, m, p[20010], mark[20010], x, tot;
LL dp[20010];
int main() {
    scanf("%d", &T);
    for (int Cas = 1; Cas <= T; Cas++) {
        memset(dp, 0, sizeof(dp));
        memset(mark, 0, sizeof(mark));
        scanf("%d%d", &n, &m);
        tot = 0;
        for (int i = 1; i * i <= m; i++) {
            if (m % i == 0) {
                p[++tot] = i;
                if (i * i != m) p[++tot] = m / i;
            }
        }
        sort(p + 1, p + tot + 1);
        for (int i = 1; i <= n; i++) {
            scanf("%d", &x);
            int GCD = __gcd(x, m);
            for (int j = 1; j <= tot; j++)
                if (p[j] % GCD == 0) mark[j] = 1;
        }
        LL ans = 0;
        for (int i = tot; i; i--)
            if (mark[i]) {
                int t = (m - 1) / p[i];
                dp[i] = 1LL * t * (t + 1) / 2 * p[i];
                for (int j = i + 1; j <= tot; j++)
                    if (mark[j] && p[j] % p[i] == 0) dp[i] -= dp[j];
                ans = ans + dp[i];
            }
        printf("Case #%d: %lld\n", Cas, ans);
    }
    return 0;
}
/*
    题目大意：
        给出一个数列每个位置可以取到的最大值，
        问这个可以构造多少个数列，使得他们的最大公约数大于1
    题解：
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
