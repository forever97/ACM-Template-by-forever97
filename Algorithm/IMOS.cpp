/*
    imos法
    Example：给出一个nxm的矩形方格图，给出每个矩形障碍块的左上角和右下角，
    多次询问，每次给出一个矩形，问放入这个有障碍的方格图有几种方案
    Solution：通过二维差分计算出可以放的区域，枚举每一行作为底边，维护高度的单调栈，
    当每个元素出栈的时候维护包含位置的最大矩形，这些最大矩形对于长宽不超过自己的询问贡献均为1，
    因为最大矩形是二维imos差分后位置的区间修改(一段-1和一段+1)，所以可以继续差分为单点修改，
    最后计算二维差分的前缀和就可以得到答案的预处理数组。
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int N = 2010;
int n, m, u, q;
int top, h[N], st[N], mp[N][N], ans[N][N];
void doit(int x, int y, int z, int l) {
    ans[l][0]--;
    ans[l][y - x + 1]++;
    ans[l][z - y + 1]++;
    ans[l][z - x + 1 + 1]--;
}
int main() {
    scanf("%d%d%d%d", &n, &m, &u, &q);
    for (int i = 1; i <= u; i++) {
        int x1, y1, x2, y2;
        scanf("%d%d%d%d", &x1, &x2, &y1, &y2);
        mp[x1][y1]++;
        mp[x2][y2]++;
        mp[x1][y2]--;
        mp[x2][y1]--;
    }
    for (int i = n; i; i--)
        for (int j = m; j; j--)
            mp[i][j] += mp[i + 1][j] + mp[i][j + 1] - mp[i + 1][j + 1];
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) mp[i][j] = mp[i][j] == 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) h[j] = mp[i][j] ? h[j] + 1 : 0;
        st[top = 0] = 0;
        for (int j = 1; j <= m; j++) {
            while (top && h[st[top]] > h[j]) {
                doit(st[top - 1] + 1, st[top], j - 1, h[st[top]]);
                top--;
            }
            st[++top] = j;
        }
        for (int j = 1; j <= top; j++) doit(st[j - 1] + 1, st[j], m, h[st[j]]);
    }
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m + 1; j++) ans[i][j] += ans[i][j - 1];
    for (int i = n; i; i--)
        for (int j = m; j; j--)
            ans[i][j] += ans[i][j + 1] + ans[i + 1][j] - ans[i + 1][j + 1];
    while (q--) {
        int x, y;
        scanf("%d%d", &x, &y);
        printf("%d\n", ans[x][y]);
    }
    return 0;
}
