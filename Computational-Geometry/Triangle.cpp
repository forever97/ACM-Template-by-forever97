/*
    求两个三角形的关系判断
    contain：内含
    intersect：相交
    disjoint：相离
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
using namespace std;
const double EPS = 1e-12;
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
    Point operator+(const Point &b) const { return Point(x + b.x, y + b.x); }
    Point operator-(const Point &b) const { return Point(x - b.x, y - b.y); }
    double operator*(const Point &b) const { return x * b.x + y * b.y; }  //点积
    double operator^(const Point &b) const { return x * b.y - y * b.x; }  //叉积
    Point operator/(const double b) const { return Point(x / b, y / b); }
    //绕原点旋转角度B（弧度值），后x,y的变化
    void transXY(double B) {
        double tx = x, ty = y;
        x = tx * cos(B) - ty * sin(B);
        y = tx * sin(B) + ty * cos(B);
    }
};
double dist(Point a, Point b) { return sqrt((a - b) * (a - b)); }
//直线类，包含位置判断
struct Line {
    Point u, v;
    Line() {}
    Line(Point u1, Point v1) {
        u = u1;
        v = v1;
    }
    pair<Point, int> operator&(const Line &b) const {
        Point res = u;
        if (sgn((u - v) ^ (b.u - b.v)) == 0) {
            if (sgn((b.u - u) ^ (b.v - u)) == 0)
                return make_pair(res, 0);  //重合
            else
                return make_pair(res, 1);  //平行
        }
        double k = ((u - b.u) ^ (b.u - b.v)) / ((u - v) ^ (b.u - b.v));
        res.x += (v.x - u.x) * k;
        res.y += (v.y - u.y) * k;
        return make_pair(res, 2);  //相交
    }
};
//判断线段是否相交
bool inter(Line l1, Line l2) {
    return max(l1.u.x, l1.v.x) >= min(l2.u.x, l2.v.x) &&
           max(l2.u.x, l2.v.x) >= min(l1.u.x, l1.v.x) &&
           max(l1.u.y, l1.v.y) >= min(l2.u.y, l2.v.y) &&
           max(l2.u.y, l2.v.y) >= min(l1.u.y, l1.v.y) &&
           sgn((l2.u - l1.u) ^ (l1.v - l1.u)) *
                   sgn((l2.v - l1.u) ^ (l1.v - l1.u)) <=
               0 &&
           sgn((l1.u - l2.u) ^ (l2.v - l2.u)) *
                   sgn((l1.v - l2.u) ^ (l2.v - l2.u)) <=
               0;
}
//判断点在线段上
bool OnSeg(Point P, Line L) {
    return sgn((L.u - P) ^ (L.v - P)) == 0 &&
           sgn((P.x - L.u.x) * (P.x - L.v.x)) <= 0 &&
           sgn((P.y - L.u.y) * (P.y - L.v.y)) <= 0;
}
/*
  inConvexPoly:判断点在凸多边形内
    点形成一个凸包，而且按逆时针排序（如果是顺时针把里面的<0改为>0）
    点的编号:0~n-1
    返回值：
      -1:点在凸多边形外
      0:点在凸多边形边界上
      1:点在凸多边形内
*/
int inConvexPoly(Point a, Point p[], int n) {
    for (int i = 0; i < n; i++) {
        if (sgn((p[i] - a) ^ (p[(i + 1) % n] - a)) < 0)
            return -1;
        else if (OnSeg(a, Line(p[i], p[(i + 1) % n])))
            return 0;
    }
    return 1;
}
Point A[10], B[10];
bool contain(Point A[], Point B[]) {
    int flag = 1;
    for (int i = 0; i < 3; i++)
        if (inConvexPoly(A[i], B, 3) != 1) flag = 0;
    return flag;
}
bool Intersect() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (inter(Line(A[i], A[(i + 1) % 3]), Line(B[j], B[(j + 1) % 3])))
                return 1;
    return 0;
}
Point o;
bool _cmp(Point p1, Point p2) {
    double tmp = (p1 - o) ^ (p2 - o);
    if (sgn(tmp) > 0)
        return true;
    else if (sgn(tmp) == 0 && sgn(dist(p1, o) - dist(p2, o)) <= 0)
        return true;
    else
        return false;
}
int T;
int main() {
    scanf("%d", &T);
    while (T--) {
        for (int i = 0; i < 3; i++) scanf("%lf%lf", &A[i].x, &A[i].y);
        o = (A[0] + A[1] + A[2]) / 3.0;
        sort(A, A + 3, _cmp);
        for (int i = 0; i < 3; i++) scanf("%lf%lf", &B[i].x, &B[i].y);
        o = (B[0] + B[1] + B[2]) / 3.0;
        sort(B, B + 3, _cmp);
        if (contain(A, B) || contain(B, A))
            puts("contain");
        else if (Intersect())
            puts("intersect");
        else
            puts("disjoint");
    }
    return 0;
}
