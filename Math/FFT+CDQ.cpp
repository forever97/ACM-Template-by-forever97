/*
    FFT+CDQ分治
*/
/*
    dp[i]=sum_{j=0}^{i-1} dp[j]w[i-j]
    c[i]=sum_{j=0}^i a[j]b[i-j]
    题目大意:
        给出一个数组w，表示不同长度的字段的权值，
        比如w[3]=5表示如果字段长度为3，则其权值为5，现在有长度为n的字段，
        求通过不同拆分得到的字段权值乘积和。
    题解:
        记DP[i]表示长度为i时候的答案，DP[i]=sum_{j=0}^{i-1}DP[j]w[i-j]，
        发现是一个卷积的式子，因此运算过程可以用FFT优化，
        但是由于在计算过程中DP[j]是未知值，顺次计算复杂度是O(n2logn)，
        考虑到加法运算对乘法运算可分配，因此可以采取CDQ分治，
        利用递归统计每个区间内左边DP值对右边DP值的贡献，
        对于每次贡献值的计算则利用FFT进行优化，优化时间复杂度至O(nlognlogn)。
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
using namespace std;
typedef long long LL;
const int N = 524300, P = 313;
int n, pos[N];
namespace FFT {
struct comp {
    double r, i;
    comp(double _r = 0, double _i = 0) : r(_r), i(_i) {}
    comp operator+(const comp &x) { return comp(r + x.r, i + x.i); }
    comp operator-(const comp &x) { return comp(r - x.r, i - x.i); }
    comp operator*(const comp &x) {
        return comp(r * x.r - i * x.i, i * x.r + r * x.i);
    }
    comp conj() { return comp(r, -i); }
} A[N], B[N];
const double pi = acos(-1.0);
void FFT(comp a[], int n, int t) {
    for (int i = 1; i < n; i++)
        if (pos[i] > i) swap(a[i], a[pos[i]]);
    for (int d = 0; (1 << d) < n; d++) {
        int m = 1 << d, m2 = m << 1;
        double o = pi * 2 / m2 * t;
        comp _w(cos(o), sin(o));
        for (int i = 0; i < n; i += m2) {
            comp w(1, 0);
            for (int j = 0; j < m; j++) {
                comp &A = a[i + j + m], &B = a[i + j], t = w * A;
                A = B - t;
                B = B + t;
                w = w * _w;
            }
        }
    }
    if (t == -1)
        for (int i = 0; i < n; i++) a[i].r /= n;
}
void mul(int *a, int *b, int *c, int k) {
    int i, j;
    for (i = 0; i < k; i++) A[i] = comp(a[i], b[i]);
    j = __builtin_ctz(k) - 1;
    for (int i = 0; i < k; i++) {
        pos[i] = pos[i >> 1] >> 1 | ((i & 1) << j);
    }
    FFT(A, k, 1);
    for (int i = 0; i < k; i++) {
        j = (k - i) & (k - 1);
        B[i] = (A[i] * A[i] - (A[j] * A[j]).conj()) * comp(0, -0.25);
    }
    FFT(B, k, -1);
    for (int i = 0; i < k; i++) c[i] = (long long)(B[i].r + 0.5) % P;
}
}  // namespace FFT
int w[N], a[N], b[N], c[N], F[N];
void CDQ(int l, int r) {
    if (l == r) {
        F[l] += w[l];
        F[l] %= P;
        return;
    }
    int mid = (l + r) >> 1;
    CDQ(l, mid);
    int N = 1;
    while (N < r - l) N <<= 1;
    for (int i = 0; i <= mid - l; i++) a[i] = F[i + l];
    for (int i = mid - l + 1; i < N; i++) a[i] = 0;
    for (int i = 0; i < r - l; i++) b[i] = w[i + 1];
    for (int i = r - l; i < N; i++) b[i] = 0;
    FFT::mul(a, b, c, N);
    for (int i = mid + 1; i <= r; i++) {
        F[i] += c[i - l - 1];
        F[i] %= P;
    }
    CDQ(mid + 1, r);
}
int main() {
    while (~scanf("%d", &n) && n) {
        F[0] = 1;
        for (int i = 1; i <= n; i++) {
            scanf("%d", &w[i]);
            w[i] %= P;
            F[i] = 0;
        }
        CDQ(1, n);
        printf("%d\n", F[n]);
    }
    return 0;
}
