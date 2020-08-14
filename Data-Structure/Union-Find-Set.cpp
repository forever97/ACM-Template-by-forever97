/*
    按秩合并并查集
    连通块数量为block，大小为size。
*/
namespace Union_Find_Set {
int f[M], size[M], block;
void Initialize() {
    memset(f, 0, sizeof(f));
    block = n;
}
int Find(int x) {
    if (!f[x]) f[x] = x, size[x] = 1;
    if (f[x] == x) return x;
    return f[x] = Find(f[x]);
}
void Union(int x, int y) {
    x = Find(x);
    y = Find(y);
    if (x == y) return;
    if (size[x] > size[y]) swap(x, y);
    f[x] = y;
    size[y] += size[x];
    block--;
}
}  // namespace Union_Find_Set
/*
    加权并查集
*/
int sf(int x) {
    if (f[x] == x)
        return x;
    else {
        int fx = sf(f[x]);
        r[x] = (r[x] + r[f[x]]) % 2;
        return f[x] = fx;
    }
}
void U(int x, int y, int fx, int fy, int d) {
    f[fy] = fx;
    r[fy] = (2 - r[y] + d + r[x]) % 2;
}
/*
    食物链(拆点)
    草原上有三种物种，分别为A,B,C
    A吃B，B吃C，C吃A。
    1 x y表示x和y是同类，2 x y表示x吃y
    问给出的信息有几条是和前面相违背的
*/
#include <cstdio>
const int N = 50010 * 3;
int f[N], n, k, ans = 0, op, x, y;
void init(int n) {
    for (int i = 0; i <= n; i++) f[i] = i;
}
int sf(int x) { return f[x] == x ? x : f[x] = sf(f[x]); }
bool Same(int x, int y) { return sf(x) == sf(y); }
bool Union(int x, int y) { f[sf(x)] = sf(y); }
int main() {
    scanf("%d%d", &n, &k);
    init(3 * n);
    for (int i = 1; i <= k; i++) {
        scanf("%d%d%d", &op, &x, &y);
        if (--x < 0 || x >= n || --y < 0 || y >= n) {
            ans++;
            continue;
        }
        if (op == 1) {
            if (Same(x, y + n) || Same(x, y + 2 * n))
                ans++;
            else {
                Union(x, y);
                Union(x + n, y + n);
                Union(x + 2 * n, y + 2 * n);
            }
        } else {
            if (Same(x, y) || Same(x, y + 2 * n))
                ans++;
            else {
                Union(x, y + n);
                Union(x + n, y + 2 * n);
                Union(x + 2 * n, y);
            }
        }
    }
    printf("%d\n", ans);
    return 0;
}
