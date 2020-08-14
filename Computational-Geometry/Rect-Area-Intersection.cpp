/*
    矩形面积交
    线段树+扫描线
    求被覆盖超过一次的面积和
*/
int n;
struct Seg {
    double l, r, h;
    int d;
    Seg() {}
    Seg(double l, double r, double h, double d) : l(l), r(r), h(h), d(d) {}
    bool operator<(const Seg& rhs) const { return h < rhs.h; }
} a[N];
int cnt[N << 2];
double one[N << 2], two[N << 2], all[N];
#define lson l, m, rt << 1
#define rson m + 1, r, rt << 1 | 1
void push_up(int l, int r, int rt) {
    if (cnt[rt] >= 2)
        two[rt] = one[rt] = all[r + 1] - all[l];
    else if (cnt[rt] == 1) {
        one[rt] = all[r + 1] - all[l];
        if (l == r)
            two[rt] = 0;
        else
            two[rt] = one[rt << 1] + one[rt << 1 | 1];
    } else {
        if (l == r)
            one[rt] = two[rt] = 0;
        else {
            one[rt] = one[rt << 1] + one[rt << 1 | 1];
            two[rt] = two[rt << 1] + two[rt << 1 | 1];
        }
    }
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
int T;
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
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
        memset(one, 0, sizeof(one));
        memset(two, 0, sizeof(two));
        double ans = 0;
        for (int i = 1; i < n; i++) {
            int l = lower_bound(all + 1, all + 1 + m, a[i].l) - all;
            int r = lower_bound(all + 1, all + 1 + m, a[i].r) - all;
            if (l < r) update(l, r - 1, a[i].d, 1, m, 1);
            ans += two[1] * (a[i + 1].h - a[i].h);
        }
        printf("%.2f\n", ans);
    }
    return 0;
}