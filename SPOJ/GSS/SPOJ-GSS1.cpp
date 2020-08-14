/*
    题目大意：给出一个数列，多次询问区间最大连续子段和
    题解：线段树维护区间最大连续子段和gss，区间从最左元素开始的最大连续子段和lgss
    区间以最右元素为结尾的最大连续子段和rgss以及区间和s，信息传递并合并即可
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int N = 100010, M = N << 2;
struct data {
    int s, lgss, rgss, gss;
} T[M];
int a[N], n, m;
data merge(data L, data R) {
    data res;
    res.s = L.s + R.s;
    res.lgss = max(L.lgss, L.s + R.lgss);
    res.rgss = max(L.rgss + R.s, R.rgss);
    res.gss = max(max(L.gss, R.gss), L.rgss + R.lgss);
    return res;
}
void build(int x, int l, int r) {
    if (l == r) {
        T[x].s = T[x].lgss = T[x].rgss = T[x].gss = a[l];
        return;
    }
    int mid = (l + r) >> 1;
    build(x << 1, l, mid);
    build(x << 1 | 1, mid + 1, r);
    T[x] = merge(T[x << 1], T[x << 1 | 1]);
}
data query(int x, int l, int r, int L, int R) {
    if (L <= l && r <= R) return T[x];
    int mid = (l + r) >> 1;
    data lft, rht;
    if (L <= mid) lft = query(x << 1, l, mid, L, R);
    if (R > mid) rht = query(x << 1 | 1, mid + 1, r, L, R);
    if (R <= mid) return lft;
    if (L > mid) return rht;
    return merge(lft, rht);
}
int main() {
    while (~scanf("%d", &n)) {
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        build(1, 1, n);
        scanf("%d", &m);
        while (m--) {
            int l, r;
            scanf("%d%d", &l, &r);
            printf("%d\n", query(1, 1, n, l, r).gss);
        }
    }
    return 0;
}
