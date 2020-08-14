/*
    Kruskal重构树
    在用kruskal算法做最小生成树的过程中，将边权作为点权，
    当做父节点将两个需要连接的点连起来，
    那么树上每个点的权值都小于父节点的权值，构成一棵单调可二分的树
*/
void ReKruskal(int n, int m) {
    sort(e + 1, e + m + 1, cmp);
    for (int i = 1; i <= n; i++) {
        f[i] = i;
        w[i] = 0;
    }
    R = n;
    for (int i = 1; i <= m; i++) {
        int x = sf(e[i].x), y = sf(e[i].y);
        if (x == y) continue;
        w[++R] = e[i].z;
        f[x] = f[y] = f[R] = R;
        v[R].push_back(x);
        v[R].push_back(y);
    }
    w[0] = N;
}
/*
    Problem:
        给出一个无向连通图，有n个顶点，m条边。
        有q次询问，每次给出x,y,z，
        最小化从x和y开始，总计访问z个不重复顶点，经过的边的编号的最大值
    Solution:
        二分答案+Kruskal重构树
        对原图建立Kruskal重构树，对重构树建立父节点倍增表，
        对于每个询问，二分最大边编号，倍增找到x和y节点在重构树上对应的小于等于二分值的深度最小的祖先，
        其子树和即为能够访问到的不重复点数量。
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 500010;
vector<int> v[N];
int f[N], w[N], R;
int sf(int x) { return x == f[x] ? x : f[x] = sf(f[x]); }
struct data {
    int x, y, z;
} e[N];
bool cmp(data a,data b){
	return a.z < b.z;
} 
void ReKruskal(int n, int m) {
    sort(e + 1, e + m + 1, cmp);
    for (int i = 1; i <= n; i++) {
        f[i] = i;
        w[i] = 0;
    }
    R = n;
    for (int i = 1; i <= m; i++) {
        int x = sf(e[i].x), y = sf(e[i].y);
        if (x == y) continue;
        w[++R] = e[i].z;
        f[x] = f[y] = f[R] = R;
        v[R].push_back(x);
        v[R].push_back(y);
    }
    w[0] = N;
}
int n, m;
const int LOG = 20;
int s[N], fa[N][LOG+1];
void dfs(int x, int fx) {
    fa[x][0] = fx;
    for (int i = 1; i < LOG; i++) fa[x][i] = fa[fa[x][i - 1]][i - 1];
    for (auto y : v[x]) {
        dfs(y, x);
        s[x] += s[y];
    }
    if (x <= n) s[x] = 1;
}
int find(int x, int W) {
    for (int i = LOG; ~i; i--)
        if (w[fa[x][i]] <= W) x = fa[x][i];
    return x;
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++) {
        scanf("%d%d", &e[i].x, &e[i].y);
        e[i].z = i;
    }
    ReKruskal(n, m);
    dfs(R, 0);
    int q, x, y, z;
    scanf("%d", &q);
    while (q--) {
        scanf("%d%d%d", &x, &y, &z);
        int ans = m, l = 0, r = m;
        while (l <= r) {
            int mid = (l + r) >> 1;
            int fx = find(x, mid), fy = find(y, mid), Size = s[fx];
            if (fy != fx) Size += s[fy];
            if (Size >= z)
                ans = mid, r = mid - 1;
            else
                l = mid + 1;
        }
        printf("%d\n", ans);
    }
    return 0;
}
