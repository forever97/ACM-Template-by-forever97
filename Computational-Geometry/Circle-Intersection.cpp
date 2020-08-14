/*
    求圆交
    Example：
        给出两个圆环，圆心位置不同，但是大小圆半径相同，求两个圆环相交的面积
    Solution：
        面积为大圆交面积-2*大小圆交面积+小圆交面积
*/
using namespace std;
const double EPS = 1e-12;
const double PI = acos(-1.0);
const int inf = ~0U >> 1;
int sgn(double x) {
    if (fabs(x) < EPS) return 0;
    if (x < 0)
        return -1;
    else
        return 1;
}
struct Point {
    double x, y;
    Point() {}
    Point(double x1, double y1) {
        x = x1;
        y = y1;
    }
    Point operator-(const Point &b) const { return Point(x - b.x, y - b.y); }
    double operator*(const Point &b) const { return x * b.x + y * b.y; }  //点积
    double operator^(const Point &b) const { return x * b.y - y * b.x; }  //叉积
};
struct Line {
    Point u, v;
    Line() {}
    Line(Point u1, Point v1) {
        u = u1;
        v = v1;
    }
};
double dist(Point a, Point b) { return sqrt((a - b) * (a - b)); }
// 两圆相交部分面积，a，b是圆心，r1，r2是半径
double Area(Point a, double r1, Point b, double r2) {
    double k = dist(a, b);
    if (k + EPS >= r1 + r2) return 0;  //相切或者相离
    if (k <= fabs(r1 - r2) + EPS) {
        double R = min(r1, r2);
        return PI * R * R;
    }
    double x = (k * k + r1 * r1 - r2 * r2) / (2.0 * k);
    double w1 = acos(x / r1);
    double w2 = acos((k - x) / r2);
    return (w1 * r1 * r1 + w2 * r2 * r2 - k * r1 * sin(w1));
}
int main(int T) {
    scanf("%d", &T);
    int cnt = 1;
    while (T--) {
        double x, y, r, R;
        Point p1, p2;
        scanf("%lf%lf", &r, &R);
        scanf("%lf%lf", &x, &y);
        p1 = Point(x, y);
        scanf("%lf%lf", &x, &y);
        p2 = Point(x, y);
        double ans =
            Area(p1, R, p2, R) - 2 * Area(p1, R, p2, r) + Area(p1, r, p2, r);
        printf("Case #%d: %.6f\n", cnt++, ans);
    }
    return 0;
}