/*
    TopTree
    c[0],c[1]: LCT的左右儿子(维护实边)
    C[2],c[3]: Splay的左右儿子(维护虚边)
    链上信息：LCT维护
    子树信息：以点x为根的子树的信息
    =x在链上的所有后代的信息+x在链上的所有后代连出去的虚边的信息+它本身的信息
    (在实际操作中其实只有后两个的信息)
    cha：在Splay维护的实边形成的链中x的所有子节点(含本身)信息和
    sub：x的所有子节点(含本身)连出去的虚边的信息和(不包括链的信息)
    all：x的所有信息和,即cha+sub
    All：子树修改标记(不包括链)
    Cha：传统的链标记
    linkcut：
    cut(u)：把u的父亲access,然后del(u)
    link(u,v)：把v的父亲access,然后add(u,v->f)
    makert: 同LCT
*/
#include <bits/stdc++.h>
#define rep(i, a, n) for (int i = a; i < n; i++)
using namespace std;
const int maxn = 1e5 + 1000;
int getint() {
    int res = 0, f = 1;
    char c = getchar();
    while (!isdigit(c)) f = f == -1 || c == '-' ? -1 : 1, c = getchar();
    while (isdigit(c)) res = res * 10 + c - '0', c = getchar();
    return res * f;
}
int n, m;
struct info {
    int mx, mn, sum, sz;
    info() {}
    info(int mx, int mn, int sum, int sz) : mx(mx), mn(mn), sum(sum), sz(sz) {}
    void deb() { printf("sum:%d size:%d", (int)sum, sz); }
};
struct flag {
    int mul, add;
    flag() { mul = 1; }
    flag(int mul, int add) : mul(mul), add(add) {}
    bool empty() { return mul == 1 && add == 0; }
};
info operator+(const info &a, const flag &b) {
    return a.sz ? info(a.mx * b.mul + b.add, a.mn * b.mul + b.add,
                       a.sum * b.mul + b.add * a.sz, a.sz)
                : a;
}
info operator+(const info &a, const info &b) {
    return info(max(a.mx, b.mx), min(a.mn, b.mn), a.sum + b.sum, a.sz + b.sz);
}
flag operator+(const flag &a, const flag &b) {
    return flag(a.mul * b.mul, a.add * b.mul + b.add);
}
struct node {
    node *c[4], *f;
    flag Cha, All;
    info cha, sub, all;
    bool rev, inr;
    int val;
    void makerev() {
        rev ^= 1;
        swap(c[0], c[1]);
    }
    void makec(const flag &a) {
        Cha = Cha + a;
        cha = cha + a;
        val = val * a.mul + a.add;
        all = cha + sub;
    }
    void makes(const flag &a, bool _ = 1) {
        All = All + a;
        all = all + a;
        sub = sub + a;
        if (_) makec(a);
    }
    void rz() {
        cha = all = sub = info(-(1 << 30), 1 << 30, 0, 0);
        if (!inr) all = cha = info(val, val, val, 1);
        rep(i, 0, 2) if (c[i]) cha = cha + c[i]->cha, sub = sub + c[i]->sub;
        rep(i, 0, 4) if (c[i]) all = all + c[i]->all;
        rep(i, 2, 4) if (c[i]) sub = sub + c[i]->all;
    }
    void init(int _val) {
        rep(i, 0, 4) c[i] = 0;
        f = 0;
        All = Cha = flag(1, 0);
        sub = info(-(1 << 30), 1 << 30, 0, 0);
        inr = rev = 0;
        val = _val;
        all = cha = info(val, val, val, 1);
    }
    void pd() {
        if (rev) {
            if (c[0]) c[0]->makerev();
            if (c[1]) c[1]->makerev();
            rev = 0;
        }
        if (!All.empty()) {
            rep(i, 0, 4) if (c[i]) c[i]->makes(All, i >= 2);
            All = flag(1, 0);
        }
        if (!Cha.empty()) {
            rep(i, 0, 2) if (c[i]) c[i]->makec(Cha);
            Cha = flag(1, 0);
        }
    }
    node *C(int i) {
        if (c[i]) c[i]->pd();
        return c[i];
    }
    bool d(int ty) { return f->c[ty + 1] == this; }
    int D() { rep(i, 0, 4) if (f->c[i] == this) return i; }
    void sets(node *x, int d) {
        if (x) x->f = this;
        c[d] = x;
    }
    bool rt(int ty) {
        if (ty == 0)
            return !f || (f->c[0] != this && f->c[1] != this);
        else
            return !f || !f->inr || !inr;
    }
} nd[maxn * 2], *cur = nd + maxn, *pool[maxn], **Cur = pool;
int _cnt;
node *newnode() {
    _cnt++;
    node *x = (Cur == pool) ? cur++ : *(--Cur);
    rep(i, 0, 4) x->c[i] = 0;
    x->f = 0;
    x->All = x->Cha = flag(1, 0);
    x->all = x->cha = info(-(1 << 30), (1 << 30), 0, 0);
    x->inr = 1;
    x->rev = 0;
    x->val = 0;
    return x;
}
void dele(node *x) { *(Cur++) = x; }
void rot(node *x, int ty) {
    node *p = x->f;
    int d = x->d(ty);
    if (!p->f)
        x->f = 0;
    else
        p->f->sets(x, p->D());
    p->sets(x->c[!d + ty], d + ty);
    x->sets(p, !d + ty);
    p->rz();
}
node *splay(node *x, int ty = 0) {
    while (!x->rt(ty)) {
        if (x->f->rt(ty))
            rot(x, ty);
        else if (x->d(ty) == x->f->d(ty))
            rot(x->f, ty), rot(x, ty);
        else
            rot(x, ty), rot(x, ty);
    }
    x->rz();
    return x;
}
void add(node *u, node *w) {
    w->pd();
    rep(i, 2, 4) if (!w->c[i]) {
        w->sets(u, i);
        return;
    }
    node *x = newnode(), *v;
    for (v = w; v->c[2]->inr; v = v->C(2))
        ;
    x->sets(v->c[2], 2);
    x->sets(u, 3);
    v->sets(x, 2);
    splay(x, 2);
}
void del(node *w) {
    if (w->f->inr) {
        w->f->f->sets(w->f->c[5 - w->D()], w->f->D());
        dele(w->f);
        splay(w->f->f, 2);
    } else
        w->f->sets(0, w->D());
    w->f = 0;
}
void access(node *w) {
    static node *sta[maxn];
    static int top = 0;
    node *v = w, *u;
    for (u = w; u; u = u->f) sta[top++] = u;
    while (top) sta[--top]->pd();
    splay(w);
    if (w->c[1]) u = w->c[1], w->c[1] = 0, add(u, w), w->rz();
    while (w->f) {
        for (u = w->f; u->inr; u = u->f)
            ;
        splay(u);
        if (u->c[1])
            w->f->sets(u->c[1], w->D()), splay(w->f, 2);
        else
            del(w);
        u->sets(w, 1);
        (w = u)->rz();
    }
    splay(v);
}
void makert(node *x) {
    access(x);
    x->makerev();
}
node *findp(node *u) {  // 找到原树上的父亲
    access(u);
    u = u->C(0);
    while (u && u->c[1]) u = u->C(1);
    return u;
}
node *findr(node *u) {
    for (; u->f; u = u->f)
        ;
    return u;
}
node *cut(node *u) {
    node *v = findp(u);
    if (v) access(v), del(u), v->rz();
    return v;
}
void link(node *u, node *v) {
    node *p = cut(u);
    if (findr(u) != findr(v)) p = v;
    if (p) access(p), add(u, p), p->rz();
}
int main() {
    n = getint();
    m = getint();
    static int _u[maxn], _v[maxn];
    rep(i, 1, n) _u[i] = getint(), _v[i] = getint();
    rep(i, 1, n + 1) nd[i].init(getint());
    rep(i, 1, n) makert(nd + _u[i]), link(nd + _u[i], nd + _v[i]);
    int root = getint();
    makert(nd + root);
    int x, y;
    node *u;
    while (m--) {
        int k = getint();
        x = getint();
        u = nd + x;
        if (k == 0 || k == 3 || k == 4 || k == 5 || k == 11) {
            access(u);
            if (k == 3 || k == 4 || k == 11) {  // 子树查询
                int ans = u->val;
                rep(i, 2, 4) if (u->c[i]) {
                    info res = u->c[i]->all;
                    if (k == 3)
                        ans = min(ans, res.mn);
                    else if (k == 4)
                        ans = max(ans, res.mx);
                    else if (k == 11)
                        ans += res.sum;
                }
                printf("%d\n", ans);
            } else {
                y = getint();
                flag fg(k == 5, y);  // 子树加法/子树赋值
                u->val = u->val * fg.mul + fg.add;
                rep(i, 2, 4) if (u->c[i]) u->c[i]->makes(fg);
                u->rz();
            }
        } else if (k == 2 || k == 6 || k == 7 || k == 8 || k == 10) {
            y = getint();
            makert(u), access(nd + y), splay(u);
            if (k == 7 || k == 8 || k == 10) {  // 链查询
                info ans = u->cha;
                if (k == 7)
                    printf("%d\n", ans.mn);
                else if (k == 8)
                    printf("%d\n", ans.mx);
                else
                    printf("%d\n", ans.sum);
            } else
                u->makec(flag(k == 6, getint()));  // 链加法/链查询
            makert(nd + root);
        } else if (k == 9)
            link(u, nd + getint());  // x的父节点变成y
        else if (k == 1)
            makert(u), root = x;  // 换根
    }
    return 0;
}
