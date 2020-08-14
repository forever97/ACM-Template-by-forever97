/*
    题目大意：给出一个数列，多次询问区间最大连续子段和，计数时重复元素只算进子段和中一次
    题解：考虑离线对询问进行扫描线，线段树i位置表示i时间一直到当前的时间节点的累加值，
    区间最大值nmx表示当前时间为终点的后缀最大子段和，记mx为区间最大子段和，
    我们发现区间最大子段和mx其实就是后缀最大子段和的历史最大值，
    每次处理到节点i，只要在区间加上a[i]，就能更新所有的后缀和nmx，
    对于非重元素，更新区间为[1,i]，对于重复元素，更新区间为[lst[a[i]]+1,i],
    区间更新需要打tag，相对的，我们还要维护一个历史最大标记mxtag，
    历史最大值的是需要当前的最大值加上下传的历史最大标记来更新的，
    在历史最大值更新之后才能用tag去更新当前的最大值，
    因为线段树标记发生变化的前提条件是上方没有任何标记，
    所以上下标记作用的时间域不会有交集，从而能保证正确性。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long LL;
const LL INF = 0x3f3f3f3f3f3f3f3f;
const int N = 100010, M = N << 2;
namespace Segment_Tree {
int tot;
struct node {
    int l, r, a, b;
    LL tag, mx, mxtag, nmx;
} T[M];
void build(int, int);
void Initialize(int n) {
    tot = 0;
    build(1, n);
}
void pb(int x) {
    T[T[x].l].mxtag = max(T[T[x].l].mxtag, T[T[x].l].tag + T[x].mxtag);
    T[T[x].r].mxtag = max(T[T[x].r].mxtag, T[T[x].r].tag + T[x].mxtag);
    T[T[x].l].tag += T[x].tag;
    T[T[x].r].tag += T[x].tag;
    T[T[x].l].mx = max(T[T[x].l].mx, T[T[x].l].nmx + T[x].mxtag);
    T[T[x].r].mx = max(T[T[x].r].mx, T[T[x].r].nmx + T[x].mxtag);
    T[T[x].l].nmx += T[x].tag;
    T[T[x].r].nmx += T[x].tag;
    T[x].tag = 0;
    T[x].mxtag = -INF;
}
void up(int x) {
    T[x].mx = max(T[T[x].l].mx, T[T[x].r].mx);
    T[x].nmx = max(T[T[x].l].nmx, T[T[x].r].nmx);
}
void build(int l, int r) {
    int x = ++tot;
    T[x].a = l;
    T[x].b = r;
    T[x].l = T[x].r = T[x].tag = 0;
    T[x].mxtag = T[x].nmx = T[x].mx = -INF;
    if (l == r) {
        return;
    }
    int mid = (l + r) >> 1;
    T[x].l = tot + 1;
    build(l, mid);
    T[x].r = tot + 1;
    build(mid + 1, r);
}
void modify(int x, int y, int p) {
    if (T[x].a == T[x].b) {
        T[x].mx = T[x].nmx = p;
        return;
    }
    int mid = (T[x].a + T[x].b) >> 1;
    pb(x);
    if (mid >= y) modify(T[x].l, y, p);
    if (mid < y) modify(T[x].r, y, p);
    up(x);
}
void change(int x, int a, int b, int p) {
    if (T[x].a >= a && T[x].b <= b) {
        T[x].nmx += p;
        T[x].mx = max(T[x].mx, T[x].nmx);
        T[x].tag += p;
        T[x].mxtag = max(T[x].mxtag, T[x].tag);
        return;
    }
    int mid = (T[x].a + T[x].b) >> 1;
    pb(x);
    if (mid >= a) change(T[x].l, a, b, p);
    if (mid < b) change(T[x].r, a, b, p);
    up(x);
}
LL query(int x, int a, int b) {
    if (T[x].a >= a && T[x].b <= b) return T[x].mx;
    int mid = (T[x].a + T[x].b) >> 1;
    pb(x);
    LL res = -INF;
    if (mid >= a) res = max(res, query(T[x].l, a, b));
    if (mid < b) res = max(res, query(T[x].r, a, b));
    return res;
}
}  // namespace Segment_Tree
const int base = 100000;
LL ans[N];
int head[N], nxt[N], v[N], lst[N + base];
void add(int x, int y, int id) {
    nxt[id] = head[y];
    head[y] = id;
    v[id] = x;
}
int n, m, a[N];
int main() {
    while (~scanf("%d", &n)) {
        memset(head, 0, sizeof(head));
        memset(lst, 0, sizeof(lst));
        Segment_Tree::Initialize(n);
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        scanf("%d", &m);
        for (int i = 1; i <= m; i++) {
            int x, y;
            scanf("%d%d", &x, &y);
            add(x, y, i);
        }
        for (int i = 1; i <= n; i++) {
            Segment_Tree::modify(1, i, a[i]);
            if (lst[a[i] + base] + 1 < i)
                Segment_Tree::change(1, lst[a[i] + base] + 1, i - 1, a[i]);
            lst[a[i] + base] = i;
            for (int j = head[i]; j; j = nxt[j])
                ans[j] = Segment_Tree::query(1, v[j], i);
        }
        for (int i = 1; i <= m; i++) printf("%lld\n", max(ans[i], 0ll));
    }
    return 0;
}
