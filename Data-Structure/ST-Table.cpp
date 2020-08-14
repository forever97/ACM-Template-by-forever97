/*
   ST表
   O(nlogn)预处理
   O(1)查询区间极值
*/
namespace ST {
int f[N][30], lg2[N];
void Init(int n) {
    for (int i = 2; i <= n; i++) lg2[i] = lg2[i / 2] + 1;
    for (int i = 1; i <= n; i++) f[i][0] = a[i];
    for (int j = 1; (1 << j) <= n; j++)
        for (int i = 1; i + (1 << j) - 1 <= n; i++)
            f[i][j] = min(f[i][j - 1], f[i + (1 << (j - 1))][j - 1]);
}
int Min(int l, int r) {
    if (l > r) swap(l, r);
    int k = lg2[r - l + 1];
    return min(f[l][k], f[r - (1 << k) + 1][k]);
}
}  // namespace ST
/*
    Problem: 求区间最长的连续相同的数字长度
    Solution:
   ST表维护区间不考虑边界限制的最长长度，将左右端连续长度去掉即可直接查询区间内的答案，
    左右端点单独考虑即可。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 100010;
int n, m, sumL[N], sumR[N], a[N], d[N][20], lg2[N];
int Max(int l, int r) {
    int x = min(sumR[l], r - l + 1);
    int y = min(sumL[r], r - l + 1);
    l += x;
    r -= y;
    if (r < l) return max(x, y);
    int k = lg2[r - l + 1];
    return max(max(x, y), max(d[l][k], d[r - (1 << k) + 1][k]));
}
int main() {
    while (scanf("%d", &n), n) {
        scanf("%d", &m);
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]), sumL[i] = sumR[i] = 1;
        for (int i = 2; i <= n; i++)
            if (a[i] == a[i - 1]) sumL[i] += sumL[i - 1];
        for (int i = n - 1; i; i--)
            if (a[i] == a[i + 1]) sumR[i] += sumR[i + 1];
        for (int i = 2; i <= n; i++) lg2[i] = lg2[i / 2] + 1;
        for (int i = 1; i <= n; i++) d[i][0] = sumL[i] + sumR[i] - 1;
        ;
        for (int j = 1; (1 << j) <= n; j++)
            for (int i = 1; i + (1 << j) - 1 <= n; i++)
                d[i][j] = max(d[i][j - 1], d[i + (1 << (j - 1))][j - 1]);
        int x, y;
        while (m--) {
            scanf("%d%d", &x, &y);
            printf("%d\n", Max(x, y));
        }
    }
    return 0;
}
