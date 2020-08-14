/*
    Matrix-Tree定理
    1、G的度数矩阵D[G]是一个n*n的矩阵，并且满足：当i≠j时,dij=0；
        当i=j时，dij等于vi的度数。
    2、G的邻接矩阵A[G]也是一个n*n的矩阵，
        并且满足：如果vi、vj之间有边直接相连，则aij=1，否则为0。
        我们定义G的Kirchhoff矩阵(也称为拉普拉斯算子)C[G]为C[G]=D[G]-A[G]，
        则Matrix-Tree定理可以描述为：
            G的所有不同的生成树的个数等于
            其Kirchhoff矩阵C[G]任何一个n-1阶余子式的行列式的绝对值。
            所谓n-1阶余子式，就是对于r(1≤r≤n)，
            将C[G]的第r行、第r列同时去掉后得到的新矩阵，用Cr[G]表示。
    Example:
        给出一张格子图，.表示房间，一开始房间之间都有墙壁相连
        求打通一些墙壁使得所有房间唯一路径，求方案数
*/
typedef long long LL;
#define rep(i, n, m) for (int i = n; i <= m; i++)
const LL mod = 1000000000;
int dx[] = {0, 0, 1, -1}, dy[] = {1, -1, 0, 0};
int n, m, id;
char mp[110][110];
LL a[110][110], p[110][110];
int det(int n) {
    LL ans = 1, f = 1;
    rep(i, 1, n) rep(j, 1, n) a[i][j] = (a[i][j] + mod) % mod;
    rep(i, 1, n) {
        rep(j, i + 1, n) {
            LL A = a[i][i], B = a[j][i];
            while (B != 0) {
                LL t = A / B;
                A %= B;
                swap(A, B);
                for (int k = 1; k <= n; k++)
                    a[i][k] = (a[i][k] - t * a[j][k] % mod + mod) % mod;
                for (int k = 1; k <= n; k++) swap(a[i][k], a[j][k]);
                f = -f;
            }
        }
        if (!a[i][i]) return 0;
        ans = ans * a[i][i] % mod;
    }
    if (f == -1) return (mod - ans) % mod;
    return ans;
}
bool check(int x, int y) {
    if (x < 1 || y < 1 || x > n || y > m || mp[x][y] != '.') return 0;
    return 1;
}
int main() {
    scanf("%d%d", &n, &m);
    rep(i, 1, n) scanf("%s", mp[i] + 1);
    rep(i, 1, n) rep(j, 1, m) if (mp[i][j] == '.') p[i][j] = ++id;
    rep(i, 1, n) rep(j, 1, m) if (mp[i][j] == '.') {
        rep(k, 0, 4) {
            int x = i + dx[k], y = j + dy[k];
            if (check(x, y)) {
                int u = p[i][j], v = p[x][y];
                a[u][u]++;
                a[u][v]--;
            }
        }
    }
    printf("%d\n", det(id - 1));  // n-1阶余子式
    return 0;
}