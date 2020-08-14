/*
    最长公共子串
    dp[i][j]为i位置和j位置匹配往前的最长公共子串
*/
void LCS(char *a, char *b, int n, int m, int dp[][N]) {
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = 0;
        }
}
/*
    最短唯一子串
    字符串自匹配最长公共子串
    加一即为每个位置往前的最短唯一子串
*/
void GetMin(int n, int dp[][N], int mn[]) {
    for (int i = 1; i <= n; i++) {
        mn[i] = 0;
        for (int j = 1; j <= n; j++) {
            if (i == j) continue;
            mn[i] = max(mn[i], dp[i][j]);
        }
        mn[i]++;
    }
}
/*
    Example
    求出两个串的最短公共子串，使得该串在两个串中唯一
*/
const int N = 5005;
int dpa[N][N], dpb[N][N], dpab[N][N], la, lb, mna[N], mnb[N];
char a[N], b[N];
int main() {
    scanf(" %s %s", a, b);
    la = strlen(a);
    lb = strlen(b);
    LCS(a, a, la, la, dpa);
    LCS(b, b, lb, lb, dpb);
    GetMin(la, dpa, mna);
    GetMin(lb, dpb, mnb);
    LCS(a, b, la, lb, dpab);
    int ans = la + 1;
    for (int i = 1; i <= la; i++) {
        for (int j = 1; j <= lb; j++) {
            int t = max(mna[i], mnb[j]);
            if (dpab[i][j] >= t) {
                ans = min(ans, t);
            }
        }
    }
    if (ans > la) ans = -1;
    printf("%d\n", ans);
}