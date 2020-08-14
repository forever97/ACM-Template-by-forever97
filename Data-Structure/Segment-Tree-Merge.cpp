/*
    线段树合并
*/
int merge(int x, int y, int l, int r) {
    if (!x) return y;
    if (!y) return x;
    int z = ++tot;
    if (l == r) {
        T[z].v = T[x].v + T[y].v;
        return z;
    }
    int mid = (l + r) >> 1;
    T[z].l = merge(T[x].l, T[y].l, l, mid);
    T[z].r = merge(T[x].r, T[y].r, mid + 1, r);
    T[z].v = T[T[z].l].v + T[T[z].r].v;
    return z;
}
/*
    Example
        给出一棵树，对于每个节点，求其子树中比父节点大的点个数
    Solution
        我们考虑每个权值建立一棵线段树，边dfs边将子节点合并为一颗线段树，
        那么只要查询当前点的树上后缀和即可。
*/
const int N = 100010, M = N * 20;
int n, a[N], ans[N], root[N], disc[N];
vector<int> v[N];
namespace Segment_Tree {
int tot;
struct node {
    int l, r, a, b, sum;
} T[M];
void up(int x) { T[x].sum = T[T[x].l].sum + T[T[x].r].sum; }
int build(int l, int r, int p) {
    int x = ++tot;
    T[x].a = l;
    T[x].b = r;
    T[x].sum = 0;
    if (l == r) {
        T[x].sum = 1;
        return x;
    }
    int mid = (l + r) >> 1;
    if (p <= mid) {
        T[x].l = build(l, mid, p);
    } else {
        T[x].r = build(mid + 1, r, p);
    }
    return up(x), x;
}
int ask(int x, int l, int r) {
    if (!x) return 0;
    if (l <= T[x].a && T[x].b <= r) return T[x].sum;
    int mid = (T[x].a + T[x].b) >> 1, res = 0;
    if (l <= mid) res += ask(T[x].l, l, r);
    if (r > mid) res += ask(T[x].r, l, r);
    return res;
}
int merge(int x, int y) {
    if (!x || !y) return x ^ y;
    T[x].l = merge(T[x].l, T[y].l);
    T[x].r = merge(T[x].r, T[y].r);
    return up(x), x;
}
void dfs(int x, int fx) {
    int res = 0;
    for (int i = 0; i < v[x].size(); i++) {
        int y = v[x][i];
        if (y == fx) continue;
        dfs(y, x);
        res += ask(root[y], a[x] + 1, n);
        root[x] = merge(root[x], root[y]);
    }
    ans[x] = res;
}
}  // namespace Segment_Tree
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]), disc[i] = a[i];
    sort(disc + 1, disc + n + 1);
    int m = unique(disc + 1, disc + n + 1) - disc - 1;
    for (int i = 1; i <= n; i++)
        a[i] = lower_bound(disc + 1, disc + m + 1, a[i]) - disc;
    for (int i = 2; i <= n; i++) {
        int x;
        scanf("%d", &x);
        v[x].push_back(i);
        v[i].push_back(x);
    }
    for (int i = 1; i <= n; i++) root[i] = Segment_Tree::build(1, n, a[i]);
    Segment_Tree::dfs(1, 1);
    for (int i = 1; i <= n; i++) printf("%d\n", ans[i]);
    return 0;
}
/*
    Example
        给出一棵二叉树，每个叶节点上有一个权值，现在可以任意交换左右儿子，
        使得逆序对最少，求最少的逆序对数量
    Solution
        我们发现对于每个非叶节点来说，其贡献值为左右两个儿子的权值树上，
        每个节点想反位置的数量和乘积，
        比如左儿子的权值树左节点和右儿子权值树的右节点相乘，
        那么我们对于每个节点建立一颗权值线段树，仅保留非0链，
        递归合并这些权值线段树，同时每次将相反位置数量乘积的最小值累加到答案即可
*/
const int N = 400010, M = N * 20;
typedef long long LL;
int n, a[N], cnt, Root, root[N];
int Tree[N][2];
LL Ans, Ans0, Ans1;
void Read_Tree(int &x) {
    x = ++cnt;
    scanf("%d", &a[x]);
    if (a[x]) return;
    Read_Tree(Tree[x][0]);
    Read_Tree(Tree[x][1]);
}
namespace Segment_Tree {
int tot;
struct node {
    int l, r, a, b, sum;
} T[M];
void up(int x) { T[x].sum = T[T[x].l].sum + T[T[x].r].sum; }
int build(int l, int r, int p) {
    int x = ++tot;
    T[x].a = l;
    T[x].b = r;
    T[x].sum = 0;
    if (l == r) {
        T[x].sum = 1;
        return x;
    }
    int mid = (l + r) >> 1;
    if (p <= mid) {
        T[x].l = build(l, mid, p);
    } else {
        T[x].r = build(mid + 1, r, p);
    }
    return up(x), x;
}
int merge(int x, int y) {
    if (!x || !y) return x ^ y;
    Ans0 += (LL)T[T[x].r].sum * (LL)T[T[y].l].sum;
    Ans1 += (LL)T[T[x].l].sum * (LL)T[T[y].r].sum;
    T[x].l = merge(T[x].l, T[y].l);
    T[x].r = merge(T[x].r, T[y].r);
    return up(x), x;
}
void dfs(int x) {
    if (a[x]) return;
    dfs(Tree[x][0]);
    dfs(Tree[x][1]);
    Ans0 = Ans1 = 0;
    root[x] = merge(root[Tree[x][0]], root[Tree[x][1]]);
    Ans += min(Ans0, Ans1);
}
}  // namespace Segment_Tree
int main() {
    scanf("%d", &n);
    Read_Tree(Root);
    for (int i = 1; i <= cnt; i++)
        if (a[i] != 0) root[i] = Segment_Tree::build(1, n, a[i]);
    Segment_Tree::dfs(Root);
    printf("%lld\n", Ans);
    return 0;
}