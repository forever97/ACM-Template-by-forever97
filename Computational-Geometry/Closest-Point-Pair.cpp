/*
    平面最近点对
    分治+最近查找三个优化
*/
const int N = 100010;
struct P {
    double x, y;
} s[N];
int n, p[N];
inline bool cmpx(P const& a, P const& b) {
    return a.x == b.x ? a.y < b.y : a.x < b.x;
}
inline bool cmpy(int const& a, int const& b) { return s[a].y < s[b].y; }
inline double dis(int const& a, int const& b) {
    return sqrt((s[a].x - s[b].x) * (s[a].x - s[b].x) +
                (s[a].y - s[b].y) * (s[a].y - s[b].y));
}
double fuc(int l, int r) {
    double d = 1e20;
    if (l == r) return d;
    if (r - l == 1) {
        return dis(l, r);
    }
    if (r - l == 2) {
        double s1 = min(dis(l, r), dis(l, l + 1));
        s1 = min(s1, dis(l + 1, r));
        return s1;
    }
    int mid = (l + r) >> 1;
    d = min(fuc(l, mid), fuc(mid + 1, r));
    int cnt = 0;
    for (int i = l; i <= r; i++) {
        if (fabs(s[mid].x - s[i].x) <= d) p[cnt++] = i;
    }
    sort(p, p + cnt, cmpy);
    for (int i = 0; i < cnt; i++)
        for (int j = i + 1; j < cnt && j < i + 3; j++) {
            if (s[p[j]].y - s[p[i]].y >= d) break;
            double d1 = dis(p[i], p[j]);
            if (d > d1) d = d1;
        }
    return d;
}
int main() {
    while (~scanf("%d", &n)) {
        for (int i = 0; i < n; i++) scanf("%lf%lf", &s[i].x, &s[i].y);
        sort(s, s + n, cmpx);
        printf("%.2lf\n", fuc(0, n - 1));
    }
    return 0;
}
