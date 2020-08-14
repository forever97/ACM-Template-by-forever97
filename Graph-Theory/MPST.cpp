/*
 * 最小乘积生成树
 * 给出一张无向图，每条边上有a,b两个值，求生成树，
 * 使得suma*sumb最小，在满足这个前提下保证suma最小
 */
const int N = 210, M = 10010;
typedef long long LL;
struct P {
    int x, y;
    P() { x = y = 0; }
    P(int _x, int _y) {
        x = _x;
        y = _y;
    }
    P operator-(const P& rhs) { return P(x - rhs.x, y - rhs.y); }
} l, r;
LL cross(P a, P b) { return (LL)a.x * b.y - (LL)a.y * b.x; }
struct E {
    int x, y, a, b, c;
} e[M];
bool cmp(E a, E b) { return a.c < b.c; }
int n, m, f[N];
LL ans = 0x3f3f3f3f3f3f3f3f, ansx, ansy;
int sf(int x) { return f[x] == x ? x : f[x] = sf(f[x]); }
P kruskal() {
    P p;
    int i;
    sort(e + 1, e + m + 1, cmp);
    for (int i = 1; i <= n; i++) f[i] = i;
    for (int i = 1; i <= m; i++) {
        if (sf(e[i].x) != sf(e[i].y)) {
            f[f[e[i].x]] = f[e[i].y];
            p.x += e[i].a, p.y += e[i].b;
        }
    }
    if ((LL)p.x * p.y <= ans) {
        if (ans == (LL)p.x * p.y) {
            if (p.x < ansx) ansx = p.x, ansy = p.y;
        } else {
            ans = (LL)p.x * p.y;
            ansx = p.x;
            ansy = p.y;
        }
    }
    return p;
}
void work(P l, P r) {
    P t = l - r;
    for (int i = 1; i <= m; i++) e[i].c = cross(P(e[i].a, e[i].b), t);
    P mid = kruskal();
    if (cross(mid - l, r - mid) > 0) work(l, mid), work(mid, r);
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d%d", &e[i].x, &e[i].y, &e[i].a, &e[i].b);
        e[i].x++;
        e[i].y++;
        e[i].c = e[i].a;
    }
    l = kruskal();
    for (int i = 1; i <= m; i++) e[i].c = e[i].b;
    r = kruskal();
    work(l, r);
    printf("%lld %lld\n", ansx, ansy);
    return 0;
}