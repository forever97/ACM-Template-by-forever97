/*
    动态树(点修改)
    初始化的时候，读入val[i]，sum[i]=xor[i]=val[i]
*/
namespace Link_Cut_Tree {
int f[N], son[N][2], val[N], sum[N], tmp[N], Xor[N];
bool rev[N];
void Initialize() {
    memset(f, 0, sizeof(f));
    memset(son, 0, sizeof(son));
    memset(val, 0, sizeof(val));
    memset(rev, 0, sizeof(rev));
    memset(sum, 0, sizeof(sum));
    memset(Xor, 0, sizeof(Xor));
}
bool isroot(int x) { return !f[x] || son[f[x]][0] != x && son[f[x]][1] != x; }
void rev1(int x) {
    if (!x) return;
    swap(son[x][0], son[x][1]);
    rev[x] ^= 1;
}
void pb(int x) {
    if (rev[x]) rev1(son[x][0]), rev1(son[x][1]), rev[x] = 0;
}
void up(int x) {
    sum[x] = Xor[x] = val[x];
    if (son[x][0]) sum[x] += sum[son[x][0]];
    if (son[x][1]) sum[x] += sum[son[x][1]];
    if (son[x][0]) Xor[x] ^= Xor[son[x][0]];
    if (son[x][1]) Xor[x] ^= Xor[son[x][1]];
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
// 查询x到y的链和
int ask(int x, int y) {
    makeroot(x);
    access(y);
    splay(y);
    return sum[y];
}
// 计算x到y的xor和
int xorsum(int x, int y) {
    makeroot(x);
    access(y);
    splay(y);
    return Xor[y];
}
// 查询节点到根的距离
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
}  // namespace Link_Cut_Tree