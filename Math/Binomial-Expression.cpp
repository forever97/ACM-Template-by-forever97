/*
    二项式展开
*/
/*
    Example1:
        m个位置每个位置可以填上[0,n-1]，求m个数和为k的方案数
    Solution:
        答案为生成函数(1+x^1+x^2+x^3+……)^m中x^k的系数
        1+x^1+x^2+x^3+……等比数列求和为(1-x^n)/(1-x)
        那么生成函数为(1-x^n)^m*(1-x)^(-m)
        (1-x^n)^m展开第i项为C(m,i)*(-1)^i*x^i
        (1-x)^(-m)展开第i项为C(m+i-1,i)*x^i
        我们找到对应幂次乘积为k的系数乘积即可
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long LL; 
const int P = 998244353;
const int U = 200000;
int f[U + 3], rf[U + 3];
LL inv(LL a, LL m) { return (a == 1 ? 1 : inv(m % a, m) * (m - m / a) % m); }
void init() {
    f[0] = 1;
    for (int i = 1; i <= U; i++) f[i] = (LL)f[i - 1] * i % P;
    rf[U] = inv(f[U], P);
    for (int i = U; i; i--) rf[i - 1] = (LL)rf[i] * i % P;
}
LL C(int n, int m) {
    if (m < 0 || m > n) return 0;
    return (LL)f[n] * rf[m] % P * rf[n - m] % P;
}
int T, n, m, k;
int main() {
    init();
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d%d", &n, &m, &k);
        int ans = 0;
        if (1ll * m * (n - 1) < k) {
            puts("0");
            continue;
        }
        for (int i = 0; i <= k / n; i++) {
            int j = k - n * i;
            int upd = 1LL * C(m, i) * C(m + j - 1, j) % P;
            if (i % 2)
                ans = (ans - upd + P) % P;
            else
                ans = (ans + upd) % P;
        }
        printf("%d\n", ans);
    }
    return 0;
}
/*
    Example2：
        求出Ans[n]=(a[1]+a[2]+……+a[n])^k+(a[2]+……+a[n])^k+(a[3]+……+a[n])^k，
        并顺序输出Ans[1~n]。
    Solution：
        记s[n]=∑_{i=1}^{n}a[i]，
        则Ans[n]=∑_{i=0}^{n}(s[n]-s[i])^k
        我们将式子展开，得到Ans[n]=∑_C(k,i)s[n]^(k-i)*(∑_{i=0}^{n-1}(-s[j])^i)
        我们预处理S[i][j]=s[i]^j 以及 R[i][j]=∑_{t=0}^{i}S[t][j]
        那么就可以O(nk)计算答案了。
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int N = 100010, M = 110;
typedef long long LL;
const LL P = 1000000007LL;
LL C[M][M], a[N], ans[N], s[N], S[N][M], T[N][M], R[N][M];
char ch[N];
int Cas, n, k;
void up(LL &x, LL y) {
    x += y;
    if (x >= P) x -= P;
    if (x < 0) x += P;
}
int main() {
    scanf("%d", &Cas);
    while (Cas--) {
        scanf("%d%d", &n, &k);
        for (int i = 0; i <= k; i++) C[i][0] = C[i][i] = 1LL;
        for (int i = 1; i <= k; i++)
            for (int j = 1; j <= i; j++)
                C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % P;
        scanf("%s", ch + 1);
        for (int i = 1; i <= n; i++) a[i] = ch[i] - '0';
        for (int i = 1; i <= n; i++) s[i] = (s[i - 1] + a[i]) % P;
        // S[i][j]=s[i]^j
        for (int i = 0; i <= n; i++) S[i][0] = 1;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= k; j++) S[i][j] = S[i][j - 1] * s[i] % P;
        // R[i][j]=∑S[t][j](t<=i)
        for (int i = 0; i <= k; i++) R[0][i] = S[0][i];
        for (int i = 1; i <= n; i++)
            for (int j = 0; j <= k; j++) R[i][j] = (R[i - 1][j] + S[i][j]) % P;
        for (int i = 1; i <= n; i++) {
            ans[i] = 0;
            for (int p = 0; p <= k; p++) {
                if (p % 2 == 0)
                    up(ans[i], C[k][p] * S[i][k - p] % P * R[i - 1][p] % P);
                else
                    up(ans[i], -C[k][p] * S[i][k - p] % P * R[i - 1][p] % P);
            }
        }
        for (int i = 1; i <= n; i++)
            printf("%lld%c", ans[i], i == n ? '\n' : ' ');
    }
    return 0;
}


