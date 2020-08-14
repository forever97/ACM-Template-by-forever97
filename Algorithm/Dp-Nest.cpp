/*
    Dp套Dp
    给出一个矩阵求至少删去一个元素之后的最大子矩阵
*/
const int INF = 0x3f3f3f3f, N = 310;
int n, m, P;
int dp[N][2], s[N][2], a[N][N];
int DP(int x, int y) {
    int res = -INF, st = 0;
    dp[0][1] = dp[0][0] = -INF;
    for (int i = 1; i <= m; i++) {
        dp[i][1] =
            max(max(dp[i - 1][1] + s[i][0], s[i][1]), dp[i - 1][0] + s[i][1]);
        if (dp[i - 1][0] < 0) {
            st = i;
            dp[i][0] = s[i][0];
        } else
            dp[i][0] = dp[i - 1][0] + s[i][0];
        if (x == 1 && y == n && i == m && st == 1)
            res = max(res, dp[i][1]);
        else
            res = max(res, max(dp[i][0], dp[i][1]));
    }
    return res;
}
int Dp(int n, int m) {
    int ans = -INF;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) s[j][1] = -INF, s[j][0] = 0;
        for (int j = i; j <= n; j++) {
            for (int k = 1; k <= m; k++) {
                s[k][1] = max(s[k][1] + a[j][k], s[k][0] + P);
                s[k][0] += a[j][k];
            }
            ans = max(ans, DP(i, j));
        }
    }
    return ans;
}
int main() {
    while (~scanf("%d%d%d", &n, &m, &P)) {
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++) scanf("%d", &a[i][j]);
        printf("%d\n", Dp(n, m));
    }
    return 0;
}