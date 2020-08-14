/*
    悬线法
    求出最大全1矩阵的面积
    在处理每个点的时候，继承上一个点等高度下的左右最大扩展，
    计算在该层的左右最大扩展，然后对于每个点更新答案即可。
*/
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 2010;
int i, j, n, m, ans, l[N], r[N], h[N], lmax, rmax, a[N][N];
int main() {
    while (~scanf("%d%d", &n, &m)) {
        ans = 0;
        memset(h, 0, sizeof(h));
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++) scanf("%d", &a[i][j]);
        for (int i = 1; i <= m; i++) l[i] = 1, r[i] = m;
        for (int i = 1; i <= n; i++) {
            for (lmax = j = 1; j <= m; j++)
                if (a[i][j]) {
                    h[j]++;
                    if (lmax > l[j]) l[j] = lmax;
                } else
                    h[j] = 0, l[j] = 1, r[j] = m, lmax = j + 1;
            for (rmax = j = m; j; j--)
                if (a[i][j]) {
                    if (rmax < r[j]) r[j] = rmax;
                    if ((r[j] - l[j] + 1) * h[j] > ans)
                        ans = (r[j] - l[j] + 1) * h[j];
                } else
                    rmax = j - 1;
        }
        printf("%d\n", ans);
    }
    return 0;
}
/*
    Example：给出一个数字矩阵，求子矩阵期望数字种数
    Solution：我们统计[x,y]为其所表示的数字的最左上方的矩形数量，即该格子的贡献值，
    我们用悬线法将上边界上移，调整左右边界保证[x,y]为数字左上边界，
    那么x及其以下的部分都可以作为下边界，顺序统计即可，
    由于障碍点的顺序设置保证了二维的偏序性，不会出现重复统计贡献。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long LL;
const int N = 110;
int n, m, c, l[N], r[N], row[N * N][N];
LL Cal(int x, int y, int c) {
    LL res = 0;
    for (int i = 1; i <= x; i++) l[i] = 0, r[i] = m + 1;
    for (int i = 1; i < y; i++) l[row[c][i]] = i;
    for (int i = m; i > y; i--) r[row[c][i]] = i;
    int h = row[c][y];
    for (int i = x - 1; i > h; i--)
        l[i] = max(l[i], l[i + 1]), r[i] = min(r[i], r[i + 1]);
    for (int i = x; i > h; i--)
        res += (LL)(r[i] - y) * (y - l[i]) * (n - x + 1);
    return res;
}
int T;
int main() {
    scanf("%d", &T);
    while (T--) {
        memset(row, 0, sizeof(row));
        scanf("%d%d", &n, &m);
        LL ans = 0, tot = 0;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                scanf("%d", &c);
                ans += Cal(i, j, c);
                row[c][j] = i;
                tot += 1LL * i * j;
            }
        }
        printf("%.9f\n", 1.0 * ans / tot);
    }
    return 0;
}