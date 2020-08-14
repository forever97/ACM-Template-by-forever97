/*
    半平面交+凸包内缩
*/
#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-8;
const double PI = acos(-1.0);
const int N = 1010;
int sgn(double x) {
    if (fabs(x) < eps) return 0;
    if (x < 0)
        return -1;
    else
        return 1;
}
struct Point {
    double x, y;
    Point() {}
    Point(double _x, double _y) {
        x = _x;
        y = _y;
    }
    Point operator-(const Point &b) const { return Point(x - b.x, y - b.y); }
    double operator^(const Point &b) const { return x * b.y - y * b.x; }
    double operator*(const Point &b) const { return x * b.x + y * b.y; }
};
int dist2(Point a, Point b) { return (a - b) * (a - b); }
struct Line {
    Point s, e;
    double k;
    Line() {}
    Line(Point _s, Point _e) {
        s = _s;
        e = _e;
        k = atan2(e.y - s.y, e.x - s.x);
    }
    Point operator&(const Line &b) const {
        Point res = s;
        double t = ((s - b.s) ^ (b.s - b.e)) / ((s - e) ^ (b.s - b.e));
        res.x += (e.x - s.x) * t;
        res.y += (e.y - s.y) * t;
        return res;
    }
};
// 半平面交，直线的左边代表有效区域
bool HPIcmp(Line a, Line b) {
    if (fabs(a.k - b.k) > eps) return a.k < b.k;
    return ((a.s - b.s) ^ (b.e - b.s)) < 0;
}
Line Q[N];
void HPI(Line line[], int n, Point res[], int &resn) {
    int tot = n;
    sort(line, line + n, HPIcmp);
    tot = 1;
    for (int i = 1; i < n; i++)
        if (fabs(line[i].k - line[i - 1].k) > eps) line[tot++] = line[i];
    int head = 0, tail = 1;
    Q[0] = line[0];
    Q[1] = line[1];
    resn = 0;
    for (int i = 2; i < tot; i++) {
        if (fabs((Q[tail].e - Q[tail].s) ^ (Q[tail - 1].e - Q[tail - 1].s)) <
                eps ||
            fabs((Q[head].e - Q[head].s) ^ (Q[head + 1].e - Q[head + 1].s)) <
                eps)
            return;
        while (head < tail && (((Q[tail] & Q[tail - 1]) - line[i].s) ^
                               (line[i].e - line[i].s)) > eps)
            tail--;
        while (head < tail && (((Q[head] & Q[head + 1]) - line[i].s) ^
                               (line[i].e - line[i].s)) > eps)
            head++;
        Q[++tail] = line[i];
    }
    while (head < tail && (((Q[tail] & Q[tail - 1]) - Q[head].s) ^
                           (Q[head].e - Q[head].s)) > eps)
        tail--;
    while (head < tail && (((Q[head] & Q[head - 1]) - Q[tail].s) ^
                           (Q[tail].e - Q[tail].e)) > eps)
        head++;
    if (tail <= head + 1) return;
    for (int i = head; i < tail; i++) res[resn++] = Q[i] & Q[i + 1];
    if (head < tail - 1) res[resn++] = Q[head] & Q[tail];
}
Point p[N];
Line line[N];
// 两点间距离
double dist(Point a, Point b) { return sqrt((a - b) * (a - b)); }
// 将线段ab往左移动距离p
void change(Point a, Point b, Point &c, Point &d, double p) {
    double len = dist(a, b);
    double dx = (a.y - b.y) * p / len;
    double dy = (b.x - a.x) * p / len;
    c.x = a.x + dx;
    c.y = a.y + dy;
    d.x = b.x + dx;
    d.y = b.y + dy;
}
/*
    Problem：
        给出一个凸包，现在要往里面放三个半径为r的圆，圆可以相交，
        求三个圆圆心相连构成的三角形最大面积的两倍
    Solution:
        半平面交获取圆心可放置位置的边界，对凸包顶点用旋转卡壳求最大面积三角形
*/
// 旋转卡壳，求三角形面积最大值的两倍
double rotating_calipers(Point p[], int n) {
    double ans = 0;
    Point v;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int k = (j + 1) % n;
        while (j != i && k != i) {
            ans = max(ans, abs((p[j] - p[i]) ^ (p[k] - p[i])));
            while (((p[i] - p[j]) ^ (p[(k + 1) % n] - p[k])) < 0)
                k = (k + 1) % n;
            j = (j + 1) % n;
        }
    }
    return ans;
}
// 求凸包
Point List[N];
int Stack[N], top;  // top为凸包的大小
bool _cmp(Point p1, Point p2) {
    double tmp = (p1 - List[0]) ^ (p2 - List[0]);
    if (tmp > 0)
        return true;
    else if (tmp == 0 && dist2(p1, List[0]) <= dist2(p2, List[0]))
        return true;
    else
        return false;
}
void Graham(int n) {
    Point p0;
    int k = 0;
    p0 = List[0];
    for (int i = 1; i < n; i++)
        if (p0.y > List[i].y || (p0.y == List[i].y && p0.x > List[i].x)) {
            p0 = List[i];
            k = i;
        }
    swap(List[0], List[k]);
    sort(List + 1, List + n, _cmp);
    if (n == 1) {
        top = 1;
        Stack[0] = 0;
        return;
    }
    if (n == 2) {
        top = 2;
        Stack[0] = 0;
        Stack[1] = 1;
        return;
    }
    Stack[0] = 0;
    Stack[1] = 1;
    top = 2;
    for (int i = 2; i < n; i++) {
        while (top > 1 && ((List[Stack[top - 1]] - List[Stack[top - 2]]) ^
                           (List[i] - List[Stack[top - 2]])) <= 0)
            top--;
        Stack[top++] = i;
    }
}
int main() {
    int n, T;
    double r;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%lf", &n, &r);
        for (int i = 0; i < n; i++) scanf("%lf%lf", &p[i].x, &p[i].y);
        reverse(p, p + n);
        for (int i = 0; i < n; i++) {
            Point t1, t2;
            change(p[i], p[(i + 1) % n], t1, t2, r);
            line[i] = Line(t1, t2);
        }
        int resn;
        HPI(line, n, p, resn);
        for (int i = 0; i < resn; i++) List[i].x = p[i].x, List[i].y = p[i].y;
        Graham(resn);
        for (int i = 0; i < top; i++) p[i] = List[Stack[i]];
        double ans = rotating_calipers(p, top);  // 注意参数
        printf("%.12f\n", ans);
    }
    return 0;
}
