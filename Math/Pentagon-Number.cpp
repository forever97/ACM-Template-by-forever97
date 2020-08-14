/*
    五边形数
    整数划分方案数
*/
namespace Pentagon_Number {
const int N = 731, P = mod;
int i, j, f[N + 1], g[200010];
void init(int n) {
    for (f[1] = 1, f[2] = 2, f[3] = 5, f[4] = 7, i = 5; i < N; i++)
        f[i] = 3 + 2 * f[i - 2] - f[i - 4];
    for (g[0] = i = 1; i <= n; i++)
        for (j = 1; f[j] <= i; j++)
            if ((j + 1) >> 1 & 1)
                g[i] = (g[i] + g[i - f[j]]) % P;
            else
                g[i] = (g[i] - g[i - f[j]] + P) % P;
}
}  // namespace Pentagon_Number