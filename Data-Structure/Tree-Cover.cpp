/*
    线段树套Treap
    离散动态区间第k小
*/
const int N = 200010, M = 524289;
int n, m, cnt, i, a[N], op[N][4], b[N];
inline void read(int& a) {
    char c;
    while (!(((c = getchar()) >= '0') && (c <= '9')))
        ;
    a = c - '0';
    while (((c = getchar()) >= '0') && (c <= '9')) (a *= 10) += c - '0';
}
inline int lower(int x) {
    int l = 1, r = cnt, t, mid;
    while (l <= r)
        if (b[mid = (l + r) >> 1] <= x)
            l = (t = mid) + 1;
        else
            r = mid - 1;
    return t;
}
struct node {
    int val, cnt, sum, p;
    node *l, *r;
    node() {
        val = sum = cnt = p = 0;
        l = r = NULL;
    }
    inline void up() { sum = l->sum + r->sum + cnt; }
}* blank = new (node), *T[M], pool[2000000], *cur;
inline void Rotatel(node*& x) {
    node* y = x->r;
    x->r = y->l;
    x->up();
    y->l = x;
    y->up();
    x = y;
}
inline void Rotater(node*& x) {
    node* y = x->l;
    x->l = y->r;
    x->up();
    y->r = x;
    y->up();
    x = y;
}
void Ins(node*& x, int y, int p) {
    if (x == blank) {
        x = cur++;
        x->val = y;
        x->l = x->r = blank;
        x->sum = x->cnt = 1;
        x->p = std::rand();
        return;
    }
    x->sum += p;
    if (y == x->val) {
        x->cnt += p;
        return;
    }
    if (y < x->val) {
        Ins(x->l, y, p);
        if (x->l->p > x->p) Rotater(x);
    } else {
        Ins(x->r, y, p);
        if (x->r->p > x->p) Rotatel(x);
    }
}
inline int Ask(node* x, int y) {  // ask how many <= y
    int t = 0;
    while (x != blank)
        if (y < x->val)
            x = x->l;
        else
            t += x->l->sum + x->cnt, x = x->r;
    return t;
}
inline void add(int v, int i, int p) {
    int a = 1, b = cnt, mid, f = 1, x = 1;
    while (a < b) {
        if (f) Ins(T[x], i, p);
        mid = (a + b) >> 1;
        x <<= 1;
        if (f = v <= mid)
            b = mid;
        else
            a = mid + 1, x |= 1;
    }
    Ins(T[x], i, p);
}
inline int kth(int l, int r, int k) {
    int x = 1, a = 1, b = cnt, mid;
    while (a < b) {
        mid = (a + b) >> 1;
        x <<= 1;
        int t = Ask(T[x], r) - Ask(T[x], l - 1);
        if (k <= t)
            b = mid;
        else
            k -= t, a = mid + 1, x |= 1;
    }
    return a;
}
void build(int x, int a, int b) {
    T[x] = blank;
    if (a == b) return;
    int mid = (a + b) >> 1;
    build(x << 1, a, mid), build(x << 1 | 1, mid + 1, b);
}
int main() {
    blank->l = blank->r = blank;
    while (~scanf("%d", &n)) {
        cur = pool;
        for (i = 1; i <= n; i++) read(a[i]), b[i] = a[i];
        cnt = n;
        read(m);
        for (i = 1; i <= m; i++) {
            read(op[i][0]), read(op[i][1]), read(op[i][2]);
            if (op[i][0] == 1)
                b[++cnt] = op[i][2];
            else
                read(op[i][3]);
        }
        sort(b + 1, b + cnt + 1);
        for (i = 1; i <= n; i++) a[i] = lower(a[i]);
        for (i = 1; i <= m; i++)
            if (op[i][0] == 1) op[i][2] = lower(op[i][2]);
        build(1, 1, cnt);
        for (i = 1; i <= n; i++) add(a[i], i, 1);
        for (i = 1; i <= m; i++) {
            if (op[i][0] == 1)
                add(a[op[i][1]], op[i][1], -1),
                    add(a[op[i][1]] = op[i][2], op[i][1], 1);
            else
                printf("%d\n", b[kth(op[i][1], op[i][2], op[i][3])]);
        }
    }
    return 0;
}
/*
    权值线段树套替罪羊式KD树
    操作 1 x y k表示在点(x,y)上放置k个物品，
    操作 2 x0 y0 x1 y1 k表示查询矩形内放置物品第k多的格子有几个物品
    同一个格子不会被同时放置物品一次以上
*/
const int N = 3000000, INF = 1e9;
namespace KD_Tree {
struct Dot {
    int d[2], mn[2], mx[2], l, r, sz;
    Dot() { l = r = 0; }
    Dot(int x, int y) {
        d[0] = x;
        d[1] = y;
        l = r = sz = 0;
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
inline int NewDot(int x, int y) {
    ++dcnt;
    T[dcnt][0] = x;
    T[dcnt][1] = y;
    return up(dcnt), dcnt;
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
// 替罪羊式重构
int tot = 0, pt[N];
int gt, gtd, gtf;
const double alp = 0.8;
inline bool isbad(int x) {
    return max(T[T[x].l].sz, T[T[x].r].sz) > T[x].sz * alp + 5;
}
void ins(int& x, int D, const Dot& p) {
    if (!x) {
        x = NewDot(p.d[0], p.d[1]);
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
int tot = 0;
struct data {
    int rt, l, r;
} T[N];
void Insert(int& x, const KD_Tree::Dot& p, int val, int l = 1, int r = INF) {
    if (!x) x = ++tot;
    KD_Tree::Insert(T[x].rt, p);
    if (l == r) return;
    int mid = (l + r) >> 1;
    if (val <= mid)
        Insert(T[x].l, p, val, l, mid);
    else
        Insert(T[x].r, p, val, mid + 1, r);
}
int query(int x, int x0, int y0, int x1, int y1, int k, int l = 1,
          int r = INF) {
    if (l == r) return l;
    int rcnt = KD_Tree::query(T[T[x].r].rt, x0, y0, x1, y1);
    int mid = (l + r) >> 1;
    if (k <= rcnt) return query(T[x].r, x0, y0, x1, y1, k, mid + 1, r);
    return query(T[x].l, x0, y0, x1, y1, k - rcnt, l, mid);
}
int n, q, op, x0, y0, x1, y1, k, root = 0, ans = 0;
int main() {
    scanf("%d%d", &n, &q);
    while (q--) {
        scanf("%d", &op);
        if (op == 1) {
            scanf("%d%d%d", &x0, &y0, &k);
            x0 ^= ans, y0 ^= ans, k ^= ans;
            KD_Tree::Dot p = KD_Tree::Dot(x0, y0);
            Insert(root, p, k);
        } else {
            scanf("%d%d%d%d%d", &x0, &y0, &x1, &y1, &k);
            x0 ^= ans, y0 ^= ans, x1 ^= ans, y1 ^= ans, k ^= ans;
            int res = KD_Tree::query(T[root].rt, x0, y0, x1, y1);
            if (res < k)
                puts("NAIVE!ORZzyz."), ans = 0;
            else
                printf("%d\n", ans = query(root, x0, y0, x1, y1, k));
        }
    }
    return 0;
}
