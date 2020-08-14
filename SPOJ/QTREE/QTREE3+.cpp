/*
    题目大意：给出一棵一开始都是白点的树，要求维护操作：
        1.将某个点的颜色取反(黑白互换)
        2.查询1到x的路径上第一个黑色的点
    题解：LCT维护子树中黑点的数量，对于颜色取反，则直接flap，
    对于查询操作通过access(x)将1到x这条链取出，splay上查找即可
    考虑根为1不会变动，因此直接dfs建树，效率更高
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
const int N = 100010;
namespace Link_Cut_Tree {
int f[N], son[N][2], val[N], sum[N], tmp[N];
vector<int> v[N];
void init() {
    memset(f, 0, sizeof(f));
    memset(son, 0, sizeof(son));
    memset(val, 0, sizeof(val));
    memset(sum, 0, sizeof(sum));
}
bool isroot(int x) { return !f[x] || son[f[x]][0] != x && son[f[x]][1] != x; }
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
void dfs(int x, int fx) {
    for (int i = 0; i < v[x].size(); i++) {
        int y = v[x][i];
        if (y == fx) continue;
        f[y] = x;
        dfs(y, x);
    }
    up(x);
}
int flip(int x) {
    splay(x);
    val[x] ^= 1;
    up(x);
}
int ask(int x) {
    access(x);
    splay(x);
    if (!sum[x]) return -1;
    while (x) {
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
        for (int i = 1; i <= n; i++) v[i].clear();
        for (int i = 1; i < n; i++) {
            scanf("%d%d", &x, &y);
            v[x].push_back(y);
            v[y].push_back(x);
        }
        dfs(1, 0);
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
