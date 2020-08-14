/* 极大团
 * 定义：团是G的一个完全子图，不能被更大的团所包含的团称为极大团
 * 用法：Initialize(n)
 *       Add_Edge()
 *       Dfs(1,0,n,0)
 */
namespace BronKerbosch {
const int N = 200;
int G[N][N], Allow[N][N], Forbid[N][N], Num[N][N], Ans;
void Initialize(int n) {
    Ans = 0;
    for (int i = 1; i <= n; i++) Allow[1][i] = i;
    memset(G, 0, sizeof(G));
}
void Add_Edge(int x, int y) { G[x][y] = G[y][x] = 1; }
void Dfs(int x, int Nn, int An, int Fn) {
    if (!An && !Fn) {
        // Nn为极大团的大小，当两个集合同时为0时表示找到了一个极大团
        Ans = max(Ans, Nn);
        return;
        // 可以用 Ans++ 统计极大团的数量
    }
    if (!An) return;
    int key = Allow[x][1];
    for (int j = 1; j <= An; j++) {
        int v = Allow[x][j], tAn = 0, tFn = 0;
        if (G[key][v]) continue;
        for (int i = 1; i <= Nn; i++) Num[x + 1][i] = Num[x][1];
        Num[x + 1][Nn + 1] = v;
        for (int i = 1; i <= An; i++)
            if (G[v][Allow[x][i]]) Allow[x + 1][++tAn] = Allow[x][i];
        for (int i = 1; i <= Fn; i++)
            if (G[v][Forbid[x][i]]) Forbid[x + 1][++tFn] = Forbid[x][i];
        Dfs(x + 1, Nn + 1, tAn, tFn);
        Allow[x][j] = 0;
        Forbid[x][++Fn] = v;
    }
}
}  // namespace BronKerbosch