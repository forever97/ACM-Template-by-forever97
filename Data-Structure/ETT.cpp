/*
    ETT
    欧拉游览树(欧拉序+Splay维护)
    Q 查询子树和
    C 改变x的父节点为y
    F 增加子树的权值
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
typedef long long ll;
template <class T>
void rd(T &a) {
    a = 0;
    char c;
    while (c = getchar(), c < 48)
        ;
    do
        a = a * 10 + (c ^ 48);
    while (c = getchar(), c > 47);
}
inline void rdc(char &c) {
    c = 0;
    char s;
    while (s = getchar(), s < 65)
        ;
    c = s;
}
template <class T>
void nt(T x) {
    if (!x) return;
    nt(x / 10);
    putchar(48 + x % 10);
}
template <class T>
void pt(T x) {
    if (!x)
        putchar('0');
    else
        nt(x);
}
const int M = 2e5 + 10;
const int N = 1e5 + 2;
int f[N], w[N];
struct Edge {
    int to, nxt;
} G[N];
int tot_edge, head[N];
inline void add_edge(int from, int to) {
    G[tot_edge] = (Edge){to, head[from]};
    head[from] = tot_edge++;
}
struct Splay_Tree {
    struct node {
        int tr[2], fa, sz, pos;
        ll sum, add, val;
        inline node() { tr[0] = tr[1] = sum = add = val = fa = sz = pos = 0; }
    } T[M];
    int que[M], allc, rt;
    inline void up(int &x) {
        node &l = T[T[x].tr[0]], &r = T[T[x].tr[1]];
        T[x].sz = l.sz + r.sz + 1;
        T[x].sum = l.sum + r.sum + T[x].val;
        T[x].pos = l.pos + r.pos + (x <= N);
    }
    inline void down(int &x) {
        if (!T[x].add) return;
        ll &ad = T[x].add;
        T[x].val += x >= N ? -ad : ad;
        node &l = T[T[x].tr[0]], &r = T[T[x].tr[1]];
        l.add += T[x].add, r.add += T[x].add;
        int nagl = l.sz - l.pos, nagr = r.sz - r.pos;
        l.sum += 1ll * l.pos * ad - 1ll * nagl * ad;
        r.sum += 1ll * r.pos * ad - 1ll * nagr * ad;
        ad = 0;
    }
    inline void rotate(int x, int &k) {
        int y = T[x].fa, z = T[y].fa, r = T[y].tr[0] == x, l = r ^ 1;
        if (y == k)
            k = x;
        else {
            if (T[z].tr[0] == y)
                T[z].tr[0] = x;
            else
                T[z].tr[1] = x;
        }
        T[x].fa = z, T[y].fa = x, T[T[x].tr[r]].fa = y;
        T[y].tr[l] = T[x].tr[r], T[x].tr[r] = y;
        up(y);
    }
    inline void Splay(int x, int &k) {
        for (int v = x; v != rt; v = T[v].fa) que[++allc] = v;
        que[++allc] = rt;
        for (; allc;) down(que[allc--]);
        for (; x != k;) {
            int y = T[x].fa, z = T[y].fa;
            if (y != k) {
                if (T[z].tr[0] == y ^ T[y].tr[0] == x)
                    rotate(x, k);
                else
                    rotate(y, k);
            }
            rotate(x, k);
        }
        up(x);
    }
    inline void Insert(int x, int w) {
        T[x].val = w;
        if (!rt)
            rt = x;
        else
            T[rt].tr[1] = x, T[x].fa = rt, up(x), Splay(x, rt);
    }
    inline ll query(int &x) {
        Splay(x, rt);
        return T[T[x].tr[0]].sum + T[x].val;
    }
    inline int search(int x, const bool &dir) {
        if (!T[x].tr[dir]) return x;
        return search(T[x].tr[dir], dir);
    }
    inline void Change(int x, int y) {
        Splay(x, rt);
        int k1 = search(T[rt].tr[0], 1);
        Splay(x + N, rt);
        int k2 = search(T[rt].tr[1], 0);
        Splay(k1, rt), Splay(k2, T[rt].tr[1]);
        int w = T[k2].tr[0];
        T[k2].tr[0] = 0, T[w].fa = 0;
        Splay(y, rt);
        int k = search(T[y].tr[1], 0);
        Splay(k, T[y].tr[1]);
        T[w].fa = k, T[k].tr[0] = w;
    }
    inline void Add(int x, int w) {
        Splay(x, rt);
        int k1 = search(T[rt].tr[0], 1);
        Splay(x + N, rt);
        int k2 = search(T[rt].tr[1], 0);
        Splay(k1, rt), Splay(k2, T[rt].tr[1]);
        int s = T[k2].tr[0];
        T[s].add += w,
            T[s].sum += 1ll * w * T[s].pos - 1ll * (T[s].sz - T[s].pos) * w;
    }
} spt;
inline void dfs(int v) {
    spt.Insert(v + 1, w[v]);
    for (int i = head[v]; ~i; i = G[i].nxt) dfs(G[i].to);
    spt.Insert(v + N + 1, -w[v]);
}
int main() {
    int n, q, u, v;
    cin >> n;
    memset(head, -1, sizeof(head));
    tot_edge = 0;
    for (int i = 2; i <= n; ++i) rd(f[i]), add_edge(f[i], i);
    for (int i = 1; i <= n; ++i) rd(w[i]);
    spt.Insert(1, 0);
    dfs(1);
    spt.Insert(N << 1 | 1, 0);
    char c;
    for (cin >> q; q--;) {
        rdc(c);
        switch (c) {
            case ('Q'):  // 查询子树和
                rd(v), ++v;
                pt(spt.query(v)), putchar('\n');
                break;
            case ('C'):  // 把u的父亲改为v
                rd(u), rd(v), ++u, ++v;
                spt.Change(u, v);
                break;
            case ('F'):  // 增加子树权值
                rd(u), rd(v);
                ++u;
                spt.Add(u, v);
                break;
        }
    }
    return 0;
}
