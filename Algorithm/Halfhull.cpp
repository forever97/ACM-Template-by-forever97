/*
    凸包+分治
    分上下凸壳维护
*/
/*
    Problem：给出一个直线集Ax+B，现在给出一些查询，每次查询一条直线Cx+D，
    问与直线集中直线交点最大的x是多少
    Solution：我们得到交点x=-(B-D)/(A-C)，我们对所有的A和C取负，
    那么答案就是求点(-Cq,Dq)到点集(-Ai,Bi)的最大斜率，
    我们按照横纵坐标排序，维护上左凸壳和下右凸壳，询问点到凸壳的斜率是一个单峰函数，
    可以三分找到最大斜率的位置，也可以二分找到点到mid和mid+1大小状况突变的位置
*/
// 三分
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 100010;
const double eps = 1e-8;
struct Point {
    int x, y, id;
} p[N], st[N];
bool cmp(Point a, Point b) {
    if (a.x == b.x) return a.y < b.y;
    return a.x < b.x;
}
Point operator-(const Point &a, const Point &b) {
    return (Point){a.x - b.x, a.y - b.y};
}
LL operator^(const Point &a, const Point &b) {
    return 1LL * a.x * b.y - 1LL * a.y * b.x;
}
double GetK(Point p) { return 1.0 * p.y / p.x; }
double ans[N];
void Halfhull(int n) {
    int top = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].id) {
            int l = 0, r = top - 1;
            while (l < r) {
                int m1 = (l + l + r) / 3, m2 = (l + r + r + 2) / 3;
                if (((p[i] - st[m1]) ^ (p[i] - st[m2])) < 0)
                    r = m2 - 1;
                else
                    l = m1 + 1;
            }
            if (l < top) ans[p[i].id] = max(ans[p[i].id], GetK(p[i] - st[l]));
        } else {
            while (top > 1 &&
                   ((st[top - 1] - st[top - 2]) ^ (p[i] - st[top - 2])) <= 0)
                top--;
            st[top++] = p[i];
        }
    }
}
void Convexhull(int n) {
    sort(p, p + n, cmp);
    Halfhull(n);
    reverse(p, p + n);
    Halfhull(n);
}
int n, m;
int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d%d", &p[i].x, &p[i].y);
    scanf("%d", &m);
    for (int i = 0; i < m; i++)
        scanf("%d%d", &p[n + i].x, &p[n + i].y), p[n + i].id = i + 1;
    for (int i = 0; i < n + m; i++) p[i].x = -p[i].x;
    Convexhull(n + m);
    for (int i = 1; i <= m; i++) {
        if (ans[i] < eps)
            puts("No cross");
        else
            printf("%.10f\n", ans[i]);
    }
    return 0;
}
// 二分
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 100010;
const double eps = 1e-8;
struct Point {
    int x, y, id;
} p[N], st[N];
bool cmp(Point a, Point b) {
    if (a.x == b.x) return a.y < b.y;
    return a.x < b.x;
}
Point operator-(const Point &a, const Point &b) {
    return (Point){a.x - b.x, a.y - b.y};
}
LL operator^(const Point &a, const Point &b) {
    return 1LL * a.x * b.y - 1LL * a.y * b.x;
}
double GetK(Point p) { return 1.0 * p.y / p.x; }
double ans[N];
void Halfhull(int n) {
    int top = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].id) {
            int l = 0, r = top - 1;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (((p[i] - st[mid]) ^ (p[i] - st[mid + 1])) <= 0)
                    r = mid;
                else
                    l = mid + 1;
            }
            if (l < top) ans[p[i].id] = max(ans[p[i].id], GetK(p[i] - st[l]));
        } else {
            while (top > 1 &&
                   ((st[top - 1] - st[top - 2]) ^ (p[i] - st[top - 2])) <= 0)
                top--;
            st[top++] = p[i];
        }
    }
}
void Convexhull(int n) {
    sort(p, p + n, cmp);
    Halfhull(n);
    reverse(p, p + n);
    Halfhull(n);
}
int n, m;
int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d%d", &p[i].x, &p[i].y);
    scanf("%d", &m);
    for (int i = 0; i < m; i++)
        scanf("%d%d", &p[n + i].x, &p[n + i].y), p[n + i].id = i + 1;
    for (int i = 0; i < n + m; i++) p[i].x = -p[i].x;
    Convexhull(n + m);
    for (int i = 1; i <= m; i++) {
        if (ans[i] < eps)
            puts("No cross");
        else
            printf("%.10f\n", ans[i]);
    }
    return 0;
}
