/*
    替罪羊式KD树
    初始化只需要把dcnt置0
    支持在线动态插入查询矩阵中数字和，矩阵中点的个数，最近点距离
    Insert(root[i],p) 向第i棵KD树中插入节点p，支持多棵
    一开始将root[i]置0传入即可
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
int D, dcnt = 0;  // 维度,点数
Dot T[N];
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
    T[dcnt][0] = x;
    T[dcnt][1] = y;
    T[dcnt].val = c;
    T[dcnt].l = T[dcnt].r = T[dcnt].sz = 0;
    return up(dcnt), dcnt;
}
// 查询矩阵内数字和
int Query(int x, int x0, int y0, int x1, int y1) {
    if (!x || T[x].mn[0] > x1 || T[x].mx[0] < x0 || T[x].mn[1] > y1 ||
        T[x].mx[1] < y0)
        return 0;
    if (T[x].mn[0] >= x0 && T[x].mx[0] <= x1 && T[x].mn[1] >= y0 &&
        T[x].mx[1] <= y1)
        return T[x].sum;
    int res = 0;
    if (T[x][0] >= x0 && T[x][0] <= x1 && T[x][1] >= y0 && T[x][1] <= y1)
        res += T[x].val;
    return res + Query(T[x].l, x0, y0, x1, y1) + Query(T[x].r, x0, y0, x1, y1);
}
// 查询矩阵内点的个数
int query(int x, int x0, int y0, int x1, int y1) {
    if (!x || T[x].mn[0] > x1 || T[x].mx[0] < x0 || T[x].mn[1] > y1 ||
        T[x].mx[1] < y0)
        return 0;
    if (T[x].mn[0] >= x0 && T[x].mx[0] <= x1 && T[x].mn[1] >= y0 &&
        T[x].mx[1] <= y1)
        return T[x].sz;
    int res = 0;
    if (T[x][0] >= x0 && T[x][0] <= x1 && T[x][1] >= y0 && T[x][1] <= y1) res++;
    return res + query(T[x].l, x0, y0, x1, y1) + query(T[x].r, x0, y0, x1, y1);
}
// 曼哈顿距离估价函数
inline int dist(int p1, int px, int py) {
    int dis = 0;
    if (px < T[p1].mn[0]) dis += T[p1].mn[0] - px;
    if (px > T[p1].mx[0]) dis += px - T[p1].mx[0];
    if (py < T[p1].mn[1]) dis += T[p1].mn[1] - py;
    if (py > T[p1].mx[1]) dis += py - T[p1].mx[1];
    return dis;
}
//欧几里得距离估价函数
inline LL dist(int p1, int px, int py) {
    LL dis = 0;
    if (px < T[p1].mn[0]) dis += sqr(T[p1].mn[0] - px);
    if (px > T[p1].mx[0]) dis += sqr(px - T[p1].mx[0]);
    if (py < T[p1].mn[1]) dis += sqr(T[p1].mn[1] - py);
    if (py > T[p1].mx[1]) dis += sqr(py - T[p1].mx[1]);
    return dis;
}
// 查询(px,py)最近点距离
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
// 替罪羊式重构
int tot = 0, pt[N];
int gt, gtd, gtf;
const double alp = 0.8;
inline bool isbad(int x) {
    return max(T[T[x].l].sz, T[T[x].r].sz) > T[x].sz * alp + 5;
}
void ins(int& x, int D, const Dot& p) {
    if (!x) {
        x = NewDot(p.d[0], p.d[1], p.val);
        return;
    }
    if (p.d[D] < T[x][D])
        ins(T[x].l, D ^ 1, p);
    else
        ins(T[x].r, D ^ 1, p);
    up(x);
    if (isbad(x))
        gt = x, gtd = D, gtf = 0;
    else if (gt == T[x].l || gt == T[x].r)
        gtf = x;
}
void treavel(int& x) {
    if (!x) return;
    pt[++tot] = x;
    treavel(T[x].l), treavel(T[x].r);
}
int build(int l, int r, int now) {
    if (l > r) return 0;
    int mid = (l + r) >> 1, x;
    D = now;
    nth_element(pt + l, pt + mid, pt + r + 1, cmp);
    x = pt[mid];
    T[x].l = build(l, mid - 1, now ^ 1);
    T[x].r = build(mid + 1, r, now ^ 1);
    return up(x), x;
}
void Insert(int& x, const Dot& p) {
    gt = gtf = 0, ins(x, 0, p);
    if (!gt) return;
    tot = 0, treavel(gt);
    if (!gtf) {
        x = build(1, tot, gtd);
        return;
    }
    if (gt == T[gtf].l)
        T[gtf].l = build(1, tot, gtd);
    else
        T[gtf].r = build(1, tot, gtd);
}
}  // namespace KD_Tree
