/*
    二维字符串Hash
    找到矩阵中出现至少两次的正方形，输出边长
    改变x和y可以查询矩形
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 1010;
typedef unsigned long long ll;
const ll D1 = 197, D2 = 131;
int n, m, x, y, i, j, ans, t, cnt;
char a[N][N];
ll pow1[N], pow2[N], h[N][N], tmp;
struct P {
    int x, y;
    ll z;
    P() {}
    P(int _x, int _y, ll _z) { x = _x, y = _y, z = _z; }
} q[N * N], fin[N * N];
bool cmp(const P& a, const P& b) { return a.z < b.z; }
bool cmp2(const P& a, const P& b) { return a.x == b.x ? a.y < b.y : a.x < b.x; }
int main() {
    while (~scanf("%d%d", &n, &m)) {
        gets(a[0]);
        for (i = 1; i <= n; i++) gets(a[i] + 1);
        for (pow1[0] = pow2[0] = i = 1; i <= n || i <= m; i++) {
            pow1[i] = pow1[i - 1] * D1, pow2[i] = pow2[i - 1] * D2;
        }
        int Ans = 0;
        int l = 1, r = min(n, m);
        while (l <= r) {
            int mid = (l + r) >> 1;
            ans = cnt = 0;
            x = y = mid;  // x和y为矩形边长
            for (i = 1; i <= n; i++) {
                for (tmp = 0, j = 1; j < y; j++)
                    tmp = tmp * D1 + a[i][j], h[i][j] = 0;
                for (j = y; j <= m; j++)
                    h[i][j] = tmp = tmp * D1 - pow1[y] * a[i][j - y] + a[i][j];
            }
            for (t = 0, i = y; i <= m; i++) {
                for (tmp = 0, j = 1; j < x; j++) tmp = tmp * D2 + h[j][i];
                for (j = x; j <= n; j++)
                    q[++t] =
                        P(j - x + 1, i - y + 1,
                          tmp = tmp * D2 - pow2[x] * h[j - x][i] + h[j][i]);
            }
            for (sort(q + 1, q + t + 1, cmp), j = 1, i = 2; i <= t; i++)
                if (q[i - 1].z != q[i].z) {
                    if (i - j > ans) ans = i - j, tmp = q[j].z;
                    j = i;
                }
            if (t - j + 1 > ans) ans = t - j + 1, tmp = q[t].z;
            for (i = 1; i <= t; i++)
                if (q[i].z == tmp) fin[++cnt] = P(q[i].x, q[i].y, 0);
            if (cnt > 1)
                Ans = mid, l = mid + 1;
            else
                r = mid - 1;
            /*
                注释部分可输出该大小下最大的矩阵和出现位置
            */
            // sort(fin+1,fin+cnt+1,cmp2);
            // printf("%d\n",cnt);
            // for(i=0;i<x;puts(""),i++)for(j=0;j<y;j++)putchar(a[fin[1].x+i][fin[1].y+j]);
            // for(printf("%d\n",cnt),i=1;i<=cnt;i++)printf(" %d
            // %d\n",fin[i].x,fin[i].y);
        }
        printf("%d\n", Ans);
    }
    return 0;
}