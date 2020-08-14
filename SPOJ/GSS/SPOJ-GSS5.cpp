/*
    题目大意：给出一个数列，多次询问区间最大连续子段和，
    要求区间的左端点在x1,y1之间，右端点在x2,y2之间
    题解：线段树维护区间最大连续子段和gss，区间从最左元素开始的最大连续子段和lgss
    区间以最右元素为结尾的最大连续子段和rgss以及区间和s，信息传递并合并即可
    考虑询问区间的特殊性，如果两个端点区间不相交，
    则答案为[x1,y1].rgss+[y1+1,x2-1].s+[x2,y2].lgss
    如果区间相交，则中间区间[x2,y1]中必须有取到的部分，剩余两个区间可取可不取，
    但是必须与中间取到部分连接才能用于更新答案
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int N = 100010, M = N << 2, INF = 0x3f3f3f3f;
struct data {
    int s, lgss, rgss, gss;
} T[M];
int a[N], s[N], n, m;
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
int Cas, ans;
data Q;
int main() {
    scanf("%d", &Cas);
    while (Cas--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]), s[i] = s[i - 1] + a[i];
        build(1, 1, n);
        scanf("%d", &m);
        while (m--) {
            int x1, x2, y1, y2;
            scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
            if (y1 < x2) {  // 区间不相交的情况
                ans = query(1, 1, n, x1, y1).rgss;
                ans += query(1, 1, n, x2, y2).lgss;
                printf("%d\n", ans + s[x2 - 1] - s[y1]);
            } else {  // 区间相交
                ans = -INF;
                int L = 0, M = 0, MR = 0, R = 0;
                if (x2 > x1) {
                    Q = query(1, 1, n, x1, x2 - 1);
                    L = max(Q.rgss, L);
                }
                Q = query(1, 1, n, x2, y1);
                ans = max(L + Q.lgss, Q.gss);
                MR = Q.rgss;
                M = Q.s;
                if (y1 < y2) {
                    Q = query(1, 1, n, y1 + 1, y2);
                    R = max(R, Q.lgss);
                }
                ans = max(ans, L + M + R);
                ans = max(ans, MR + R);
                printf("%d\n", ans);
            }
        }
    }
    return 0;
}
