/*
    二维莫队
    给出一个矩阵
    询问查询给出子矩阵中每个种类数字出现次数的平方和
*/
inline char nc() {
    static char buf[100000], *p1 = buf, *p2 = buf;
    if (p1 == p2) {
        p2 = (p1 = buf) + fread(buf, 1, 100000, stdin);
        if (p1 == p2) return EOF;
    }
    return *p1++;
}
inline void read(int &x) {
    char c = nc(), b = 1;
    for (; !(c >= '0' && c <= '9'); c = nc())
        if (c == '-') b = -1;
    for (x = 0; c >= '0' && c <= '9'; x = x * 10 + c - '0', c = nc())
        ;
    x *= b;
}
struct W {
    int x, y, v;
} t[40010];
struct Q {
    int x1, x2, y1, y2, p;
} q[100010];
bool vis[210][210];
int a[210][210], bl[210][210], Ans[100010], id[210][210], T[40010];
int n, m, tot, mx, bn, bm, ans;
bool cmp(W a, W b) { return a.v < b.v; }
bool cmp1(Q a, Q b) {
    return bl[a.x1][a.y1] == bl[b.x1][b.y1] ? id[a.x2][a.y2] < id[b.x2][b.y2]
                                            : bl[a.x1][a.y1] < bl[b.x1][b.y1];
}
void insert(int x, int y) {
    if (!vis[x][y]) {
        vis[x][y] = 1;
        ans += 2 * (T[a[x][y]]++) + 1;
    }
}
void del(int x, int y) {
    if (vis[x][y]) {
        vis[x][y] = 0;
        ans -= 2 * (--T[a[x][y]]) + 1;
    }
}
void ADD(int x1, int x2, int y1, int y2) {
    for (int i = x1; i <= x2; i++)
        for (int j = y1; j <= y2; j++) insert(i, j);
}
void DEL(int x1, int x2, int y1, int y2) {
    for (int i = x1; i <= x2; i++)
        for (int j = y1; j <= y2; j++) del(i, j);
}
void change(Q a, Q b) {
    int t;
    t = min(a.x1 - 1, b.x2);
    ADD(b.x1, t, b.y1, b.y2);
    t = max(a.x2 + 1, b.x1);
    ADD(t, b.x2, b.y1, b.y2);
    t = min(a.y1 - 1, b.y2);
    ADD(b.x1, b.x2, b.y1, t);
    t = max(a.y2 + 1, b.y1);
    ADD(b.x1, b.x2, t, b.y2);
    t = min(b.x1 - 1, a.x2);
    DEL(a.x1, t, a.y1, a.y2);
    t = max(b.x2 + 1, a.x1);
    DEL(t, a.x2, a.y1, a.y2);
    t = min(b.y1 - 1, a.y2);
    DEL(a.x1, a.x2, a.y1, t);
    t = max(b.y2 + 1, a.y1);
    DEL(a.x1, a.x2, t, a.y2);
}
int main() {
    read(n);
    read(m);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            id[i][j] = ++tot, t[tot].x = i, t[tot].y = j, read(t[tot].v);
    sort(t + 1, t + tot + 1, cmp);
    for (int i = 1; i <= tot; i++) {
        if (t[i].v != t[i - 1].v)
            a[t[i].x][t[i].y] = ++mx;
        else
            a[t[i].x][t[i].y] = mx;
    }
    bn = sqrt(n), bm = sqrt(m);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            bl[i][j] = (i - 1) / bn * bm + (j - 1) / bm;
    int Q;
    read(Q);
    for (int i = 1; i <= Q; i++) {
        q[i].p = i;
        read(q[i].x1), read(q[i].y1);
        read(q[i].x2), read(q[i].y2);
        if (q[i].x1 > q[i].x2) swap(q[i].x1, q[i].x2);
        if (q[i].y1 > q[i].y2) swap(q[i].y1, q[i].y2);
    }
    sort(q + 1, q + Q + 1, cmp1);
    for (int i = q[1].x1; i <= q[1].x2; i++)
        for (int j = q[1].y1; j <= q[1].y2; j++) insert(i, j);
    Ans[q[1].p] = ans;
    for (int i = 2; i <= Q; i++) {
        change(q[i - 1], q[i]);
        Ans[q[i].p] = ans;
    }
    for (int i = 1; i <= Q; i++) printf("%d\n", Ans[i]);
    return 0;
}