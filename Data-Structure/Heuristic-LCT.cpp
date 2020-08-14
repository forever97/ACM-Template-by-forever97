/*
    启发式合并LCT
    加边，查询每个连通块的重心到其它点的距离和的和
*/
namespace Heuristic_LCT {
int ans, g[N], v[N << 1], nxt[N << 1], ed;
int f[N], son[N][2], val[N], tag[N], sum[N], ts[N], td[N], size[N], tmp[N];
bool isroot(int x) { return !f[x] || son[f[x]][0] != x && son[f[x]][1] != x; }
void add1(int x, int p) {
    if (!x) return;
    val[x] += p;
    tag[x] += p;
}
void add2(int x, int s, int d) {
    if (!x) return;
    sum[x] += s + size[son[x][1]] * d;
    ts[x] += s;
    td[x] += d;
}
void pb(int x) {
    if (tag[x]) {
        add1(son[x][0], tag[x]);
        add1(son[x][1], tag[x]);
        tag[x] = 0;
    }
    if (td[x]) {
        add2(son[x][0], ts[x] + (size[son[x][1]] + 1) * td[x], td[x]);
        add2(son[x][1], ts[x], td[x]);
        ts[x] = td[x] = 0;
    }
}
void up(int x) { size[x] = size[son[x][0]] + size[son[x][1]] + 1; }
void rotate(int x) {
    int y = f[x], w = son[y][1] == x;
    son[y][w] = son[x][w ^ 1];
    if (son[x][w ^ 1]) f[son[x][w ^ 1]] = y;
    if (f[y]) {
        int z = f[y];
        if (son[z][0] == y)
            son[z][0] = x;
        else if (son[z][1] == y)
            son[z][1] = x;
    }
    f[x] = f[y];
    f[y] = x;
    son[x][w ^ 1] = y;
    up(y);
}
void splay(int x) {
    int s = 1, i = x, y;
    tmp[1] = i;
    while (!isroot(i)) tmp[++s] = i = f[i];
    while (s) pb(tmp[s--]);
    while (!isroot(x)) {
        y = f[x];
        if (!isroot(y)) {
            if ((son[f[y]][0] == y) ^ (son[y][0] == x))
                rotate(x);
            else
                rotate(y);
        }
        rotate(x);
    }
    up(x);
}
void access(int x) {
    for (int y = 0; x; y = x, x = f[x]) splay(x), son[x][1] = y, up(x);
}
int root(int x) {
    access(x);
    splay(x);
    while (son[x][0]) x = son[x][0];
    return x;
}
void addleaf(int x, int y) {
    f[y] = x,
    son[y][0] = son[y][1] = val[y] = tag[y] = sum[y] = ts[y] = td[y] = 0,
    size[y] = 1;
    x = root(x), access(y), splay(x), add1(x, 1), add2(x, 0, 1);
    for (y = son[x][1]; son[y][0]; y = son[y][0])
        ;
    splay(y);
    int vx = val[x], vy = val[y];
    if (vy * 2 > vx) {
        val[y] = vx, val[x] -= vy;
        sum[x] -= sum[y] + vy, sum[y] += sum[x] + vx - vy;
        access(y), splay(x), son[x][0] = y, son[x][1] = 0;
    }
}
void dfs(int x, int y) {
    addleaf(y, x);
    for (int i = g[x]; i; i = nxt[i])
        if (v[i] != y) dfs(v[i], x);
}
void addedge(int x, int y) {
    v[++ed] = y;
    nxt[ed] = g[x];
    g[x] = ed;
}
void link(int x, int y) {
    int X = root(x), Y = root(y);
    ans -= sum[X] + sum[Y];
    if (val[X] < val[Y]) swap(x, y);
    dfs(y, x), addedge(x, y), addedge(y, x);
    ans += sum[root(x)];
}
void Initialize() {
    ans = 0;
    memset(f, 0, sizeof(f));
    memset(son, 0, sizeof(son));
    memset(ts, 0, sizeof(ts));
    memset(td, 0, sizeof(td));
    memset(tag, 0, sizeof(tag));
}
}  // namespace Heuristic_LCT
int n, m, x, y;
char op[5];
int main() {
    using namespace Heuristic_LCT;
    Initialize();
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) val[i] = size[i] = 1;
    while (m--) {
        scanf("%s", op);
        if (op[0] == 'A') scanf("%d%d", &x, &y), link(x, y);
        if (op[0] == 'Q') printf("%d\n", ans);
    }
    return 0;
}