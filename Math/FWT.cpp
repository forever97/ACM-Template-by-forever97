/*
    快速沃尔什变化
*/
void FWT(int *a, int n) {
    for (int d = 1; d < n; d <<= 1)
        for (int m = d << 1, i = 0; i < n; i += m)
            for (int j = 0; j < d; j++) {
                int x = a[i + j], y = a[i + j + d];
                // xor:a[i+j]=x+y,a[i+j+d]=x-y;
                // and:a[i+j]=x+y;
                // or:a[i+j+d]=x+y;
            }
}
void UFWT(int *a, int n) {
    for (int d = 1; d < n; d <<= 1)
        for (int m = d << 1, i = 0; i < n; i += m)
            for (int j = 0; j < d; j++) {
                int x = a[i + j], y = a[i + j + d];
                // xor:a[i+j]=(x+y)/2,a[i+j+d]=(x-y)/2;
                // and:a[i+j]=x-y;
                // or:a[i+j+d]=y-x;
            }
}
/*
    Problem：有n堆石子，每堆都是m以内的质数，请问后手必胜的局面有几种
    Solution：后手必胜，则sg为0，那么就是求n个m以内的数xor为0的情况有几种，
    首先筛出素数，保存素数的个数数组，利用FWT计算c[i^j]=a[i]*b[j]，
    计算n次的结果逆向变化回来就是最终的sg个数数组，
    在计算n次c[i]=a[i]*b[i]的过程中，等价于计算c[i]=a[i]^n，
    这里我们可以用快速幂优化一个log。
*/
#include <algorithm>
#include <cstdio>
using namespace std;
typedef long long LL;
const int N = 100000;
const LL mod = 1e9 + 7;
LL a[N], u;
int p[N], n, m;
void FWT(LL *a, int n) {
    for (int d = 1; d < n; d <<= 1)
        for (int m = d << 1, i = 0; i < n; i += m)
            for (int j = 0; j < d; j++) {
                LL x = a[i + j], y = a[i + j + d];
                a[i + j] = (x + y) % mod, a[i + j + d] = (x - y + mod) % mod;
            }
}
void UFWT(LL *a, int n) {
    for (int d = 1; d < n; d <<= 1)
        for (int m = d << 1, i = 0; i < n; i += m)
            for (int j = 0; j < d; j++) {
                LL x = a[i + j], y = a[i + j + d];
                a[i + j] = (x + y) % mod * u % mod,
                      a[i + j + d] = (x - y + mod) % mod * u % mod;
            }
}
LL pow(LL a, LL b, LL p) {
    LL t = 1;
    for (a %= p; b; b >>= 1LL, a = a * a % p)
        if (b & 1LL) t = t * a % p;
    return t;
}
int main() {
    for (int i = 2; i <= 50000; i++) p[i] = 1;
    for (int i = 2; i <= 50000; i++)
        if (p[i]) {
            for (int j = 2; i * j <= 50000; j++) p[i * j] = 0;
        }
    u = pow(2, mod - 2, mod);
    while (~scanf("%d%d", &n, &m)) {
        int len = 1;
        while (len <= m) len <<= 1;
        for (int i = 0; i < len; i++) a[i] = p[i] & (i <= m);
        FWT(a, len);
        for (int i = 0; i < len; i++) a[i] = pow(a[i], n, mod);
        UFWT(a, len);
        printf("%lld\n", a[0]);
    }
    return 0;
}
/*
    Problem：给出一棵树，其每棵连通子树的价值为其点权的xor和，
    问有多少连通子树的价值为1~m
    Solution：首先定1为根，转有根树，我们在树的每个节点保存一个权值数组，
    表示与其连通的子树的权值，当一个子树并入其父节点时，
    dp[x][i]=dp[x][i]+dp[x][j]*dp[y][k](j^k==i)，我们发现这是一个位运算卷积式子，
    所以树上转移可以用fwt加速。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
typedef long long LL;
const int N = 1030;
const LL mod = 1e9 + 7;
const LL inv2 = (mod + 1) / 2;
int val[N];
LL dp[N][N], tmp[N];
vector<int> v[N];
void FWT(LL *a, int n) {
    for (int d = 1; d < n; d <<= 1)
        for (int m = d << 1, i = 0; i < n; i += m)
            for (int j = 0; j < d; j++) {
                LL x = a[i + j], y = a[i + j + d];
                a[i + j] = (x + y) % mod, a[i + j + d] = (x - y + mod) % mod;
            }
}
void UFWT(LL *a, int n) {
    for (int d = 1; d < n; d <<= 1)
        for (int m = d << 1, i = 0; i < n; i += m)
            for (int j = 0; j < d; j++) {
                LL x = a[i + j], y = a[i + j + d];
                a[i + j] = (x + y) % mod * inv2 % mod,
                      a[i + j + d] = (x - y + mod) % mod * inv2 % mod;
            }
}
int T, n, m, x, y;
LL ans[N];
void Cal(LL *a, LL *b) {
    for (int i = 0; i < m; i++) tmp[i] = a[i];
    FWT(a, m);
    FWT(b, m);
    for (int i = 0; i < m; i++) a[i] = (1ll * a[i] * b[i]) % mod;
    UFWT(a, m);
    UFWT(b, m);
    for (int i = 0; i < m; i++) a[i] = (a[i] + tmp[i]) % mod;
}
void DP(int x, int fx) {
    dp[x][val[x]] = 1;
    for (auto y : v[x]) {
        if (y == fx) continue;
        DP(y, x);
        Cal(dp[x], dp[y]);
    }
}
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 0; i <= n; i++) v[i].clear();
        for (int i = 1; i <= n; i++) scanf("%d", &val[i]);
        for (int i = 1; i < n; i++) {
            scanf("%d%d", &x, &y);
            v[x].push_back(y);
            v[y].push_back(x);
        }
        memset(dp, 0, sizeof(dp));
        memset(ans, 0, sizeof(ans));
        DP(1, 1);
        for (int i = 0; i < m; i++) {
            for (int j = 1; j <= n; j++) ans[i] = (ans[i] + dp[j][i]) % mod;
        }
        for (int i = 0; i < m; i++) printf(i < m - 1 ? "%d " : "%d\n", ans[i]);
    }
    return 0;
}
/*
    Problem：定义一张无向图的价值：
        给每个节点染色使得每条边连接的两个节点颜色不相同的最少颜色数。 　　
        对于给定的一张由n个点组成的无向图，求该图的2^n-1张非空子图的价值。
    Solution：设f[i][S]表示i种颜色覆盖S这个集合的方案数，
    我们只要得到最小的i，f[i][S]大于0，那么i就是S集合的答案。
    显然有f[i][S]=∑f[1][u]×f[i-1][v](u|v==S)，
    处理位运算形式的卷积，所以我们现在只要求n次FWT，就可以得到答案。
*/
#include <cstdio>
const int N = 18, M = 1 << N;
char s[N + 2];
int T, n, i, j, g[N], f[N + 1][M], h[M];
unsigned int pow[M], ans;
void FWT(int *a, int n) {
    for (int d = 1; d < n; d <<= 1)
        for (int m = d << 1, i = 0; i < n; i += m)
            for (int j = 0; j < d; j++) {
                a[i + j + d] = a[i + j] + a[i + j + d];
            }
}
void UFWT(int *a, int n) {
    for (int d = 1; d < n; d <<= 1)
        for (int m = d << 1, i = 0; i < n; i += m)
            for (int j = 0; j < d; j++) {
                a[i + j + d] = a[i + j + d] - a[i + j];
            }
}
void mul() {
    FWT(h, 1 << n);
    for (i = 2; i <= n; i++) {
        for (j = 0; j < 1 << n; j++) f[i][j] = f[i - 1][j];
        FWT(f[i], 1 << n);
        for (j = 0; j < 1 << n; j++) f[i][j] *= h[j];
        UFWT(f[i], 1 << n);
        for (j = 0; j < 1 << n; j++) f[i][j] = !!f[i][j];
    }
}
int main() {
    scanf("%d", &T);
    for (pow[0] = i = 1; i < M; i++) pow[i] = pow[i - 1] * 233;
    while (T--) {
        scanf("%d", &n);
        for (i = 0; i < n; i++) {
            scanf("%s", s);
            g[i] = 0;
            for (j = 0; j < n; j++)
                if (s[j] == '1') g[i] |= 1 << j;
        }
        for (i = 0; i < 1 << n; i++) f[1][i] = 0;
        for (i = f[1][0] = 1; i < (1 << n); i++) {
            j = i & -i;
            if (!f[1][i - j]) continue;
            if (g[__builtin_ctz(j)] & i) continue;
            f[1][i] = 1;
        }
        for (j = 0; j < 1 << n; j++) h[j] = f[1][j];
        mul();
        ans = 0;
        for (i = 1; i < 1 << n; i++) {
            for (j = 1; !f[j][i]; j++)
                ;
            ans += j * pow[i];
        }
        printf("%u\n", ans);
    }
    return 0;
}
/*
    Problem：给出一个数集，A从中选择一些数，B从中选择一些数，不能同时不选
    要求两者选择的数异或和为0，问方案数
    Solution：题目等价于选取一个非空且xor为0的集合并将其拆分为两个子集的方案数
    用dp表示xor为j的方案数，易得dp方程dp[i][j]=dp[i-1][j]+2*dp[i-1][j^a[i]]
    该式等价于dp数组与只有两个元素有值的g[0]=1,g[a[i]]=2的数组做卷积运算
    对g数组进行反演可以发现每次卷积的数组只包含3和-1，
    那么我们只要知道对一个下标来说，做的n次卷积中有几个3和-1，
    就能够直接乘算出答案，根据FWT的和等于和的FWT，我们将多次需要做卷积的数组相加，
    一并做FWT，得到他们和的反演值，在每个位置解关于3和-1的二元一次方程组，
    再将其替换为正确值，最后FWT求逆之后下标为0的答案减去1就是答案，
    减一是因为两个人取数不能同时为空。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int P = 998244353;
const int inv2 = (P + 1) >> 1;
const int N = 2000000;
void FWT(int *a, int n) {
    for (int d = 1; d < n; d <<= 1)
        for (int m = d << 1, i = 0; i < n; i += m)
            for (int j = 0; j < d; j++) {
                int x = a[i + j], y = a[i + j + d];
                a[i + j] = (x + y) % P, a[i + j + d] = (x - y + P) % P;
            }
}
void UFWT(int *a, int n) {
    for (int d = 1; d < n; d <<= 1)
        for (int m = d << 1, i = 0; i < n; i += m)
            for (int j = 0; j < d; j++) {
                int x = a[i + j], y = a[i + j + d];
                a[i + j] = 1LL * (x + y) * inv2 % P,
                      a[i + j + d] = 1LL * (x - y) * inv2 % P;
            }
}
int n, x, mx, pw[N], a[N];
int main() {
    pw[0] = 1;
    for (int i = 1; i < N; i++) pw[i] = 3LL * pw[i - 1] % P;
    while (~scanf("%d", &n)) {
        memset(a, 0, sizeof(a));
        for (int i = mx = 1; i <= n; i++) {
            scanf("%d", &x);
            a[0]++;
            a[x] += 2;
            mx = max(mx, x);
        }
        int m = 1;
        while (m <= mx) m <<= 1;
        FWT(a, m);
        for (int i = 0; i < m; i++) {
            x = (3ll * n + P - a[i]) * inv2 % P * inv2 % P;
            a[i] = (x & 1) ? (P - pw[n - x]) % P : pw[n - x];
        }
        UFWT(a, m);
        printf("%d\n", (a[0] + P - 1) % P);
    }
    return 0;
}