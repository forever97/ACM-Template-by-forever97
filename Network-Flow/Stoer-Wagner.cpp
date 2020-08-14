/*
    全局最小割
    -邻接矩阵-
*/
namespace Stoer_Wagner() {
    const int INF = 0x3f3f3f3f;
    const int MAX_N = 510;
    int N, M;
    int G[MAX_N][MAX_N], v[MAX_N], f[MAX_N], vis[MAX_N];
    int Stoer_Wagner() {
        int ret = INF;
        for (int i = 1; i <= N; i++) v[i] = i;
        while (N > 1) {
            int k, lst = v[1];
            memset(vis, 0, sizeof(vis));
            memset(f, 0, sizeof(f));
            vis[v[1]] = 1;
            for (int i = 2; i <= N; i++) {
                k = 1;
                for (int j = 2; j <= N; j++)
                    if (!vis[v[j]] && (f[v[j]] += G[lst][v[j]]) > f[v[k]])
                        k = j;
                vis[v[k]] = 1;
                if (i < N) lst = v[k];
            }
            ret = min(ret, f[v[k]]);
            for (int j = 1; j <= N; j++)
                G[v[j]][lst] = G[lst][v[j]] = G[lst][v[j]] + G[v[k]][v[j]];
            v[k] = v[N--];
        }
        return ret;
    }
    void Initialize() {
        memset(G, 0, sizeof(G));
        for (int i = 1; i <= M; i++) {
            int x, y, z;
            scanf("%d%d%d", &x, &y, &z);
            ++x;
            ++y;
            G[x][y] += z;
            G[y][x] += z;
        }
    }
}  // namespace Stoer_Wagner()