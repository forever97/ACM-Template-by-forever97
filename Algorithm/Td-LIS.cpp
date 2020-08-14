/*
    二维最长上升子序列（严格单调）
        一维排序（区间按照a进行排序，a相同情况下标大的在前）
        二维CDQ（下标划分左右区间，左边更新右边统计）
        三维BIT（以b为下标，用统计值dp对BIT更新，b值域离散化）
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int N = 100010;
int n, a[N], b[N], c[N], d[N], dp[N], p[N], q[N];
bool cmp(int x, int y) {
    if (a[x] == a[y]) return x > y;
    return a[x] < a[y];
}
void CDQ(int l, int r) {
    if (l == r) return;
    int mid = (l + r) >> 1;
    CDQ(l, mid);
    for (int i = l; i <= r; i++) q[i] = i;
    sort(q + l, q + r + 1, cmp);
    for (int i = l; i <= r; i++) {
        if (q[i] <= mid)
            for (int j = b[q[i]]; j <= p[0]; j += j & -j)
                c[j] = max(c[j], dp[q[i]]);
        else
            for (int j = b[q[i]] - 1; j; j -= j & -j)
                dp[q[i]] = max(dp[q[i]], c[j] + 1);
    }
    for (int i = l; i <= r; i++)
        if (q[i] <= mid)
            for (int j = b[q[i]]; j <= p[0]; j += j & -j) c[j] = 0;
    CDQ(mid + 1, r);
}
int main() {
    while (~scanf("%d", &n)) {
        p[0] = 0;
        for (int i = 1; i <= n; i++)
            scanf("%d%d", &a[i], &b[i]), p[++p[0]] = b[i], dp[i] = 1;
        sort(p + 1, p + p[0] + 1);
        p[0] = unique(p + 1, p + p[0] + 1) - p - 1;
        for (int i = 1; i <= n; i++)
            b[i] = lower_bound(p + 1, p + p[0] + 1, b[i]) - p;
        CDQ(1, n);
        int ans = 0;
        for (int i = 1; i <= n; i++) ans = max(ans, dp[i]);
        printf("%d\n", ans);
    }
    return 0;
}