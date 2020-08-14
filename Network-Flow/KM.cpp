/*
    KM算法
    复杂度O(nx*nx*ny)
    求最大权匹配
    若求最小权匹配，可将权值取相反数，结果取相反数
    点的编号从0开始
*/
const int N = 310, INF = 0x3f3f3f3f;
namespace KM {
int nx, ny;                   //两边的点数
int g[N][N];                  //二分图描述
int linker[N], lx[N], ly[N];  // y中各点匹配状态，x,y中的点标号
int n, slack[N];
bool visx[N], visy[N];
bool DFS(int x) {
    visx[x] = 1;
    for (int y = 0; y < ny; y++) {
        if (visy[y]) continue;
        int tmp = lx[x] + ly[y] - g[x][y];
        if (tmp == 0) {
            visy[y] = true;
            if (linker[y] == -1 || DFS(linker[y])) {
                linker[y] = x;
                return 1;
            }
        } else if (slack[y] > tmp)
            slack[y] = tmp;
    }
    return 0;
}
int KM() {
    memset(linker, -1, sizeof(linker));
    memset(ly, 0, sizeof(ly));
    for (int i = 0; i < nx; i++) {
        lx[i] = -INF;
        for (int j = 0; j < ny; j++)
            if (g[i][j] > lx[i]) lx[i] = g[i][j];
    }
    for (int x = 0; x < nx; x++) {
        for (int i = 0; i < ny; i++) slack[i] = INF;
        for (;;) {
            memset(visx, false, sizeof(visx));
            memset(visy, false, sizeof(visy));
            if (DFS(x)) break;
            int d = INF;
            for (int i = 0; i < ny; i++)
                if (!visy[i] && d > slack[i]) d = slack[i];
            for (int i = 0; i < nx; i++)
                if (visx[i]) lx[i] -= d;
            for (int i = 0; i < ny; i++) {
                if (visy[i])
                    ly[i] += d;
                else
                    slack[i] -= d;
            }
        }
    }
    int res = 0;
    for (int i = 0; i < ny; i++)
        if (linker[i] != -1) res += g[linker[i]][i];
    return res;
}
}  // namespace KM
// Test
int main() {
    using namespace KM;
    while (~scanf("%d", &n)) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) scanf("%d", &g[i][j]);
        nx = ny = n;
        printf("%d\n", KM());
    }
    return 0;
}