/*
    线段树+矩阵变换
    Example
    1.Move l r x y 让编号在[l,r]内的所有石子堆向东移动x个单位，向北移动y个单位。
    2.To l r x y 让编号在[l,r]内的所有石子堆移动至(x,y) 。
    3.Rotate l r 让编号在[l,r]内的所有石子堆按原点逆时针旋转90°。
    4.Ask k 询问第k个石子堆的坐标。
*/
#define rep(i, n) for (int i = 0; i < n; i++)
const int N = 3;
struct mat {
    int a[N][N];
    mat() { rep(i, N) rep(j, N) a[i][j] = 0; }
};
mat loc(int x, int y) {
    mat c;
    c.a[0][0] = x;
    c.a[1][0] = y;
    c.a[2][0] = 1;
    return c;
}
mat rotate() {
    mat c;
    c.a[0][0] = 0;
    c.a[0][1] = -1;
    c.a[1][0] = 1;
    c.a[1][1] = 0;
    c.a[2][2] = 1;
    return c;
}
mat move(int x, int y) {
    mat c;
    c.a[0][0] = c.a[1][1] = c.a[2][2] = 1;
    c.a[0][2] = x;
    c.a[1][2] = y;
    return c;
}
mat moveto(int x, int y) {
    mat c;
    c.a[0][2] = x;
    c.a[1][2] = y;
    c.a[2][2] = 1;
    return c;
}
mat one() {
    mat c;
    c.a[0][0] = c.a[1][1] = c.a[2][2] = 1;
    return c;
}
mat mul(mat a, mat b) {
    mat c;
    rep(i, N) rep(j, N) rep(k, N) c.a[i][j] += a.a[i][k] * b.a[k][j];
    return c;
}
const int M = 100010;
int l[M], r[M], tot;
mat val[M], tmp;
bool tag[M];
int n, i, j, q;
int X[50010], Y[50010], XX, YY;
char op[10];
void build(int a, int b) {
    int x = ++tot;
    if (a == b) {
        val[x] = loc(X[a], Y[a]);
        return;
    }
    int mid = (a + b) >> 1;
    val[x] = one();
    l[x] = tot + 1;
    build(a, mid);
    r[x] = tot + 1;
    build(mid + 1, b);
}
void addtag(int x, mat p) { val[x] = mul(p, val[x]), tag[x] = 1; }
void pb(int x) {
    if (tag[x]) {
        if (l[x]) addtag(l[x], val[x]);
        if (r[x]) addtag(r[x], val[x]);
        tag[x] = 0;
        val[x] = one();
    }
}
void change(int x, int a, int b, int c, int d, mat p) {
    if (c <= a && b <= d) {
        addtag(x, p);
        return;
    }
    pb(x);
    int mid = (a + b) >> 1;
    if (c <= mid) change(l[x], a, mid, c, d, p);
    if (d > mid) change(r[x], mid + 1, b, c, d, p);
}
mat ask(int x, int a, int b, int c) {
    if (a == b) return val[x];
    int mid = (a + b) >> 1;
    if (c <= mid) return mul(val[x], ask(l[x], a, mid, c));
    return mul(val[x], ask(r[x], mid + 1, b, c));
}
int main() {
    while (~scanf("%d", &n)) {
        for (int i = 1; i <= n; i++) scanf("%d%d", X + i, Y + i);
        tot = 0;
        build(1, n);
        scanf("%d", &q);
        while (q--) {
            scanf("%s", op);
            if (op[0] == 'M') {
                scanf("%d%d%d%d", &i, &j, &XX, &YY);
                change(1, 1, n, i, j, move(XX, YY));
            }
            if (op[0] == 'T') {
                scanf("%d%d%d%d", &i, &j, &XX, &YY);
                change(1, 1, n, i, j, moveto(XX, YY));
            } else if (op[0] == 'R') {
                scanf("%d%d", &i, &j);
                change(1, 1, n, i, j, rotate());
            } else if (op[0] == 'A') {
                scanf("%d", &i);
                tmp = ask(1, 1, n, i);
                printf("%d %d\n", tmp.a[0][0], tmp.a[1][0]);
            }
        }
    }
    return 0;
}
