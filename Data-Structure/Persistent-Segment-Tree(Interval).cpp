/*
    可持久化线段树
    区间修改lazy标记
*/
typedef long long LL;
const int N = 100010;
namespace Persistent_Segment_Tree {
int l[N * 100], r[N * 100], tot, root[N], num[N], lazy[N * 100], mark[N * 100],
    V;
LL v[N * 100];
char op[10];
void up(int x) { v[x] = v[l[x]] + v[r[x]]; }
void pb(int x, int a, int b) {
    if (mark[x]) {
        v[++tot] = v[l[x]];
        lazy[tot] = lazy[l[x]];
        l[tot] = l[l[x]];
        r[tot] = r[l[x]];
        l[x] = tot;
        v[++tot] = v[r[x]];
        lazy[tot] = lazy[r[x]];
        l[tot] = l[r[x]];
        r[tot] = r[r[x]];
        r[x] = tot;
        mark[l[x]] = mark[r[x]] = 1;
        mark[x] = 0;
    }
    if (lazy[x]) {
        int mid = (a + b) >> 1;
        lazy[l[x]] += lazy[x];
        lazy[r[x]] += lazy[x];
        v[l[x]] += (LL)lazy[x] * (mid - a + 1);
        v[r[x]] += (LL)lazy[x] * (b - mid);
        lazy[x] = 0;
    }
}
int build(int a, int b) {
    int x = ++tot;
    mark[x] = lazy[x] = 0;
    if (a == b) {
        v[x] = num[a];
        l[x] = r[x] = 0;
        return x;
    }
    int mid = (a + b) >> 1;
    return l[x] = build(a, mid), r[x] = build(mid + 1, b), up(x), x;
}
int change(int x, int a, int b, int L, int R, int val) {
    int y = ++tot;
    if (L == a && b == R) {
        lazy[y] = lazy[x] + val;
        v[y] = v[x] + LL(val) * (R - L + 1);
        if (L != R) {
            l[y] = l[x];
            r[y] = r[x];
            mark[y] = 1;
        }
        return y;
    }
    lazy[y] = v[y] = mark[y] = 0;
    pb(x, a, b);
    int mid = (a + b) >> 1;
    if (R <= mid)
        l[y] = change(l[x], a, mid, L, R, val), r[y] = r[x];
    else if (L > mid)
        l[y] = l[x], r[y] = change(r[x], mid + 1, b, L, R, val);
    else {
        l[y] = change(l[x], a, mid, L, mid, val);
        r[y] = change(r[x], mid + 1, b, mid + 1, R, val);
    }
    up(y);
    return y;
}
LL query(int x, int a, int b, int L, int R) {
    if (L == a && R == b) return v[x];
    pb(x, a, b);
    int mid = (a + b) >> 1;
    if (R <= mid)
        return query(l[x], a, mid, L, R);
    else if (L > mid)
        return query(r[x], mid + 1, b, L, R);
    else
        return query(l[x], a, mid, L, mid) +
               query(r[x], mid + 1, b, mid + 1, R);
}
}  // namespace Persistent_Segment_Tree
/*
    区间修改
    历史版本区间查询
*/
int n, Q, x, y, z, ans;
int main() {
    while (~scanf("%d%d", &n, &Q)) {
        for (int i = 1; i <= n; i++) scanf("%d", &num[i]);
        V = tot = 0;
        root[0] = build(1, n);
        while (Q--) {
            scanf("%s", op);
            if (op[0] == 'Q') {
                scanf("%d%d", &x, &y);
                printf("%lld\n", query(root[V], 1, n, x, y));
            } else if (op[0] == 'H') {
                scanf("%d%d%d", &x, &y, &z);
                printf("%lld\n", query(root[z], 1, n, x, y));
            } else if (op[0] == 'C') {
                scanf("%d%d%d", &x, &y, &z);
                root[V + 1] = change(root[V], 1, n, x, y, z);
                V++;
            } else {
                scanf("%d", &x);
                V = x;
            }
        }
    }
    return 0;
}