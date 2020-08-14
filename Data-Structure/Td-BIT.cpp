/*
    二维树状数组
*/
namespace Td_BIT {
int c[M][M];
void Initialize() { memset(c, 0, sizeof(c)); }
void add(int x, int y, int val) {
    for (int i = x; i < M; i += i & -i)
        for (int j = y; j < M; j += j & -j) c[i][j] += val;
}
int query(int x, int y) {
    int res = 0;
    for (int i = x; i; i -= i & -i)
        for (int j = y; j; j -= j & -j) res += c[i][j];
    return res;
}
}  // namespace Td_BIT