/*
    Splay维护数列
*/
namespace Splay {
int a[N];  //原数列
int val[N], mn[N], tag[N], size[N], son[N][2], f[N], tot, root;
bool rev[N];
int build(int, int, int);
void Initialize(int n) {
    tot = 0;
    root = build(0, n + 1, 0);
}
void rev1(int x) {
    if (!x) return;
    swap(son[x][0], son[x][1]);
    rev[x] ^= 1;
}
void add1(int x, int p) {
    if (!x) return;
    val[x] += p;
    mn[x] += p;
    tag[x] += p;
}
void pb(int x) {
    if (rev[x]) {
        rev1(son[x][0]);
        rev1(son[x][1]);
        rev[x] = 0;
    }
    if (tag[x]) {
        add1(son[x][0], tag[x]);
        add1(son[x][1], tag[x]);
        tag[x] = 0;
    }
}
void up(int x) {
    size[x] = 1, mn[x] = val[x];
    if (son[x][0]) {
        size[x] += size[son[x][0]];
        if (mn[x] > mn[son[x][0]]) mn[x] = mn[son[x][0]];
        //区间最大值 if(mn[x]<mn[son[x][0]])mn[x]=mn[son[x][0]];
    }
    if (son[x][1]) {
        size[x] += size[son[x][1]];
        if (mn[x] > mn[son[x][1]]) mn[x] = mn[son[x][1]];
        //区间最大值 if(mn[x]<mn[son[x][1]])mn[x]=mn[son[x][1]];
    }
}
void rotate(int x) {
    int y = f[x], w = son[y][1] == x;
    son[y][w] = son[x][w ^ 1];
    if (son[x][w ^ 1]) f[son[x][w ^ 1]] = y;
    if (f[y]) {
        int z = f[y];
        if (son[z][0] == y) son[z][0] = x;
        if (son[z][1] == y) son[z][1] = x;
    }
    f[x] = f[y];
    son[x][w ^ 1] = y;
    f[y] = x;
    up(y);
}
void splay(int x, int w) {
    int s = 1, i = x, y;
    a[1] = x;
    while (f[i]) a[++s] = i = f[i];
    while (s) pb(a[s--]);
    while (f[x] != w) {
        y = f[x];
        if (f[y] != w) {
            if ((son[f[y]][0] == y) ^ (son[y][0] == x))
                rotate(x);
            else
                rotate(y);
        }
        rotate(x);
    }
    if (!w) root = x;
    up(x);
}
// root=build(0,n+1,0);
int build(int l, int r, int fa) {
    int x = ++tot, mid = (l + r) >> 1;
    f[x] = fa;
    val[x] = a[mid];
    if (l < mid) son[x][0] = build(l, mid - 1, x);
    if (r > mid) son[x][1] = build(mid + 1, r, x);
    up(x);
    return x;
}
int kth(int k) {
    int x = root, tmp;
    while (1) {
        pb(x);
        tmp = size[son[x][0]] + 1;
        if (k == tmp) return x;
        if (k < tmp)
            x = son[x][0];
        else
            k -= tmp, x = son[x][1];
    }
}
// [x,y]区间增加z
void ADD(int x, int y, int z) {
    x = kth(x), y = kth(y + 2);
    splay(x, 0), splay(y, x), add1(son[y][0], z);
}
// 翻转区间[x,y]
void RESERVE(int x, int y) {
    x = kth(x), y = kth(y + 2);
    splay(x, 0), splay(y, x), rev1(son[y][0]);
}
// [x,y]向右旋转T个单位
void REVOLVE(int x, int y, int z) {
    z %= y - x + 1;
    if (z) {
        int u = x, t;
        x = kth(y - z + 1), y = kth(y + 2);
        splay(x, 0), splay(y, x), t = son[y][0];
        son[y][0] = 0, up(y), up(x);
        x = kth(u), y = kth(u + 1);
        splay(x, 0), splay(y, x), son[y][0] = t, f[t] = y;
        up(y), up(x);
    }
}
// 删去第x个数字
void DELETE(int x) {
    int y = x;
    x = kth(x), y = kth(y + 2);
    splay(x, 0), splay(y, x), son[y][0] = 0;
    up(y), up(x);
}
// 输出[x,y]中的最小值
void PrintMIN(int x, int y) {
    x = kth(x), y = kth(y + 2);
    splay(x, 0), splay(y, x), printf("%d\n", mn[son[y][0]]);
}
// 在第x个数字后面插入y
void INSERT(int x, int y) {
    x = kth(x + 1);
    splay(x, 0);
    f[++tot] = x, val[tot] = y;
    son[tot][1] = son[x][1], f[son[x][1]] = tot, son[x][1] = tot;
    up(tot), up(x);
}
// 输出第x个位置上的数字
int A(int x) {
    int y = kth(x + 1);
    return val[y];
}
}  // namespace Splay