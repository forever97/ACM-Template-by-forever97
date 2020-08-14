/*
    Problem:
        给出一棵点权树，每次查询一棵子树中选择一个数字与给定数异或的最大值
    Solution:
        用字典树维护每个子树的数集，按dfs序合并子树字典树，在根处查询每个询问
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 100010;
const int D = 31;
int T[(D + 2) * N][2], root[N], tot, Q[N], ans[N], a[N];
vector<int> v[N], id[N];
void init() { tot = 0; }
int newnode() {
    ++tot;
    T[tot][0] = T[tot][1] = 0;
    return tot;
}
void insert(int rt, int x) {
    for (int i = D; i >= 0; i--) {
        int idx = (x >> i) & 1;
        if (!T[rt][idx]) T[rt][idx] = newnode();
        rt = T[rt][idx];
    }
}
int query(int rt, int x) {
    int res = 0;
    for (int i = D; i >= 0; i--) {
        int idx = (x >> i) & 1;
        if (T[rt][idx ^ 1]) {
            rt = T[rt][idx ^ 1];
            res |= (1 << i);
        } else
            rt = T[rt][idx];
    }
    return res;
}
int merge(int x, int y) {
    if (!x || !y) return x ^ y;
    T[x][0] = merge(T[x][0], T[y][0]);
    T[x][1] = merge(T[x][1], T[y][1]);
    return x;
}
void dfs(int x, int fx) {
    root[x] = newnode();
    insert(root[x], a[x]);
    for (auto &y : v[x]) {
        if (y == fx) continue;
        dfs(y, x);
        root[x] = merge(root[x], root[y]);
    }
    for (auto &idx : id[x]) ans[idx] = query(root[x], Q[idx]);
}
int n, q, x;
int main() {
    while (~scanf("%d%d", &n, &q)) {
        init();
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            v[i].clear();
            id[i].clear();
        }
        for (int i = 2; i <= n; i++) {
            scanf("%d", &x);
            v[x].push_back(i);
        }
        for (int i = 1; i <= q; i++) {
            scanf("%d%d", &x, &Q[i]);
            id[x].push_back(i);
        }
        dfs(1, 1);
        for (int i = 1; i <= q; i++) printf("%d\n", ans[i]);
    }
    return 0;
}
/*
    Problem:
        给出一棵n个点的边权树，边权值从0到n-1各不相同，定义A值为一条链上的最大值，B值为一条链的异或值
        问不同的A值对应的最大B值为多少
    Solution:
        我们从小到大枚举每条边，问题就转化为将这条边两边连通，计算从这条边两边的连通块中各选出一个点，
        连起来得到的链的异或值的最大值，我们通过dfs计算每个点到定根的xor值，记为a，
        那么a[x]^a[y]就表示x到y之间的异或值，所以我们只要求两个集合中各选出一个数字异或和最大即可，
        我们将小的集合中的元素在大的集合中查询，启发式合并字典树即可
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 100010;
const int D = 31;
int T[(D + 2) * N][2], root[N], tot;
void init() { tot = 0; }
int newnode() {
    ++tot;
    T[tot][0] = T[tot][1] = 0;
    return tot;
}
void insert(int rt, int x) {
    for (int i = D; i >= 0; i--) {
        int idx = (x >> i) & 1;
        if (!T[rt][idx]) T[rt][idx] = newnode();
        rt = T[rt][idx];
    }
}
int query(int rt, int x) {
    int res = 0;
    for (int i = D; i >= 0; i--) {
        int idx = (x >> i) & 1;
        if (T[rt][idx ^ 1]) {
            rt = T[rt][idx ^ 1];
            res |= (1 << i);
        } else
            rt = T[rt][idx];
    }
    return res;
}
int merge(int x, int y) {
    if (!x || !y) return x ^ y;
    T[x][0] = merge(T[x][0], T[y][0]);
    T[x][1] = merge(T[x][1], T[y][1]);
    return x;
}
struct Edge {
    int x, y, z;
} p[N];
bool cmp(Edge a, Edge b) { return a.z < b.z; }
vector<int> w[N], v[N], s[N];
int n, f[N], ans[N], a[N];
int sf(int x) { return x == f[x] ? x : f[x] = sf(f[x]); }
void Union(int x, int y, int id) {
    int fx = sf(x), fy = sf(y);
    if (s[fx].size() < s[fy].size()) swap(fx, fy);
    for (auto &u : s[fy]) {
        s[fx].push_back(u);
        ans[id] = max(ans[id], query(root[fx], a[u]));
    }
    f[fy] = fx;
    root[fx] = merge(root[fx], root[fy]);
}
void dfs(int x, int fx) {
    for (int i = 0; i < v[x].size(); i++) {
        int y = v[x][i], z = w[x][i];
        if (y == fx) continue;
        a[y] = a[x] ^ z;
        dfs(y, x);
    }
}
int main() {
    while (~scanf("%d", &n)) {
        init();
        for (int i = 1; i <= n; i++)
            f[i] = i, root[i] = newnode(), s[i].clear();
        for (int i = 1; i < n; i++) {
            scanf("%d%d%d", &p[i].x, &p[i].y, &p[i].z);
            int x = p[i].x, y = p[i].y, z = p[i].z;
            v[x].push_back(y);
            w[x].push_back(z);
            v[y].push_back(x);
            w[y].push_back(z);
        }
        a[1] = 0;
        dfs(1, 1);
        for (int i = 1; i <= n; i++) insert(root[i], a[i]), s[i].push_back(i);
        sort(p + 1, p + n, cmp);
        for (int i = 1; i < n; i++) {
            Union(p[i].x, p[i].y, i);
            printf("%d%c", ans[i], " \n"[i == n - 1]);
        }
    }
    return 0;
}
