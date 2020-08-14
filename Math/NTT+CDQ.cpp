/*
    NTT+CDQ分治
*/
/*
    dp[i]=i!-sum_{j=0}^{i-1} dp[i-j]*j!
    题目大意：
        给出一个排列，每对逆序对之间连一条边，现在给出每个连通块中的元素，
        问满足要求的排列的种类
    题解：
        我们发现一个连通块中序号一定是连续的，否则方案为0，那么每个连通块方案的计算是独立的，
        而方案的数量只与连通块的大小有关，记大小为i的连通块方案为dp[i],
        考虑不合法的情况，不妨设最后一个块为j个连续的数，则需要减去的方案为dp[i-j]*j!
        有dp[i]=i!-sum_{j=0}^{i-1} dp[i-j]*j!，这是一个卷积运算，
        但是由于在计算过程中dp[i-j]是未知值，顺次计算复杂度是O(n^2logn)，
        考虑到加法运算对乘法运算可分配，因此可以采取CDQ分治，
        利用递归统计每个区间内左边DP值对右边DP值的贡献，
        对于每次贡献值的计算则利用NTT进行优化，优化时间复杂度至O(nlognlogn)。
*/
#include <algorithm>
#include <cstdio>
using namespace std;
typedef long long LL;
const int P = 786433;
namespace NTT {
const int G = 10;
const int NUM = 20;
int wn[20];
int mul(int x, int y, int P) { return (LL)x * y % P; }
int PowMod(int a, int b) {
    int res = 1;
    a %= P;
    while (b) {
        if (b & 1) res = mul(res, a, P);
        a = mul(a, a, P);
        b >>= 1;
    }
    return res;
}
void GetWn() {
    for (int i = 0; i < NUM; i++) {
        int t = 1 << i;
        wn[i] = PowMod(G, (P - 1) / t);
    }
}
void Change(int a[], int len) {
    int i, j, k;
    for (i = 1, j = len / 2; i < len - 1; i++) {
        if (i < j) swap(a[i], a[j]);
        k = len / 2;
        while (j >= k) {
            j -= k;
            k /= 2;
        }
        if (j < k) j += k;
    }
}
void NTT(int a[], int len, int on) {
    Change(a, len);
    int id = 0;
    for (int h = 2; h <= len; h <<= 1) {
        id++;
        for (int j = 0; j < len; j += h) {
            int w = 1;
            for (int k = j; k < j + h / 2; k++) {
                int u = a[k] % P;
                int t = mul(a[k + h / 2], w, P);
                a[k] = (u + t) % P;
                a[k + h / 2] = ((u - t) % P + P) % P;
                w = mul(w, wn[id], P);
            }
        }
    }
    if (on == -1) {
        for (int i = 1; i < len / 2; i++) swap(a[i], a[len - i]);
        int inv = PowMod(len, P - 2);
        for (int i = 0; i < len; i++) a[i] = mul(a[i], inv, P);
    }
}
}  // namespace NTT
const int N = 1 << 20;
int f[N], dp[N], a[N], b[N];
void CDQ(int l, int r) {
    if (l == r) {
        dp[l] = (-dp[l] + f[l] + P) % P;
        return;
    }
    int mid = (l + r) >> 1;
    CDQ(l, mid);
    int len = 1, n = mid - l + 1;
    while (len <= n << 1) len <<= 1;
    a[0] = b[0] = 0;
    for (int i = 1, j = l; i < len; i++, j++) a[i] = (j <= mid ? dp[j] : 0);
    for (int i = 1; i < len; i++) b[i] = f[i];
    NTT::NTT(a, len, 1);
    NTT::NTT(b, len, 1);
    for (int i = 0; i < len; i++) a[i] = 1LL * a[i] * b[i] % P;
    NTT::NTT(a, len, -1);
    for (int i = mid + 1, j = mid - l + 2; i <= r; i++, j++)
        (dp[i] += a[j]) %= P;
    CDQ(mid + 1, r);
}
int T, n, m, l, r, cnt, x;
int main() {
    NTT::GetWn();
    for (int i = f[0] = 1; i <= 100000; i++) f[i] = 1LL * f[i - 1] * i % P;
    CDQ(1, 100000);
    scanf("%d", &T);
    while (T--) {
        int ans = 1;
        scanf("%d%d", &n, &m);
        while (m--) {
            l = n + 1, r = -1;
            scanf("%d", &cnt);
            for (int i = 1; i <= cnt; i++) {
                scanf("%d", &x);
                l = min(l, x);
                r = max(r, x);
            }
            ans = 1LL * ans * dp[cnt] % P;
            if (r - l + 1 != cnt) ans = 0;
        }
        printf("%d\n", ans);
    }
    return 0;
}
