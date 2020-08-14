/*
    kmp+矩阵加速
    求长度为n的不包含长度为m的子串的串个数
*/
#define rep(i, n) for (int i = 0; i < n; i++)
using namespace std;
int t, n, m, mod, a[25][25], b[25][25], tmp[25][25], p[25];
char s[25];
void mul(int a[25][25], int b[25][25], int ans[25][25]) {
    rep(i, m) rep(j, m) {
        tmp[i][j] = 0;
        rep(k, m) tmp[i][j] = (tmp[i][j] + a[i][k] * b[k][j]) % mod;
    }
    rep(i, m) rep(j, m) ans[i][j] = tmp[i][j];
}
int main() {
    scanf("%d%d%d", &n, &m, &mod);
    scanf("%s", s + 1);
    int j = 0;
    for (int i = 2; i <= m; i++) {
        while (j > 0 && s[j + 1] != s[i]) j = p[j];
        if (s[j + 1] == s[i]) j++;
        p[i] = j;
    }
    rep(i, m) rep(j, 10) {
        for (t = i; t && s[t + 1] - '0' != j; t = p[t])
            ;
        if (s[t + 1] - '0' == j) t++;
        if (t != m) b[i][t] = (b[i][t] + 1) % mod;
    }
    rep(i, m) a[i][i] = 1;
    int sum = 0;
    while (n) {
        if (n & 1) mul(a, b, a);
        mul(b, b, b);
        n >>= 1;
    }
    for (int i = 0; i < m; i++) sum = (sum + a[0][i]) % mod;
    return printf("%d\n", sum), 0;
}