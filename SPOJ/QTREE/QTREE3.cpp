/*
    题目大意：给出一棵一开始都是白点的树，要求维护操作：
        1.将某个点的颜色取反(黑白互换)
        2.查询1到x的路径上第一个黑色的点
    题解：LCT维护子树中黑点的数量，对于颜色取反，则直接flap，
    对于查询操作通过access(x)将1到x这条链取出，splay上查找即可
    注意是从1到x，所以是makeroot(1)，保证1节点的键值最小
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 100010;
namespace Link_Cut_Tree {
int f[N], son[N][2], val[N], sum[N], tmp[N];
bool rev[N];
void init() {
    memset(f, 0, sizeof(f));
    memset(son, 0, sizeof(son));
    memset(val, 0, sizeof(val));
    memset(rev, 0, sizeof(rev));
    memset(sum, 0, sizeof(sum));
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
void up(int x) { sum[x] = sum[son[x][0]] + sum[son[x][1]] + val[x]; }
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
int flip(int x) {
    makeroot(x);
    val[x] ^= 1;
    up(x);
}
// 查询从1->x的第一个黑点
int ask(int x) {
    makeroot(1);
    access(x);
    splay(x);  // splay只保证了x到根节点标记的下传
    if (!sum[x]) return -1;
    while (x) {
        pb(x);  // 因为有makeroot操作，所以需要下传标记
        if (!sum[son[x][0]] && val[x] == 1)
            return x;
        else if (sum[son[x][0]])
            x = son[x][0];
        else
            x = son[x][1];
    }
}
}  // namespace Link_Cut_Tree
using namespace Link_Cut_Tree;
int n, q, x, y;
int main() {
    while (~scanf("%d%d", &n, &q)) {
        init();
        for (int i = 1; i < n; i++) {
            scanf("%d%d", &x, &y);
            link(x, y);
        }
        while (q--) {
            scanf("%d%d", &x, &y);
            if (x)
                printf("%d\n", ask(y));
            else
                flip(y);
        }
    }
    return 0;
}