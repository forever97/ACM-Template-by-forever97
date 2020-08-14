/*
    题目大意：给出一棵边权树，要求实现树链求和以及求链上第k个元素
    题解：边权转点权后用LCT维护，注意求Qkth和sum时区别于点权树的讨论
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
const int N = 300010;
namespace Link_Cut_Tree {
vector<int> v[N], w[N];
int f[N], son[N][2], tmp[N], size[N], val[N], sum[N];
void init() {
    memset(val, 0, sizeof(val));
    memset(sum, 0, sizeof(sum));
    memset(f, 0, sizeof(f));
    memset(son, 0, sizeof(son));
    for (int i = 1; i < N; i++) size[i] = 1, v[i].clear(), w[i].clear();
}
bool isroot(int x) { return !f[x] || son[f[x]][0] != x && son[f[x]][1] != x; }
void up(int x) {
    sum[x] = val[x] + sum[son[x][0]] + sum[son[x][1]];
    size[x] = size[son[x][0]] + size[son[x][1]] + 1;
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
// 求边权树的链和
int ask(int x, int y) {
    access(y), y = 0;
    int ans = 0;
    while (x) {
        splay(x);
        if (!f[x]) return sum[son[x][1]] + sum[y];
        son[x][1] = y;
        up(x);
        x = f[y = x];
    }
}
void dfs(int x, int fx) {
    for (int i = 0; i < v[x].size(); i++) {
        int y = v[x][i];
        if (y == fx) continue;
        f[y] = x;
        val[y] = w[x][i];
        dfs(y, x);
    }
    up(x);
}
int kth(int x, int k) {
    while (size[son[x][0]] + 1 != k) {
        if (k < size[son[x][0]] + 1)
            x = son[x][0];
        else {
            k -= size[son[x][0]] + 1;
            x = son[x][1];
        }
    }
    return x;
}
// 求边权树u->v的第k个点
int Qkth(int u, int v, int k) {
    access(u), u = 0;
    while (v) {
        splay(v);
        if (!f[v]) {
            if (size[son[v][1]] + 1 == k)
                return v;
            else if (size[son[v][1]] + 1 > k)
                return kth(son[v][1], size[son[v][1]] + 1 - k);
            else
                return kth(u, k - (size[son[v][1]] + 1));
        }
        son[v][1] = u;
        up(v);
        v = f[u = v];
    }
}
}  // namespace Link_Cut_Tree
using namespace Link_Cut_Tree;
int T, x, y, z, k, n;
char op[10];
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        init();
        for (int i = 1; i < n; i++) {
            scanf("%d%d%d", &x, &y, &z);
            v[x].push_back(y);
            w[x].push_back(z);
            v[y].push_back(x);
            w[y].push_back(z);
        }
        dfs(1, 0);
        while (scanf("%s", op)) {
            if (op[1] == 'O') break;
            if (op[0] == 'D') {
                scanf("%d%d", &x, &y);
                printf("%d\n", ask(x, y));
            } else {
                scanf("%d%d%d", &x, &y, &k);
                printf("%d\n", Qkth(x, y, k));
            }
        }
    }
    return 0;
}
