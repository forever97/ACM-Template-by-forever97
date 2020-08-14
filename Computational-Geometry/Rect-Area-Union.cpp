/*
    矩形面积并
    线段树+扫描线
*/
const int N = 2010;
int n;
struct Seg {
    double l, r, h;
    int d;
    Seg() {}
    Seg(double l, double r, double h, int d) : l(l), r(r), h(h), d(d) {}
    bool operator<(const Seg& rhs) const { return h < rhs.h; }
} a[N];
int cnt[N << 2];
double sum[N << 2], all[N];
#define lson l, m, rt << 1
#define rson m + 1, r, rt << 1 | 1
void push_up(int l, int r, int rt) {
    if (cnt[rt])
        sum[rt] = all[r + 1] - all[l];
    else if (l == r)
        sum[rt] = 0;
    else
        sum[rt] = sum[rt << 1] + sum[rt << 1 | 1];
}
void update(int L, int R, int v, int l, int r, int rt) {
    if (L <= l && r <= R) {
        cnt[rt] += v;
        push_up(l, r, rt);
        return;
    }
    int m = l + r >> 1;
    if (L <= m) update(L, R, v, lson);
    if (R > m) update(L, R, v, rson);
    push_up(l, r, rt);
}
int main() {
    while (scanf("%d", &n) && n) {
        for (int i = 1; i <= n; i++) {
            double x1, y1, x2, y2;
            scanf("%lf%lf%lf%lf", &x1, &y1, &x2, &y2);
            a[i] = Seg(x1, x2, y1, 1);
            a[i + n] = Seg(x1, x2, y2, -1);
            all[i] = x1;
            all[i + n] = x2;
        }
        n <<= 1;
        sort(a + 1, a + 1 + n);
        sort(all + 1, all + 1 + n);
        int m = unique(all + 1, all + 1 + n) - all - 1;
        memset(cnt, 0, sizeof(cnt));
        memset(sum, 0, sizeof(sum));
        double ans = 0;
        for (int i = 1; i < n; i++) {
            int l = lower_bound(all + 1, all + 1 + m, a[i].l) - all;
            int r = lower_bound(all + 1, all + 1 + m, a[i].r) - all;
            if (l < r) update(l, r - 1, a[i].d, 1, m, 1);
            ans += sum[1] * (a[i + 1].h - a[i].h);
        }
        printf("%.0f\n", ans);
    }
    puts("*");
    return 0;
}
