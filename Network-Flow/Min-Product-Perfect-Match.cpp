/*
    最小乘积匹配
    给出一张二分图，每条边上有a,b两个值，
    求完美匹配，使得suma*sumb最小
*/
typedef long long LL;
const int N = 310;
const LL INF = 0x3f3f3f3f3f3f3f3f;
int nx, ny;       //两边的点数
LL g[N][N];       //二分图描述
int linker[N];    // y中各点匹配状态
LL lx[N], ly[N];  // x,y中的点标号
int n;
LL slack[N];
bool visx[N], visy[N];
LL ans = INF;
int T, a[N][N], b[N][N];
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
bool DFS(int x) {
    visx[x] = 1;
    for (int y = 0; y < ny; y++) {
        if (visy[y]) continue;
        int tmp = lx[x] + ly[y] - g[x][y];
        if (tmp == 0) {
            visy[y] = true;
            if (linker[y] == -1 || DFS(linker[y])) {
                linker[y] = x;
                return 1;
            }
        } else if (slack[y] > tmp)
            slack[y] = tmp;
    }
    return 0;
}
P KM() {
    P p;
    memset(linker, -1, sizeof(linker));
    memset(ly, 0, sizeof(ly));
    for (int i = 0; i < nx; i++) {
        lx[i] = -INF;
        for (int j = 0; j < ny; j++)
            if (g[i][j] > lx[i]) lx[i] = g[i][j];
    }
    for (int x = 0; x < nx; x++) {
        for (int i = 0; i < ny; i++) slack[i] = INF;
        for (;;) {
            memset(visx, false, sizeof(visx));
            memset(visy, false, sizeof(visy));
            if (DFS(x)) break;
            LL d = INF;
            for (int i = 0; i < ny; i++)
                if (!visy[i] && d > slack[i]) d = slack[i];
            for (int i = 0; i < nx; i++)
                if (visx[i]) lx[i] -= d;
            for (int i = 0; i < ny; i++) {
                if (visy[i])
                    ly[i] += d;
                else
                    slack[i] -= d;
            }
        }
    }
    LL res = 0;
    for (int i = 0; i < ny; i++)
        if (linker[i] != -1) {
            p.x += a[linker[i]][i];
            p.y += b[linker[i]][i];
        }
    res = (LL)p.x * p.y;
    if (res < ans) ans = res;
    return p;
}
void work(P l, P r) {
    P t = l - r;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) g[i][j] = -cross(P(a[i][j], b[i][j]), t);
    P mid = KM();
    if (cross(mid - l, r - mid) > 0) work(l, mid), work(mid, r);
}
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        ans = INF;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) scanf("%d", &a[i][j]);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) scanf("%d", &b[i][j]);
        nx = ny = n;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) g[i][j] = -a[i][j];
        l = KM();
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) g[i][j] = -b[i][j];
        r = KM();
        work(l, r);
        printf("%lld\n", ans);
    }
    return 0;
}