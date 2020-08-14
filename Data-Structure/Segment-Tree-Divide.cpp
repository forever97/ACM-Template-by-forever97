/*
    线段树-区间除法
    线段树维护区间极值，支持区间加法和区间整除
    当区间的最大值和最小值在除法之后得到的数等价于原数字减去相同数的时候，
    区间除法转化为区间减法，否则向下划分区间直到满足条件，以此方法解决区间除法问题
*/
using namespace std;
const int INF = 1e9;
const int M = 1000000;
namespace Segment_Tree {
int tot;
struct node {
    int l, r, a, b;
    int tag, mx, mn;
} T[M];
void build(int, int);
void Initialize(int n) {
    tot = 0;
    build(1, n);
}
void addtag(int x, int tag) {
    T[x].tag += tag;
    T[x].mx += tag;
    T[x].mn += tag;
}
void pb(int x) {
    if (T[x].l) {
        addtag(T[x].l, T[x].tag);
        addtag(T[x].r, T[x].tag);
    }
    T[x].tag = 0;
}
void up(int x) {
    T[x].mx = max(T[T[x].l].mx, T[T[x].r].mx);
    T[x].mn = min(T[T[x].l].mn, T[T[x].r].mn);
}
void build(int l, int r) {
    int x = ++tot;
    T[x].a = l;
    T[x].b = r;
    T[x].tag = T[x].l = T[x].r = T[x].mx = T[x].mn = 0;
    if (l == r) {
        scanf("%d", &T[x].mx);
        T[x].mn = T[x].mx;
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
int querymx(int x, int a, int b) {
    if (T[x].a >= a && T[x].b <= b) return T[x].mx;
    if (T[x].tag) pb(x);
    int mid = (T[x].a + T[x].b) >> 1;
    int res = -INF;
    if (mid >= a && T[x].l) res = max(res, querymx(T[x].l, a, b));
    if (mid < b && T[x].r) res = max(res, querymx(T[x].r, a, b));
    ;
    return res;
}
void Divide(int x, int a, int b, int p) {
    if (T[x].a >= a && T[x].b <= b) {
        if (T[x].mn - T[x].mn / p == T[x].mx - T[x].mx / p) {
            addtag(x, T[x].mx / p - T[x].mx);
            return;
        }
    }
    if (T[x].tag) pb(x);
    int mid = (T[x].a + T[x].b) >> 1;
    if (mid >= a && T[x].l) Divide(T[x].l, a, b, p);
    if (mid < b && T[x].r) Divide(T[x].r, a, b, p);
    up(x);
}
}  // namespace Segment_Tree
int n, q, t, l, r, x;
int main() {
    scanf("%d%d", &n, &q);
    Segment_Tree::Initialize(n);
    while (q--) {
        scanf("%d%d%d%d", &t, &l, &r, &x);
        l++;
        r++;
        if (t == 0)
            Segment_Tree::change(1, l, r, x);
        else if (t == 1)
            Segment_Tree::Divide(1, l, r, x);
        else
            printf("%d\n", Segment_Tree::querymx(1, l, r));
    }
    return 0;
}
