/*
    题目大意：维护一棵树，允许链上修改，询问链上最大连续子链和
    题解：我们用动态树维护操作，对于链上修改，我们提取链后打标记即可，
    考虑到标记修改值绝对值在10000以内，我们用0x3f来标记累加值，
    维护区间最长连续子段和gss，区间从最左元素开始的最大连续子段和lgss
    区间以最右元素为结尾的最大连续子段和rgss以及区间和s，
    当标记传递时，lgss和rgss以及gss将会变为0或者区间长度乘新值，s正常维护即可。
    需要注意的是，当发生区间翻转的时候，因为左右子树的交换，lgss和rgss的值也要发生交换，
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int INF = 0x3f3f3f3f;
const int N = 200010;
namespace Link_Cut_Tree {
int f[N], son[N][2], tmp[N], size[N], tag[N], val[N];
bool rev[N];
int s[N], lgss[N], rgss[N], gss[N];
bool isroot(int x) { return !f[x] || son[f[x]][0] != x && son[f[x]][1] != x; }
void rev1(int x) {
    if (!x) return;
    swap(son[x][0], son[x][1]);
    // 子树旋转代表区间的左右最长连续子段和也发生交换
    swap(lgss[x], rgss[x]);
    rev[x] ^= 1;
}
void addtag(int x, int y) {
    if (!x) return;
    lgss[x] = rgss[x] = gss[x] = y > 0 ? y * size[x] : 0;
    s[x] = y * size[x];
    tag[x] = val[x] = y;
}
void pb(int x) {
    if (rev[x]) rev1(son[x][0]), rev1(son[x][1]), rev[x] = 0;
    if (tag[x] ^ INF)
        addtag(son[x][0], tag[x]), addtag(son[x][1], tag[x]), tag[x] = INF;
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
void makeroot(int x) {
    access(x);
    splay(x);
    rev1(x);
}
void link(int x, int y) {
    makeroot(x);
    f[x] = y;
    access(x);
}
void split(int x, int y) {
    makeroot(y);
    access(x);
    splay(x);
}
}  // namespace Link_Cut_Tree
int n, m, op, x, y, z;
using namespace Link_Cut_Tree;
int main() {
    while (~scanf("%d", &n)) {
        for (int i = 1; i <= n; i++) {
            scanf("%d", &val[i]);
            rev[i] = son[i][0] = son[i][1] = f[i] = 0;
            size[i] = 1;
            tag[i] = INF;
            lgss[i] = rgss[i] = gss[i] = val[i] > 0 ? val[i] : 0;
            s[i] = val[i];
        }
        for (int i = 1; i < n; i++) {
            int x, y;
            scanf("%d%d", &x, &y);
            link(x, y);
        }
        scanf("%d", &m);
        while (m--) {
            scanf("%d%d%d", &op, &x, &y);
            split(x, y);
            if (op == 1)
                printf("%d\n", gss[x]);
            else {
                scanf("%d", &z);
                rgss[x] = lgss[x] = gss[x] = z > 0 ? z * size[x] : 0;
                val[x] = tag[x] = z;
                s[x] = z * size[x];
            }
        }
    }
    return 0;
}
