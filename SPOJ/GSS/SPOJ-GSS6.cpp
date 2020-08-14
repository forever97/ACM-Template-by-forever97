/*
    题目大意：维护一个数列，支持在x-1和x位置之间插入数字y，删除x位置上的数字，
    改变x位置上的数字以及询问区间最大子段和
    题解：splay维护区间最大连续子段和gss，区间从最左元素开始的最大连续子段和lgss
    区间以最右元素为结尾的最大连续子段rgss和以及区间和s
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int INF = 0x3f3f3f3f;
const int N = 200010;
namespace Splay {
int a[N];  //原数列
int val[N], s[N], lgss[N], rgss[N], gss[N], size[N], son[N][2], f[N], tot, root;
int build(int, int, int);
void Initialize(int n) {
    tot = 0;
    root = build(0, n + 1, 0);
}
void up(int x) {
    int ls = son[x][0], rs = son[x][1];
    size[x] = size[ls] + size[rs] + 1;
    s[x] = s[ls] + s[rs] + val[x];
    lgss[x] = max(lgss[ls], s[ls] + val[x] + max(lgss[rs], 0));
    rgss[x] = max(rgss[rs], s[rs] + val[x] + max(rgss[ls], 0));
    gss[x] = val[x] + max(max(rgss[ls], lgss[rs]), max(0, lgss[rs] + rgss[ls]));
    gss[x] = max(gss[x], max(gss[ls], gss[rs]));
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
        tmp = size[son[x][0]] + 1;
        if (k == tmp) return x;
        if (k < tmp)
            x = son[x][0];
        else
            k -= tmp, x = son[x][1];
    }
}
// 删去第x个数字
void DELETE(int x) {
    int y = x;
    x = kth(x), y = kth(y + 2);
    splay(x, 0), splay(y, x), son[y][0] = 0;
    up(y), up(x);
}
// 在第x-1个数字后面插入y
void INSERT(int x, int y) {
    x = kth(x);
    splay(x, 0);
    f[++tot] = x, val[tot] = y;
    son[tot][1] = son[x][1], f[son[x][1]] = tot, son[x][1] = tot;
    up(tot), up(x);
}
// 替换第x个数字
void REPLACE(int x, int y) {
    x = kth(x + 1);
    splay(x, 0);
    val[x] = y;
    up(x);
}
// 查询区间最大子段和
void QUERY(int x, int y) {
    x = kth(x), y = kth(y + 2);
    splay(x, 0), splay(y, x);
    printf("%d\n", gss[son[y][0]]);
}
}  // namespace Splay
using namespace Splay;
int n, m;
int main() {
    scanf("%d", &n);
    // up里面数组太复杂了，没有判定子节点是否存在，所以要对0节点赋值-INF
    gss[0] = lgss[0] = rgss[0] = -INF;
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    Initialize(n);
    scanf("%d", &m);
    while (m--) {
        char op[10];
        int x, y;
        scanf("%s%d", op, &x);
        if (op[0] == 'D')
            DELETE(x);
        else
            scanf("%d", &y);
        if (op[0] == 'I') INSERT(x, y);
        if (op[0] == 'R') REPLACE(x, y);
        if (op[0] == 'Q') QUERY(x, y);
    }
    return 0;
}
