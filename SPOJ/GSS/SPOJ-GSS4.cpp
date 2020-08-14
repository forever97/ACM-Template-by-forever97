/*
    题目大意：维护一个数列，支持区间开方和区间求和
    题解：我们记录区间最大值，暴力开方，因为开方到1之后无需再开方，
    因此当区间最大值为1时该区间不再递归处理
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
using namespace std;
typedef long long LL;
const int N = 1000010, M = 2000010;
namespace Segment_Tree {
int tot;
struct node {
    int l, r, a, b;
    LL mx, val;
} T[M];
void build(int, int);
void Initialize(int n) {
    tot = 0;
    build(1, n);
}
void up(int x) {
    T[x].val = T[T[x].l].val + T[T[x].r].val;
    T[x].mx = max(T[T[x].l].mx, T[T[x].r].mx);
}
void build(int l, int r) {
    int x = ++tot;
    T[x].a = l;
    T[x].b = r;
    T[x].l = T[x].r = T[x].val = T[x].mx = 0;
    if (l == r) {
        scanf("%lld", &T[x].val);
        T[x].mx = T[x].val;
        return;
    }
    int mid = (l + r) >> 1;
    T[x].l = tot + 1;
    build(l, mid);
    T[x].r = tot + 1;
    build(mid + 1, r);
    up(x);
}
void change(int x, int a, int b) {
    if (T[x].mx == 1) return;
    if (T[x].a == T[x].b) {
        T[x].mx = T[x].val = sqrt(T[x].val);
        return;
    }
    int mid = (T[x].a + T[x].b) >> 1;
    if (mid >= a && T[x].l) change(T[x].l, a, b);
    if (mid < b && T[x].r) change(T[x].r, a, b);
    up(x);
}
LL query(int x, int a, int b) {
    if (T[x].a >= a && T[x].b <= b) return T[x].val;
    int mid = (T[x].a + T[x].b) >> 1;
    LL res = 0;
    if (mid >= a && T[x].l) res += query(T[x].l, a, b);
    if (mid < b && T[x].r) res += query(T[x].r, a, b);
    return res;
}
}  // namespace Segment_Tree
int n, m, t, l, r, cnt = 0;
int main() {
    while (~scanf("%d", &n)) {
        Segment_Tree::Initialize(n);
        if (cnt) puts("");
        printf("Case #%d:\n", ++cnt);
        scanf("%d", &m);
        while (m--) {
            scanf("%d%d%d", &t, &l, &r);
            if (l > r) swap(l, r);
            if (t == 0)
                Segment_Tree::change(1, l, r);
            else
                printf("%lld\n", Segment_Tree::query(1, l, r));
        }
    }
    return 0;
}
