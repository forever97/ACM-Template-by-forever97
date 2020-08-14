/*
    凸包面积并
*/
#include <bits/stdc++.h>
#define all(x) x.begin(), x.end()
using namespace std;
const double pi = acos(-1.0);
typedef long long LL;
typedef unsigned long long ULL;
void umax(int &a, int b) { a = max(a, b); }
void umin(int &a, int b) { a = min(a, b); }
namespace Solver {
#define PDI pair<double, int>
#define point pair<double, double>
#define mp make_pair
#define pb push_back
#define x first
#define y second
#define zero 1e-8
#define maxN 111
#define maxp 30
point operator+(point a, point b) { return mp(a.x + b.x, a.y + b.y); }
point operator-(point a, point b) { return mp(a.x - b.x, a.y - b.y); }
double operator*(point a, point b) { return a.x * b.y - b.x * a.y; }
double operator^(point a, point b) { return a.x * b.x + a.y * b.y; }
inline double cross(point o, point a, point b) { return (a - o) * (b - o); }
inline int cmp(double x) {
    if (fabs(x) < zero) return 0;
    return x > 0 ? 1 : -1;
}
class Polygon {
   private:
    int i;
    double s;

   public:
    int n;
    point p[maxp];
    point &operator[](int idx) { return p[idx]; }
    void input() {
        for (i = 0; i < n; i++) scanf("%lf %lf", &p[i].x, &p[i].y);
        p[n] = p[0];
    }
    double Area() {
        for (s = 0, i = 0; i < n; i++) s += p[i] * p[i + 1];
        return s / 2;
    }
};
PDI s[maxN * maxp * 2];
Polygon P[maxN];
double S, ts;
int N;
inline double seg(point o, point a, point b) {
    if (cmp(b.x - a.x) == 0) return (o.y - a.y) / (b.y - a.y);
    return (o.x - a.x) / (b.x - a.x);
}
double PolygonUnion() {
    int M, c1, c2;
    double s1, s2, ret = 0;
    for (int i = 0; i < N; i++) {
        for (int ii = 0; ii < P[i].n; ii++) {
            M = 0;
            s[M++] = mp(0.00, 0);
            s[M++] = mp(1.00, 0);
            for (int j = 0; j < N; j++)
                if (i != j)
                    for (int jj = 0; jj < P[j].n; jj++) {
                        c1 = cmp(cross(P[i][ii], P[i][ii + 1], P[j][jj]));
                        c2 = cmp(cross(P[i][ii], P[i][ii + 1], P[j][jj + 1]));
                        if (c1 == 0 && c2 == 0) {
                            if (((P[i][ii + 1] - P[i][ii]) ^
                                 (P[j][jj + 1] - P[j][jj])) > 0 &&
                                i > j) {
                                s[M++] = mp(
                                    seg(P[j][jj], P[i][ii], P[i][ii + 1]), 1);
                                s[M++] = mp(
                                    seg(P[j][jj + 1], P[i][ii], P[i][ii + 1]),
                                    -1);
                            }
                        } else {
                            s1 = cross(P[j][jj], P[j][jj + 1], P[i][ii]);
                            s2 = cross(P[j][jj], P[j][jj + 1], P[i][ii + 1]);
                            if (c1 >= 0 && c2 < 0)
                                s[M++] = mp(s1 / (s1 - s2), 1);
                            else if (c1 < 0 && c2 >= 0)
                                s[M++] = mp(s1 / (s1 - s2), -1);
                        }
                    }
            sort(s, s + M);
            double pre = min(max(s[0].x, 0.0), 1.0), now;
            double sum = 0;
            int cov = s[0].y;
            for (int j = 1; j < M; j++) {
                now = min(max(s[j].x, 0.0), 1.0);
                if (!cov) sum += now - pre;
                cov += s[j].y;
                pre = now;
            }
            ret += P[i][ii] * P[i][ii + 1] * sum;
        }
    }
    return ret / 2;
}
void solve() {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &P[i].n);
        P[i].input();
        S -= P[i].Area();
        reverse(P[i].p, P[i].p + P[i].n);
        P[i][P[i].n] = P[i][0];
    }
    // 凸包面积和，凸包面积并
    printf("%.9f %.9f\n", S, PolygonUnion());
}
};  // namespace Solver
int main() {
    Solver::solve();
    return 0;
}