// 统计每个连通分量都是完全图的方案数
#include <cstdio>
const int N = 731, P = 999999599;
int n, m, i, j, f[731], g[200005];
int main() {
    for (f[1] = 1, f[2] = 2, f[3] = 5, f[4] = 7, i = 5; i < N; i++)
        f[i] = 3 + 2 * f[i - 2] - f[i - 4];
    for (scanf("%d%d", &n, &m), g[0] = i = 1; i <= n; i++) {
        for (j = 1; f[j] <= i; j++) {
            if ((j + 1) >> 1 & 1)
                g[i] = (g[i] + g[i - f[j]]) % (P - 1);
            else
                g[i] = (g[i] - g[i - f[j]]) % (P - 1);
        }
    }
    for (i = (g[n] + P - 1) % (P - 1), j = 1; i; i >>= 1, m = 1LL * m * m % P)
        if (i & 1) j = 1LL * j * m % P;
    return printf("%d", j), 0;
}