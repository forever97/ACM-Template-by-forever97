/*
    动态树(链修改)
    初始 for(int i=1;i<=n;i++)sum[i]=val[i],mt[i]=size[i]=1
*/
namespace Link_Cut_Tree {
int f[N], son[N][2], tmp[N], size[N];
bool rev[N];
LL val[N], sum[N], at[N], mt[N];
void Initialize() {
    memset(f, 0, sizeof(f));
    memset(son, 0, sizeof(son));
    memset(val, 0, sizeof(val));
    memset(rev, 0, sizeof(rev));
    memset(sum, 0, sizeof(sum));
    memset(at, 0, sizeof(at));
    memset(mt, 0, sizeof(mt));
}
void modify(int x, int m, int a) {
    if (!x) return;
    val[x] = (val[x] * m + a) % mod;
    sum[x] = (sum[x] * m + a * size[x]) % mod;
    at[x] = (at[x] * m + a) % mod;
    mt[x] = (mt[x] * m) % mod;
}
bool isroot(int x) { return !f[x] || son[f[x]][0] != x && son[f[x]][1] != x; }
void rev1(int x) {
    if (!x) return;
    swap(son[x][0], son[x][1]);
    rev[x] ^= 1;
}
void pb(int x) {
    if (rev[x]) rev1(son[x][0]), rev1(son[x][1]), rev[x] = 0;
    int m = mt[x], a = at[x];
    mt[x] = 1;
    at[x] = 0;
    if (m != 1 || a != 0) {
        modify(son[x][0], m, a);
        modify(son[x][1], m, a);
    }
}
void up(int x) {
    sum[x] = (val[x] + sum[son[x][0]] + sum[son[x][1]]) % mod;
    size[x] = (size[son[x][0]] + size[son[x][1]] + 1) % mod;
}
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
// 查询x所在的树的根
int root(int x) {
    access(x);
    splay(x);
    while (son[x][0]) x = son[x][0];
    return x;
}
// 使x成为根
void makeroot(int x) {
    access(x);
    splay(x);
    rev1(x);
}
// 将x和y所属树合并
void link(int x, int y) {
    makeroot(x);
    f[x] = y;
    access(x);
}
// 将x和其父节点分开
void cutf(int x) {
    access(x);
    splay(x);
    f[son[x][0]] = 0;
    son[x][0] = 0;
    up(x);
}
// 将边x-y切断
void cut(int x, int y) {
    makeroot(x);
    cutf(y);
}
// 提取链
void split(int x, int y) {
    makeroot(y);
    access(x);
    splay(x);
}
// 查询x到y的链和，注意考虑清楚是int还是LL
int ask(int x, int y) {
    split(x, y);
    return sum[x];
}
// 查询节点到根的距离，注意考虑清楚是int还是LL
int query(int x) {
    access(x);
    splay(x);
    return sum[x];
}
// 将x为下标的值改为y
int change(int x, int y) {
    makeroot(x);
    val[x] = y;
    up(x);
}
// 将x的父亲改为y
int changef(int x, int y) {
    cutf(x);
    f[x] = y;
}
// 链上点权加法
void add(int x, int y, int z) {
    split(x, y);
    modify(x, 1, z);
}
// 链上点权乘法
void mul(int x, int y, int z) {
    split(x, y);
    modify(x, z, 0);
}
}  // namespace Link_Cut_Tree