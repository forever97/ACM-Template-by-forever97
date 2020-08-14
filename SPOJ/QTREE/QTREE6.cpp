/*
    题目大意：给出一棵树，一开始树上点均为黑色，要求维护操作
        1.翻转某点的颜色(黑白互换)，
        2.查询某个点所在连通块(颜色相同则连通)大小
    题解：我们分别维护黑点LCT和白点LCT，当一个点从黑变白时，
    将其从黑点LCT中与父节点断开，然后在白点LCT中与父节点相连，
    这样我们就保证了每个连通块在LCT中只有父节点是不同色的而其余一定是连通的。
    考虑用LCT维护子树信息，根据在LCT上的连接情况，我们将点的儿子分为实儿子和虚儿子
    实儿子是原本树结构上相连的点，实儿子的子树为实子树，虚儿子的子树为虚子树
    x的LCT子树的信息和等于x的实儿子的LCT子树信息和加上x的虚子树的信息和加上x自己
    在进行access操作时，我们会有更换一个点的x右儿子的操作，
    这时我们要把x原来的右儿子的LCT子树信息加入x的虚子树信息，
    把x的新的右儿子的LCT子树信息从x的虚子树信息中减去
    同时在link x到y的时候，我们需要对y进行access再splay
    这样就只会对y的虚子树信息和LCT子树信息产生影响，而不会影响到y的祖先节点
*/
#include <algorithm>
#include <cstdio>
#include <set>
const int N = 100010;
using namespace std;
int n, m, i, k, x, y, c[N], fa[N], g[N], v[N << 1], nxt[N << 1], ed;
struct LCT {
    int son[N][2], f[N], sum[N], s[N];
    bool isroot(int x) {
        return !f[x] || son[f[x]][0] != x && son[f[x]][1] != x;
    }
    void up(int x) { sum[x] = 1 + sum[son[x][0]] + sum[son[x][1]] + s[x]; }
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
        f[y] = x;
        son[x][w ^ 1] = y;
        up(y);
    }
    void splay(int x) {
        while (!isroot(x)) {
            int y = f[x];
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
        for (int y = 0; x; y = x, x = f[x]) {
            splay(x);
            if (son[x][1]) s[x] += sum[son[x][1]];
            if (son[x][1] = y) s[x] -= sum[y];
            up(x);
        }
    }
    int root(int x) {
        access(x);
        splay(x);
        while (son[x][0]) x = son[x][0];
        return x;
    }
    void link(int x) {
        access(fa[x]);
        splay(fa[x]);
        splay(x);
        son[fa[x]][1] = x;
        up(f[x] = fa[x]);
    }
    void cut(int x) {
        access(x);
        splay(x);
        f[son[x][0]] = 0;
        son[x][0] = 0;
        up(x);
    }
    int ask(int x) {
        splay(x = root(x));
        return sum[son[x][1]];
    }
} T[2];
void add(int x, int y) {
    v[++ed] = y;
    nxt[ed] = g[x];
    g[x] = ed;
}
void dfs(int x) {
    for (int i = g[x]; i; i = nxt[i])
        if (v[i] != fa[x]) {
            fa[v[i]] = T[c[v[i]]].f[v[i]] = x;
            dfs(v[i]);
            T[c[v[i]]].s[x] += T[c[v[i]]].sum[v[i]];
        }
    T[0].up(x), T[1].up(x);
}
void read(int& a) {
    char c;
    while (!(((c = getchar()) >= '0') && (c <= '9')))
        ;
    a = c - '0';
    while (((c = getchar()) >= '0') && (c <= '9')) (a *= 10) += c - '0';
}
int main() {
    read(n);
    for (i = 1; i < n; i++) read(x), read(y), add(x, y), add(y, x);
    for (i = 1; i <= n; i++) c[i] = 0;
    fa[1] = T[c[1]].f[1] = n + 1;
    dfs(1);
    T[c[1]].s[n + 1] += T[c[1]].sum[1];
    read(m);
    while (m--) {
        read(k);
        read(x);
        if (!k)
            printf("%d\n", T[c[x]].ask(x));
        else
            T[c[x]].cut(x), T[c[x] ^= 1].link(x);
    }
    return 0;
}