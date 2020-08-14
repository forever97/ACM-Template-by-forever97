/*
    组合数
*/
namespace Comb {
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
}  // namespace Comb
/*
    Problem:
        求解∑_(1,n)∑_(i+1,n)C(Ai+Aj+Bi+Bj,Ai+Aj)，A,B<=2000，n<=200000
    Solution：
        C(Ai+Aj+Bi+Bj,Ai+Aj)在方格上表示即从(-Ai,-Bi)开始
        只能向右走或者向下走走到(Aj,Bj)的方案数，
        有设dp[(起点坐标)]=1，dp[i][j]=dp[i-1][j]+dp[i][j-1]，Ans=dp[(终点坐标)]
        对于每一个组我们可以建立起点(-A[i],-B[i])与终点(A[i],B[i])，起点初始化为1
        直接dp计算即可，考虑到多算了(-A[i],-B[i])到(A[i],B[i])的方案数，
        所以我们要容斥掉C(A[i]+A[i]+B[i]+B[i],A[i]+A[i])，最后考虑偏序，答案除2即可
*/
const int base = 2000, P = 1e9 + 7, N = 200010;
int dp[4010][4010], a[N], b[N];
using namespace Comb;
inline void up(int &x) {
    while (x < 0) x += P;
    while (x >= P) x -= P;
}
int main() {
    int n;
    init();
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d", &a[i], &b[i]);
        dp[base - a[i]][base - b[i]]++;
    }
    for (int i = 0; i <= base + base; i++) {
        for (int j = 0; j <= base + base; j++) {
            if (i) dp[i][j] += dp[i - 1][j], up(dp[i][j]);
            if (j) dp[i][j] += dp[i][j - 1], up(dp[i][j]);
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        ans += dp[a[i] + base][b[i] + base];
        up(ans);
        ans -= C(a[i] + a[i] + b[i] + b[i], a[i] + a[i]);
        up(ans);
    }
    printf("%lld\n", 1ll * ans * rf[2] % P);
    return 0;
}