/*
    线性规划
    最大化a[0][i]*xi
    线性约束a[k][i]*xi<=a[k][0]
    type=1时输出x此刻的取值
*/
using namespace std;
typedef long long ll;
const int N = 25;
const double eps = 1e-8, INF = 1e15;
inline int read() {
    char c = getchar();
    int x = 0, f = 1;
    while (c < '0' || c > '9') {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    return x * f;
}
int n, m, type;
double a[N][N], ans[N];
int id[N << 1];
int q[N];
void Pivot(int l, int e) {
    swap(id[n + l], id[e]);
    double t = a[l][e];
    a[l][e] = 1;
    for (int j = 0; j <= n; j++) a[l][j] /= t;
    for (int i = 0; i <= m; i++)
        if (i != l && abs(a[i][e]) > eps) {
            t = a[i][e];
            a[i][e] = 0;
            for (int j = 0; j <= n; j++) a[i][j] -= a[l][j] * t;
        }
}
bool init() {
    while (true) {
        int e = 0, l = 0;
        for (int i = 1; i <= m; i++)
            if (a[i][0] < -eps && (!l || (rand() & 1))) l = i;
        if (!l) break;
        for (int j = 1; j <= n; j++)
            if (a[l][j] < -eps && (!e || (rand() & 1))) e = j;
        if (!e) {
            puts("Infeasible");
            return false;
        }  //不存在满足约束的解
        Pivot(l, e);
    }
    return true;
}
bool simplex() {
    while (true) {
        int l = 0, e = 0;
        double mn = INF;
        for (int j = 1; j <= n; j++)
            if (a[0][j] > eps) {
                e = j;
                break;
            }
        if (!e) break;
        for (int i = 1; i <= m; i++)
            if (a[i][e] > eps && a[i][0] / a[i][e] < mn)
                mn = a[i][0] / a[i][e], l = i;
        if (!l) {
            puts("Unbounded");
            return false;
        }  //目标值趋于无穷
        Pivot(l, e);
    }
    return true;
}
int main() {
    srand(317);
    n = read();
    m = read();
    type = read();
    for (int i = 1; i <= n; i++) a[0][i] = read();  //目标系数
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) a[i][j] = read();  //线性约束系数
        a[i][0] = read();                               //小于等于约束
    }
    for (int i = 1; i <= n; i++) id[i] = i;
    if (init() && simplex()) {
        printf("%.8lf\n", -a[0][0]);
        if (type) {
            for (int i = 1; i <= m; i++) ans[id[n + i]] = a[i][0];
            for (int i = 1; i <= n; i++) printf("%.8lf ", ans[i]);
        }
    }
    return 0;
}
