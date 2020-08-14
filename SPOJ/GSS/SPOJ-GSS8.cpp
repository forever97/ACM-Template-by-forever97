/*
    题目大意：维护一个数列，支持数字的插入，删除和替换，
    以及查询[l,r]中a[i]*(i-l+1)^k的和，maxk=10，答案对2^32取模
    题解：利用splay可以维护数字的插入和删改，接下来考虑如何在splay上维护a[i]*(i-l+1)^k
    设d[x][i]表示子树在k=i时候的答案，我们发现对于左子树来说，
    d[ls][i]是直接累加到d[x][i]上的，同时d[x][i]还要加上(pos-l+1)^k*a[x]，
    我们发现pos-l恰好是左子树的大小，所以子树根贡献为(size[ls]+1)^k*a[x]
    考虑右子树的贡献，对于单个节点x来说，答案是(size[ls[root]]+1+size[ls]+1)^k*a[x]
    对size[ls[root]]+1和size[ls]+1进行二项式拆分，
    发现右子树每个节点拆分后的项可以合并，合并项包括k=1~i的d数组，
    其对根节点x的值d[x][i]的贡献为d[rs][j]*C[i][j]*(size[ls]+1)^(k-j)
    对d数组进行维护即可，考虑到取模数字的特殊性，直接用uint自然溢出
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int INF = 0x3f3f3f3f;
const int N = 200010;
const int K = 10;
typedef unsigned int uint;
uint C[20][20], d[N][K + 5];
namespace Splay {
int a[N];  //原数列
int val[N], size[N], son[N][2], f[N], tot, root;
int build(int, int, int);
void Initialize(int n) {
    tot = 0;
    root = build(0, n + 1, 0);
}
void up(int x) {
    int ls = son[x][0], rs = son[x][1];
    size[x] = size[ls] + size[rs] + 1;
    int sz = size[ls] + 1;
    uint sum = 1;
    for (int i = 0; i <= K; i++) {
        d[x][i] = d[ls][i] + val[x] * sum;
        sum = sum * sz;
    }
    for (int i = 0; i <= K; i++) {
        uint sum = 1;
        for (int j = i; j >= 0; sum = sum * sz, j--) {
            d[x][i] += d[rs][j] * sum * C[i][j];
        }
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
    for (int i = 0; i <= K; i++) d[x][i] = a[mid];
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
    for (int i = 0; i <= K; i++) d[tot][i] = y;
    son[tot][1] = son[x][1], f[son[x][1]] = tot, son[x][1] = tot;
    up(tot), up(x);
}
// 替换第x个数字
void REPLACE(int x, int y) {
    x = kth(x + 1);
    splay(x, 0);
    val[x] = y;
    for (int i = 0; i <= K; i++) d[x][i] = y;
    up(x);
}
// 查询[l,r]中a[i]*(i-l+1)^k的和
void QUERY(int x, int y, int z) {
    x = kth(x), y = kth(y + 2);
    splay(x, 0), splay(y, x);
    printf("%u\n", d[son[y][0]][z]);
}
}  // namespace Splay
using namespace Splay;
void initC() {
    C[0][0] = 1;
    for (int i = 1; i <= K; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++) C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
    }
}
int n, m;
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    initC();
    Initialize(n);
    scanf("%d", &m);
    while (m--) {
        char op[10];
        int x, y, z;
        scanf("%s%d", op, &x);
        x++;
        if (op[0] == 'D')
            DELETE(x);
        else
            scanf("%d", &y);
        if (op[0] == 'I') INSERT(x, y);
        if (op[0] == 'R') REPLACE(x, y);
        if (op[0] == 'Q') {
            y++;
            scanf("%d", &z);
            QUERY(x, y, z);
        }
    }
    return 0;
}
