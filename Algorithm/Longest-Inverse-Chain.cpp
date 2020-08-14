/*
    在有向无环图中
    最长反链=DAG最小路径覆盖（对偶定理）
    链: 点的集合，这个集合中任意两个元素v和u，要么v能走到u，要么u能走到v。
    反链：集合中所有点不能相互抵达
*/
/*
    Example1
    一般有向无环图做法
    floyd求传递闭包，对于每对可达点拆点连边
    最长反链=n-二分图匹配数
*/
bitset<210> mp[210];
int x, y, n, m, Link[210], used[210];
void R(int& a) {
    char ch;
    while (!((ch = getchar()) >= '0') && (ch <= '9'))
        ;
    a = ch - '0';
    while (((ch = getchar()) >= '0') && (ch <= '9')) a *= 10, a += ch - '0';
}
int Dfs(int x) {
    for (int i = 1; i <= n; i++)
        if (mp[x][i] && !used[i]) {
            used[i] = 1;
            if (Link[i] == -1 || Dfs(Link[i])) {
                Link[i] = x;
                return 1;
            }
        }
    return 0;
}
int main() {
    R(n);
    R(m);
    for (int i = 1; i <= m; i++) {
        R(x);
        R(y);
        mp[x][y] = 1;
    }
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (mp[j][i]) mp[j] |= mp[i];
    int res = 0;
    memset(Link, -1, sizeof(Link));
    for (int i = 1; i <= n; i++) {
        memset(used, 0, sizeof(used));
        res += Dfs(i);
    }
    printf("%d\n", n - res);
    return 0;
}
/*
    Example2
    特殊图做法
    网格图只能往下走或者往右走，
    每次经过的格子如果数字大于0，则数字减1
    问最少要从左上角到右下角几次，才能把所有格子变成0
*/
const int N = 1010;
int T, n, m, a[N][N];
long long dp[N][N];
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++) scanf("%d", &a[i][j]);
        for (int i = 1; i <= n; i++) {
            for (int j = m; j; j--)
                dp[i][j] = max(dp[i - 1][j + 1] + a[i][j],
                               max(dp[i - 1][j], dp[i][j + 1]));
        }
        printf("%lld\n", dp[n][1]);
    }
    return 0;
}