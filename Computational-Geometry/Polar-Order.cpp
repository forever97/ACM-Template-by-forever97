/*
    题目大意：一个二维平面上有一些黑点和白点，现在用一块木板去划分他们，
    木板A面的每个黑点计一分，B面每个白点计一分，木板上每个点计一分，
    求最高得分
    题解：我们枚举木板的旋转点，将所有黑点关于旋转点做轴对称，
    那么问题就转化为统计旋转线一面的点数量，对于枚举的旋转点对剩余点做极角排序，
    之后做旋转线扫描，统计扫描线上方点和下方点，更新答案即可。
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
using namespace std;
const double eps = 1e-8;
const int N = 1010;
int n;
int sgn(double x) {
    if (x >= eps) return 1;
    if (x <= -eps) return -1;
    return 0;
}
struct Point {
    int x, y, k;
    double ang;
    Point(){};
    Point(int _x, int _y) : x(_x), y(_y) {}
} p0[N], p[N];
Point operator-(Point a, Point b) { return Point(a.x - b.x, a.y - b.y); }
int Cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
bool cmp(Point a, Point b) { return a.ang < b.ang; }
int main() {
    while (scanf("%d", &n), n) {
        int ans = 0;
        for (int i = 1; i <= n; i++) {
            scanf("%d%d%d", &p[i].x, &p[i].y, &p[i].k);
            p0[i] = p[i];
        }
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                p[j] = p0[j];
                if (p[j].k) {  //  权值为1的点关于旋转点对称
                    p[j].x = p0[i].x * 2 - p[j].x;
                    p[j].y = p0[i].y * 2 - p[j].y;
                }
                p[j].ang = atan2(p[j].y - p0[i].y, p[j].x - p0[i].x);
            }
            swap(p[i], p[1]);
            sort(p + 2, p + n + 1, cmp);
            for (int s = 2, t = 3; s <= n && sgn(p[s].ang) <= 0;
                 s++) {  //  sgn<=0来防止重复枚举线
                int on = 2;
                for (; t <= n && Cross(p[s] - p[1], p[t] - p[1]) >= 0; t++) {
                    if (Cross(p[s] - p[1], p[t] - p[1]) == 0) on++;
                }
                ans = max(ans, max(t - s + 1, n - (t - s + 1) + on));
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
/*
    题目大意：给出n个点，求点集的每个子集能构成的凸包的面积和的两倍
    题解：凸包面积两倍可以由凸包上每条有向边的两点与原点构成向量计算叉积的和来表示，
    我们枚举旋转点，对点集进行旋转线扫描，对于每条边，我们计算其逆时针180度以内点的个数x，
    那么这条边的贡献就是其端点与原点叉积乘上(pw[x]-1)，即将其作为凸包边的子集个数，
    我们只要枚举每个旋转点，对有向边进行线扫描，统计贡献和即可。
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
using namespace std;
typedef long long LL;
const int N = 1010;
const LL mod = 998244353;
const double PI = acos(-1.0);
LL pw2[N];
int T, n;
struct Point {
    LL x, y;
    double ang;
    Point(){};
    Point(LL _x, LL _y) : x(_x), y(_y) {}
} p0[N], p[N << 1];
Point operator-(Point a, Point b) { return Point(a.x - b.x, a.y - b.y); }
LL Cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
bool cmp(Point a, Point b) { return a.ang < b.ang; }
int main() {
    for (int i = pw2[0] = 1; i < N; i++) pw2[i] = (pw2[i - 1] << 1) % mod;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        LL ans = 0;
        for (int i = 1; i <= n; i++) {
            scanf("%lld%lld", &p[i].x, &p[i].y);
            p0[i] = p[i];
        }
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                p[j] = p0[j];
                p[j].ang = atan2(p[j].y - p0[i].y, p[j].x - p0[i].x);
            }
            swap(p[i], p[1]);
            sort(p + 2, p + n + 1, cmp);
            for (int j = 1; j < n; j++)
                p[n + j] = p[j + 1], p[n + j].ang += 2.0 * PI;
            for (int s = 2, t = 2; s <= n; s++) {
                while (p[t].ang < p[s].ang + PI) t++;
                LL ctb = (pw2[t - s - 1] - 1) *
                         ((Cross(p[1], p[s]) % mod + mod) % mod) % mod;
                ans = (ans + ctb) % mod;
            }
        }
        printf("%lld\n", ans);
    }
    return 0;
}
