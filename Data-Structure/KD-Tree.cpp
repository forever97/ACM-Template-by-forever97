/*
    KD树
    支持暴力重构
    初始化只需要把dcnt置0
    KD_Tree::Insert(root,0,KD_Tree::Dot(x0,y0,c))
    if(KD_Tree::dcnt%5000==0)root=KD_Tree::Rebuild(1,KD_Tree::dcnt,0)
    KD_Tree::query(root,x0,y0,x1,y1)
    Attention：有些数据情况，直接插入点不重构效率更高
*/
namespace KD_Tree {
struct Dot {
    int d[2], mn[2], mx[2], l, r, sz, sum, val;
    Dot() { l = r = 0; }
    Dot(int x, int y, int c) {
        d[0] = x;
        d[1] = y;
        val = c;
        l = r = sz = sum = 0;
    }
    int& operator[](int x) { return d[x]; }
};
int D, dcnt = 0, pt[N];
Dot T[N];
bool operator<(Dot a, Dot b) { return a[D] < b[D]; }
inline void umax(int& a, int b) {
    if (a < b) a = b;
}
inline void umin(int& a, int b) {
    if (a > b) a = b;
}
inline bool cmp(int x, int y) { return T[x][D] < T[y][D]; }
inline void up(int x) {
    T[x].sz = T[T[x].l].sz + T[T[x].r].sz + 1;
    T[x].sum = T[T[x].l].sum + T[T[x].r].sum + T[x].val;
    T[x].mn[0] = T[x].mx[0] = T[x][0];
    T[x].mn[1] = T[x].mx[1] = T[x][1];
    if (T[x].l) {
        umax(T[x].mx[0], T[T[x].l].mx[0]);
        umin(T[x].mn[0], T[T[x].l].mn[0]);
        umax(T[x].mx[1], T[T[x].l].mx[1]);
        umin(T[x].mn[1], T[T[x].l].mn[1]);
    }
    if (T[x].r) {
        umax(T[x].mx[0], T[T[x].r].mx[0]);
        umin(T[x].mn[0], T[T[x].r].mn[0]);
        umax(T[x].mx[1], T[T[x].r].mx[1]);
        umin(T[x].mn[1], T[T[x].r].mn[1]);
    }
}
// NewDot中所有用到的参数一定要初始化
inline int NewDot(int x, int y, int c) {
    ++dcnt;
    pt[dcnt] = dcnt;
    T[dcnt][0] = x;
    T[dcnt][1] = y;
    T[dcnt].val = c;
    T[dcnt].l = T[dcnt].r = T[dcnt].sz = 0;
    return up(dcnt), dcnt;
}
// 离线建树时，只加点，不构树，最后build
inline void AddDot(int x, int y, int c) {
    ++dcnt;
    pt[dcnt] = dcnt;
    p[dcnt][0] = x;
    p[dcnt][1] = y;
    p[dcnt].val = c;
}
// 查询矩阵内数字和
int query(int x, int x0, int y0, int x1, int y1) {
    if (!x || T[x].mn[0] > x1 || T[x].mx[0] < x0 || T[x].mn[1] > y1 ||
        T[x].mx[1] < y0)
        return 0;
    if (T[x].mn[0] >= x0 && T[x].mx[0] <= x1 && T[x].mn[1] >= y0 &&
        T[x].mx[1] <= y1)
        return T[x].sum;
    int res = 0;
    if (T[x][0] >= x0 && T[x][0] <= x1 && T[x][1] >= y0 && T[x][1] <= y1)
        res += T[x].val;
    return res + query(T[x].l, x0, y0, x1, y1) + query(T[x].r, x0, y0, x1, y1);
}
LL sqr(LL x) { return x * x; }
// 欧几里得上界估价函数（K远点对）
inline LL dist(int x, int px, int py) {
    LL dis = 0;
    if (!x) return 0;
    dis += max(sqr(T[x].mn[0] - px), sqr(T[x].mx[0] - px));
    dis += max(sqr(T[x].mn[1] - py), sqr(T[x].mx[1] - py));
    return dis;
}
}  // namespace KD_Tree
// 曼哈顿距离估价函数
inline int dist(int p1, int px, int py) {
    int dis = 0;
    if (px < T[p1].mn[0]) dis += T[p1].mn[0] - px;
    if (px > T[p1].mx[0]) dis += px - T[p1].mx[0];
    if (py < T[p1].mn[1]) dis += T[p1].mn[1] - py;
    if (py > T[p1].mx[1]) dis += py - T[p1].mx[1];
    return dis;
}
// 欧几里得距离估价函数
inline LL dist(int p1, int px, int py) {
    LL dis = 0;
    if (px < T[p1].mn[0]) dis += sqr(T[p1].mn[0] - px);
    if (px > T[p1].mx[0]) dis += sqr(px - T[p1].mx[0]);
    if (py < T[p1].mn[1]) dis += sqr(T[p1].mn[1] - py);
    if (py > T[p1].mx[1]) dis += sqr(py - T[p1].mx[1]);
    return dis;
}
// 查询(px,py)最近点距离（曼哈顿距离为例）
int ans = 0;
inline void ask(int x, int px, int py) {
    int dl, dr, d0 = abs(T[x][0] - px) + abs(T[x][1] - py);
    if (d0 < ans) ans = d0;
    dl = T[x].l ? dist(T[x].l, px, py) : INF;
    dr = T[x].r ? dist(T[x].r, px, py) : INF;
    if (dl < dr) {
        if (dl < ans) ask(T[x].l, px, py);
        if (dr < ans) ask(T[x].r, px, py);
    } else {
        if (dr < ans) ask(T[x].r, px, py);
        if (dl < ans) ask(T[x].l, px, py);
    }
}
int Querymin(int x, int px, int py) {
    ans = INF;
    ask(x, px, py);
    return ans;
}
// 查询离树上的某点距离最近的点的id（欧式距离为例）
LL ans, ans2;
Dot ansP, ansP2;
inline void ask(int x, int px, int py) {
    LL dl, dr, d0 = sqr(T[x][0] - px) + sqr(T[x][1] - py);
    if (d0 < ans || (d0 == ans && T[x] < ansP))
        ans2 = ans, ansP2 = ansP, ans = d0, ansP = T[x];
    else if (d0 < ans2 || (d0 == ans2 && T[x] < ansP2))
        ans2 = d0, ansP2 = T[x];
    dl = T[x].l ? dist(T[x].l, px, py) : INF;
    dr = T[x].r ? dist(T[x].r, px, py) : INF;
    if (dl < dr) {
        if (dl <= ans2) ask(T[x].l, px, py);
        if (dr <= ans2) ask(T[x].r, px, py);
    } else {
        if (dr <= ans2) ask(T[x].r, px, py);
        if (dl <= ans2) ask(T[x].l, px, py);
    }
}
int getP(int x, int px, int py) {
    ans = ans2 = INF;
    ask(root);
    return ansP2.id;
}
// 在线插入元素
void Insert(int& x, int D, const Dot& p) {
    if (!x) {
        x = NewDot(p.d[0], p.d[1], p.val);
        return;
    }
    if (p.d[D] < T[x][D])
        Insert(T[x].l, D ^ 1, p);
    else
        Insert(T[x].r, D ^ 1, p);
    up(x);
}
// 暴力重构
int Rebuild(int l, int r, int now) {
    if (l > r) return 0;
    int mid = (l + r) >> 1, x;
    D = now;
    nth_element(pt + l, pt + mid, pt + r + 1, cmp);
    x = pt[mid];
    T[x].l = Rebuild(l, mid - 1, now ^ 1);
    T[x].r = Rebuild(mid + 1, r, now ^ 1);
    return up(x), x;
}
// 建树
int build(int l, int r, int now) {
    int mid = (l + r) >> 1;
    D = now;
    nth_element(p + l, p + mid, p + r + 1);
    T[mid] = p[mid];
    for (int i = 0; i < 2; i++) T[mid].mn[i] = T[mid].mx[i] = T[mid][i];
    if (l < mid) T[mid].l = build(l, mid - 1, now ^ 1);
    if (r > mid) T[mid].r = build(mid + 1, r, now ^ 1);
    return up(mid), mid;
}
}
/*
    估价函数：
    欧几里得距离下界：
    sqr(max(max(X-x.Max[0],x.Min[0]-X),0))+sqr(max(max(Y-x.Max[1],x.Min[1]-Y),0))
    曼哈顿距离下界：
    max(x.Min[0]-X,0)+max(X-x.Max[0],0)+max(x.Min[1]-Y,0)+max(Y-x.Max[1],0)
    欧几里得距离上界：
    max(sqr(X-x.Min[0]),sqr(X-x.Max[0]))+max(sqr(Y-x.Min[1]),sqr(Y-x.Max[1])
    曼哈顿距离上界：
    max(abs(X-x.Max[0]),abs(x.Min[0]-X))+max(abs(Y-x.Max[1]),abs(x.Min[1]-Y))
*/
/*
    Example：
    给出一些点，给出与x轴交点为LA，与y轴交点为LB的直线，问有多少点在这条直线上
    因为点随机生成，所以我们对于给出的点建立KD树，
    每次查询直线时，若查询直线与当前点子树的最小包围矩形不相交，则剪枝
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int N = 100010;
int ans;
namespace KD_Tree {
struct Dot {
    int d[2], mn[2], mx[2], l, r, sz, sum, val;
    Dot() { l = r = 0; }
    Dot(int x, int y, int c) {
        d[0] = x;
        d[1] = y;
        val = c;
        l = r = sz = sum = 0;
    }
    int& operator[](int x) { return d[x]; }
};
int D, dcnt = 0, pt[N];
Dot T[N], p[N];
bool operator<(Dot a, Dot b) { return a[D] < b[D]; }
inline void umax(int& a, int b) {
    if (a < b) a = b;
}
inline void umin(int& a, int b) {
    if (a > b) a = b;
}
inline bool cmp(int x, int y) { return T[x][D] < T[y][D]; }
inline void up(int x) {
    T[x].sz = T[T[x].l].sz + T[T[x].r].sz + 1;
    T[x].sum = T[T[x].l].sum + T[T[x].r].sum + T[x].val;
    T[x].mn[0] = T[x].mx[0] = T[x][0];
    T[x].mn[1] = T[x].mx[1] = T[x][1];
    if (T[x].l) {
        umax(T[x].mx[0], T[T[x].l].mx[0]);
        umin(T[x].mn[0], T[T[x].l].mn[0]);
        umax(T[x].mx[1], T[T[x].l].mx[1]);
        umin(T[x].mn[1], T[T[x].l].mn[1]);
    }
    if (T[x].r) {
        umax(T[x].mx[0], T[T[x].r].mx[0]);
        umin(T[x].mn[0], T[T[x].r].mn[0]);
        umax(T[x].mx[1], T[T[x].r].mx[1]);
        umin(T[x].mn[1], T[T[x].r].mn[1]);
    }
}
inline int NewDot(int x, int y, int c) {
    ++dcnt;
    pt[dcnt] = dcnt;
    T[dcnt][0] = x;
    T[dcnt][1] = y;
    T[dcnt].val = c;
    return up(dcnt), dcnt;
}
inline void AddDot(int x, int y, int c) {
    ++dcnt;
    pt[dcnt] = dcnt;
    p[dcnt][0] = x;
    p[dcnt][1] = y;
    p[dcnt].val = c;
}
int build(int l, int r, int now) {
    int mid = (l + r) >> 1;
    D = now;
    nth_element(p + l, p + mid, p + r + 1);
    T[mid] = p[mid];
    for (int i = 0; i < 2; i++) T[mid].mn[i] = T[mid].mx[i] = T[mid][i];
    if (l < mid) T[mid].l = build(l, mid - 1, now ^ 1);
    if (r > mid) T[mid].r = build(mid + 1, r, now ^ 1);
    return up(mid), mid;
}
long long LA, LB, LC;
bool check(int xl, int xr, int yl, int yr) {
    long long t = -LB * xl + LC;
    if (LA * yl <= t && t <= LA * yr) return 1;
    t = -LB * xr + LC;
    if (LA * yl <= t && t <= LA * yr) return 1;
    t = -LA * yl + LC;
    if (LB * xl <= t && t <= LB * xr) return 1;
    t = -LA * yr + LC;
    if (LB * xl <= t && t <= LB * xr) return 1;
    return 0;
}
void ask(int x) {
    if (!check(T[x].mn[0], T[x].mx[0], T[x].mn[1], T[x].mx[1])) return;
    if (LB * T[x].d[0] + LA * T[x].d[1] == LC) ans++;
    if (T[x].l) ask(T[x].l);
    if (T[x].r) ask(T[x].r);
}
}  // namespace KD_Tree
int T, root, n, m;
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        KD_Tree::dcnt = 0;
        for (int i = 1; i <= n; i++) {
            int x, y;
            scanf("%d%d", &x, &y);
            KD_Tree::AddDot(x, y, 1);
        }
        int root = KD_Tree::build(1, n, 0);
        while (m--) {
            ans = 0;
            scanf("%lld%lld", &KD_Tree::LA, &KD_Tree::LB);
            KD_Tree::LC = KD_Tree::LA * KD_Tree::LB;
            KD_Tree::ask(root);
            printf("%d\n", ans);
        }
    }
    return 0;
}
/*
    Example：
    求K远点对距离
    修改估价函数为欧式上界估价，对每个点进行dfs，
    因为是无向点对，在小根堆中保留前2k个距离，
    不断更新堆顶元素即可。
*/
#include <algorithm>
#include <cstdio>
#include <queue>
using namespace std;
typedef long long LL;
const int N = 200000;
inline int read() {
    int x = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x * f;
}
struct data {
    LL dis;
    data(){};
    data(const LL& x) { dis = x; }
    bool operator<(const data& x) const { return dis > x.dis; }
} tmp;
priority_queue<data> q;  // 小根堆
namespace KD_Tree {
struct Dot {
    int d[2], mn[2], mx[2], l, r;
    Dot() { l = r = 0; }
    Dot(int x, int y) {
        d[0] = x;
        d[1] = y;
        l = r = 0;
    }
    int& operator[](int x) { return d[x]; }
};
int D, dcnt = 0, pt[N];
Dot T[N], p[N];
bool operator<(Dot a, Dot b) { return a[D] < b[D]; }
inline void umax(int& a, int b) {
    if (a < b) a = b;
}
inline void umin(int& a, int b) {
    if (a > b) a = b;
}
inline bool cmp(int x, int y) { return T[x][D] < T[y][D]; }
inline void up(int x) {
    T[x].mn[0] = T[x].mx[0] = T[x][0];
    T[x].mn[1] = T[x].mx[1] = T[x][1];
    if (T[x].l) {
        umax(T[x].mx[0], T[T[x].l].mx[0]);
        umin(T[x].mn[0], T[T[x].l].mn[0]);
        umax(T[x].mx[1], T[T[x].l].mx[1]);
        umin(T[x].mn[1], T[T[x].l].mn[1]);
    }
    if (T[x].r) {
        umax(T[x].mx[0], T[T[x].r].mx[0]);
        umin(T[x].mn[0], T[T[x].r].mn[0]);
        umax(T[x].mx[1], T[T[x].r].mx[1]);
        umin(T[x].mn[1], T[T[x].r].mn[1]);
    }
}
void AddDot(int x, int y) {
    ++dcnt;
    pt[dcnt] = dcnt;
    p[dcnt][0] = x;
    p[dcnt][1] = y;
}
// 建树
int build(int l, int r, int now) {
    if (l > r) return 0;
    int mid = (l + r) >> 1;
    D = now;
    nth_element(p + l, p + mid, p + r + 1);
    T[mid] = p[mid];
    for (int i = 0; i < 2; i++) T[mid].mn[i] = T[mid].mx[i] = T[mid][i];
    if (l < mid) T[mid].l = build(l, mid - 1, now ^ 1);
    if (r > mid) T[mid].r = build(mid + 1, r, now ^ 1);
    return up(mid), mid;
}
LL sqr(LL x) { return x * x; }
//欧几里得上界估价函数
inline LL dist(int x, int px, int py) {
    LL dis = 0;
    if (!x) return 0;
    dis += max(sqr(T[x].mn[0] - px), sqr(T[x].mx[0] - px));
    dis += max(sqr(T[x].mn[1] - py), sqr(T[x].mx[1] - py));
    return dis;
}
}  // namespace KD_Tree
void query(int x, int px, int py) {
    if (!x) return;
    LL dl, dr, d0 = sqr(T[x][0] - px) + sqr(T[x][1] - py);
    dl = T[x].l ? dist(T[x].l, px, py) : 0;
    dr = T[x].r ? dist(T[x].r, px, py) : 0;
    if (d0 > q.top().dis) q.pop(), q.push(data(d0));
    if (dl > dr) {
        if (dl > q.top().dis) query(T[x].l, px, py);
        if (dr > q.top().dis) query(T[x].r, px, py);
    } else {
        if (dr > q.top().dis) query(T[x].r, px, py);
        if (dl > q.top().dis) query(T[x].l, px, py);
    }
}
}
int n, k;
int main() {
    scanf("%d%d", &n, &k);
    for (int i = 1; i <= n; i++) KD_Tree::AddDot(read(), read());
    int root = KD_Tree::build(1, n, 0);
    for (int i = 0; i < k + k; i++) q.push(tmp);
    for (int i = 1; i <= n; i++)
        KD_Tree::query(root, KD_Tree::p[i][0], KD_Tree::p[i][1]);
    printf("%lld\n", q.top().dis);
    return 0;
}
