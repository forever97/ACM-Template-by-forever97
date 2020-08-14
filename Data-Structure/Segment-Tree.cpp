/*
    线段树
    区间修改区间求和
*/
namespace Segment_Tree {
int tot;
struct node {
    int l, r, a, b;
    LL tag, val;
} T[M];
void build(int, int);
void Initialize(int n) {
    tot = 0;
    build(1, n);
}
void addtag(int x, int tag) {
    T[x].tag += tag;
    T[x].val += ((LL)T[x].b - T[x].a + 1) * tag;
}
void pb(int x) {
    if (T[x].l) {
        addtag(T[x].l, T[x].tag);
        addtag(T[x].r, T[x].tag);
    }
    T[x].tag = 0;
}
void up(int x) { T[x].val = T[T[x].l].val + T[T[x].r].val; }
void build(int l, int r) {
    int x = ++tot;
    T[x].a = l;
    T[x].b = r;
    T[x].tag = T[x].l = T[x].r = T[x].val = 0;
    if (l == r) {
        return;
    }
    int mid = (l + r) >> 1;
    T[x].l = tot + 1;
    build(l, mid);
    T[x].r = tot + 1;
    build(mid + 1, r);
    up(x);
}
void change(int x, int a, int b, int p) {
    if (T[x].a >= a && T[x].b <= b) {
        addtag(x, p);
        return;
    }
    if (T[x].tag) pb(x);
    int mid = (T[x].a + T[x].b) >> 1;
    if (mid >= a && T[x].l) change(T[x].l, a, b, p);
    if (mid < b && T[x].r) change(T[x].r, a, b, p);
    up(x);
}
LL query(int x, int a, int b) {
    if (T[x].a >= a && T[x].b <= b) return T[x].val;
    if (T[x].tag) pb(x);
    int mid = (T[x].a + T[x].b) >> 1;
    LL res = 0;
    if (mid >= a && T[x].l) res += query(T[x].l, a, b);
    if (mid < b && T[x].r) res += query(T[x].r, a, b);
    return res;
}
}  // namespace Segment_Tree
