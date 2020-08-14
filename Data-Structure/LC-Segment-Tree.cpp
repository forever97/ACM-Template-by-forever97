/*
    LC线段树
    支持插入一条线段，查询横坐标为某个值时最上面的线段。
    插入 O(logn*logn)，查询O(logn)
*/
struct Seg {
    double k, b;
    Seg() {}
    Seg(int x0, int y0, int x1, int y1) {
        if (x0 == x1)
            k = 0, b = max(y0, y1);
        else
            k = 1.0 * (y0 - y1) / (x0 - x1), b = -k * x0 + y0;
    }
    double gety(int x) { return k * x + b; }
} s[100010];
int m, op, cnt, X0, Y0, X1, Y1, ans, v[131000];
inline int sig(double x) { return fabs(x) < 1e-8 ? 0 : (x > 0 ? 1 : -1); }
void ins(int x, int a, int b, int c, int d, int p) {
    if (c <= a && b <= d) {
        if (sig(s[p].gety(a) - s[v[x]].gety(a)) > 0 &&
            sig(s[p].gety(b) - s[v[x]].gety(b)) > 0) {
            v[x] = p;
            return;
        }
        if (sig(s[p].gety(a) - s[v[x]].gety(a)) <= 0 &&
            sig(s[p].gety(b) - s[v[x]].gety(b)) <= 0)
            return;
        if (a == b) return;
    }
    int mid = (a + b) >> 1;
    if (c <= mid) ins(x << 1, a, mid, c, d, p);
    if (d > mid) ins(x << 1 | 1, mid + 1, b, c, d, p);
}
void ask(int x, int a, int b, int c) {
    if (sig(s[ans].gety(c) - s[v[x]].gety(c)) < 0)
        ans = v[x];
    else if (!sig(s[ans].gety(c) - s[v[x]].gety(c)) && ans > v[x])
        ans = v[x];
    if (a == b) return;
    int mid = (a + b) >> 1;
    c <= mid ? ask(x << 1, a, mid, c) : ask(x << 1 | 1, mid + 1, b, c);
}
void read(int& a) {
    char ch;
    while (!((ch = getchar()) >= '0') && (ch <= '9'))
        ;
    a = ch - '0';
    while (((ch = getchar()) >= '0') && (ch <= '9')) a *= 10, a += ch - '0';
}
int main() {
    s[0].b = -1;
    read(m);
    while (m--) {
        read(op);
        if (!op) {
            read(X0);
            ans = 0, ask(1, 1, N, X0);
            printf("%d\n", ans);
        } else {
            read(X0), read(Y0), read(X1), read(Y1);
            s[++cnt] = Seg(X0, Y0, X1, Y1);
            ins(1, 1, N, X0, X1, cnt);
        }
    }
    return 0;
}