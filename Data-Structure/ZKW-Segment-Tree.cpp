/*
    ZKW线段树
    T:区间和  C:区间数字个数
*/
namespace ZKW_Segment_Tree {
int M, T[MAX_M], C[MAX_M], t, c;
void Initialize(int n) {
    for (M = 1; M < (n + 2); M <<= 1)
        ;
    memset(T, 0, sizeof(T));
    memset(C, 0, sizeof(C));
}
void Add(int x, int y) {
    T[x += M] += y;
    C[x]++;
    for (x /= 2; x; x /= 2) {
        T[x] = T[x << 1] + T[(x << 1) ^ 1];
        C[x] = C[x << 1] + C[(x << 1) ^ 1];
    }
}
void Cal(int x, int y) {
    t = c = 0;
    x += M - 1;
    y += M + 1;
    while (x ^ y ^ 1 > 0) {
        if (~x & 1) t += T[x + 1], c += C[x + 1];
        if (y & 1) t += T[y - 1], c += C[y - 1];
        x >>= 1;
        y >>= 1;
    }
}
}  // namespace ZKW_Segment_Tree
