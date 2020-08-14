/*
    凸包面积交
    逆时针输入两个凸包，求相交的面积
    用一个凸包的边不断去切割另一个凸包，保留切割线内侧的点和交点
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
using namespace std;
const int N = 1010;
struct Point {
    double x, y;
} p[N], q[N], t[N];
int T, n, m;
double a, b, c;
double Cal(Point p1) { return a * p1.x + b * p1.y + c; }
void Get_line(Point p1, Point p2) {
    a = p2.y - p1.y;
    b = p1.x - p2.x;
    c = p2.x * p1.y - p2.y * p1.x;
}
Point Intersetion(Point p1, Point p2) {
    Point ret;
    double u = fabs(Cal(p1)), v = fabs(Cal(p2));
    ret.x = (v * p1.x + u * p2.x) / (u + v);
    ret.y = (v * p1.y + u * p2.y) / (u + v);
    return ret;
}
void Cut() {
    int tm = 0;
    for (int i = 1; i <= m; i++) {
        if (Cal(q[i]) <= 0)
            t[++tm] = q[i];
        else {
            if (Cal(q[i - 1]) < 0) t[++tm] = Intersetion(q[i - 1], q[i]);
            if (Cal(q[i + 1]) < 0) t[++tm] = Intersetion(q[i], q[i + 1]);
        }
    }
    for (int i = 1; i <= tm; i++) q[i] = t[i];
    q[0] = q[tm];
    q[tm + 1] = q[1];
    m = tm;
}
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &m, &n);
        for (int i = 1; i <= m; i++) scanf("%lf%lf", &q[i].x, &q[i].y);
        q[0] = q[m], q[m + 1] = q[1];
        for (int i = 1; i <= n; i++) scanf("%lf%lf", &p[i].x, &p[i].y);
        p[n + 1] = p[1];
        for (int i = 1; i <= n; i++) {
            Get_line(p[i], p[i + 1]);
            Cut();
        }
        double ans = 0;
        for (int i = 1; i <= m; i++)
            ans += q[i].x * q[i + 1].y - q[i + 1].x * q[i].y;
        printf("%.4f\n", ans / 2);
    }
    return 0;
}
