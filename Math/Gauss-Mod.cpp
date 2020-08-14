/*
    模意义下的高斯消元
*/
namespace Gauss {
const int N = 110, MOD = 2, INF = 1e9;
int a[N][N], ans[N];
bool isFreeX[N];
int inv(int a, int m) { return (a == 1 ? 1 : inv(m % a, m) * (m - m / a) % m); }
int getAns(int n, int m, int r) {
    int res = 0;
    for (int i = r - 1; ~i; i--) {
        for (int j = 0; j < m; j++) {
            if (!a[i][j]) continue;
            ans[j] = a[i][m];
            for (int k = j + 1; k < m; k++) {
                ans[j] -= a[i][k] * ans[k];
                ans[j] %= MOD;
                if (ans[j] < 0) ans[j] += MOD;
            }
            ans[j] = ans[j] * inv(a[i][j], MOD) % MOD;
            break;
        }
    }
    for (int i = 0; i < m; i++) res += ans[i];
    return res;
}
/*
    返回值为方程的阶，即非自由变元数量
    无解返回-1
*/
int gauss(int n, int m) {
    for (int i = 0; i < m; i++) isFreeX[i] = 0;
    int r = 0, c = 0;
    for (; r < n && c < m; r++, c++) {
        int maxR = r;
        for (int i = r + 1; i < n; i++)
            if (abs(a[i][c]) > abs(a[maxR][c])) maxR = i;
        if (maxR != r) swap(a[maxR], a[r]);
        if (!a[r][c]) {
            r--;
            isFreeX[c] = 1;
            continue;
        }
        for (int i = r + 1; i < n; i++) {
            if (a[i][c]) {
                int delta = a[i][c] * inv(a[r][c], MOD);
                for (int j = c; j <= m; j++) {
                    a[i][j] -= delta * a[r][j];
                    a[i][j] %= MOD;
                    if (a[i][j] < 0) a[i][j] += MOD;
                }
            }
        }
    }
    for (int i = r; i < n; i++)
        if (a[i][m]) return -1;
    return r;
}
// 模2枚举自由变元
int getMinAns(int n, int m, int r) {
    int res = INF, freeX = m - r;
    for (int s = 0; s < 1 << freeX; s++) {
        if (__builtin_popcount(s) >= res) continue;
        int cnt = 0;
        for (int j = 0; j < m; j++) {
            if (isFreeX[j]) {
                ans[j] = s >> cnt & 1;
                ++cnt;
            }
        }
        res = min(res, getAns(n, m, r));
    }
    return res;
}
}  // namespace Gauss
/*
    Test
    给出灯的相连关系，相邻的灯开关会相互影响
    求把灯全部打开的最少开关数
*/
int n, x, y;
int main() {
    while (~scanf("%d", &n), n) {
        using namespace Gauss;
        memset(a, 0, sizeof(a));
        for (int i = 1; i < n; i++) {
            scanf("%d%d", &x, &y);
            a[x - 1][y - 1] = 1;
            a[y - 1][x - 1] = 1;
        }
        for (int i = 0; i < n; i++) a[i][i] = a[i][n] = 1;
        int r = gauss(n, n);
        printf("%d\n", getMinAns(n, n, r));
    }
    return 0;
}