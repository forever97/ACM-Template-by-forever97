/*
    可持久化线段树
    每次从root[0]版本开始更新即可
*/
namespace Persistent_Segment_Tree {
int l[N * 40], r[N * 40], v[N * 40], tot, root[S];
void Initialize() {
    root[0] = 0;
    tot = 0;
}
int change(int x, int a, int b, int c, int p) {
    int y = ++tot;
    v[y] = v[x] + p;
    if (a == b) return y;
    int mid = (a + b) >> 1;
    if (c <= mid)
        l[y] = change(l[x], a, mid, c, p), r[y] = r[x];
    else
        l[y] = l[x], r[y] = change(r[x], mid + 1, b, c, p);
    return y;
}
int query(int x, int a, int b, int c, int d) {
    if (!x) return 0;
    if (c <= a && b <= d) return v[x];
    int mid = (a + b) >> 1, res = 0;
    if (c <= mid) res += query(l[x], a, mid, c, d);
    if (d > mid) res += query(r[x], mid + 1, b, c, d);
    return res;
}
// 区间第k大
int kth(int x, int y, int a, int b, int k) {
    if (a == b) return a;
    int mid = (a + b) >> 1;
    if (v[l[y]] - v[l[x]] >= k)
        return kth(l[x], l[y], a, mid, k);
    else
        return kth(r[x], r[y], mid + 1, b, k - v[l[y]] + v[l[x]]);
}
// 第x个版本全局第k大
int getkth(int x, int a, int b, int k) {
    if (a == b) return a;
    int mid = (a + b) >> 1;
    if (v[l[x]] >= k)
        return getkth(l[x], a, mid, k);
    else
        return getkth(r[x], mid + 1, b, k - v[l[x]]);
}
}  // namespace Persistent_Segment_Tree
/*
    离散求静态区间kth
*/
using namespace Persistent_Segment_Tree;
int cmp(int i, int j) { return a[i] < a[j]; }
int n, m, rk[N], sa[N], a[N];
int main() {
    while (~scanf("%d%d", &n, &m)) {
        Initialize();
        for (int i = 1; i <= n; i++) scanf("%d", a + i);
        for (int i = 1; i <= n; i++) sa[i] = i;
        sort(sa + 1, sa + n + 1, cmp);
        for (int i = 1; i <= n; i++) rk[sa[i]] = i;
        for (int i = 1; i <= n; i++)
            root[i] = change(root[i - 1], 1, n, rk[i], 1);
        for (int i = 1; i <= m; i++) {
            scanf("%d%d%d", &x, &y, &z);
            int id = kth(root[x - 1], root[y], 1, n, z);
            printf("%d\n", a[sa[id]]);
        }
    }
    return 0;
}
/*
    求区间非重元素的个数
*/
int T, n, m, a[N], pre[N];
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        Initialize();
        memset(pre, -1, sizeof(pre));
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        root[n + 1] = 0;
        for (int i = n; i; i--) {
            if (pre[a[i]] == -1)
                root[i] = change(root[i + 1], 1, n, i, 1);
            else {
                int tmp = change(root[i + 1], 1, n, pre[a[i]], -1);
                root[i] = change(tmp, 1, n, i, 1);
            }
            pre[a[i]] = i;
        }
        while (m--) {
            scanf("%d%d", &x, &y);
            printf("%d\n", query(root[x], 1, n, x, y));
        }
    }
    return 0;
}