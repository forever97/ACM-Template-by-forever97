/*
    Problem:
        有1e9只小怪兽，第i只编号为i
        有一张n个点，m条边的无向图n,m<=1e5
        每条边只允许编号在[l,r]之间的小怪兽通过
        问有多少小怪兽可以从1点抵达n点
    Solution:
        我们对边标号区间[l,r]进行离散，最多产生2*m-1个区间，
        对区间进行时间分治，在线段树节点上保存覆盖子树所有区间的边id，
        对线段树DFS遍历，当1和n连通时将节点包含的编号统计入答案即可
        连通性判断用可回溯并查集，复杂度O(mlogm)
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 200000 + 10;
// Union Find Set
int st[N << 1], top, f[N], d[N];
void init(int n) {
    for (int i = 1; i <= n; i++) f[i] = i, d[i] = 1;
    top = 0;
}
int sf(int x) { return f[x] == x ? x : sf(f[x]); }
void back(int tag) {
    for (; top != tag; top--) {
        if (st[top] < 0)
            d[-st[top]]--;
        else
            f[st[top]] = st[top];
    }
}
void Union(int x, int y) {
    if (d[x] > d[y]) swap(x, y);
    if (d[x] == d[y]) d[y]++, st[++top] = -y;
    f[x] = y;
    st[++top] = x;
}
// Edge
struct Data {
    int x, y, l, r;
} E[N];
// Segment Tree
vector<int> v[N << 2];
void upd(int x, int l, int r, int y) {
    int mid = (l + r) >> 1;
    if (E[y].l <= l && r <= E[y].r) {
        v[x].push_back(y);
        return;
    }
    if (E[y].l < mid) upd(x << 1, l, mid, y);
    if (E[y].r > mid) upd(x << 1 | 1, mid, r, y);
}
int n, m, ans = 0;
int b[N << 2], x, y, l, r;
void dfs(int x, int l, int r) {
    int mid = (l + r) >> 1, t = top;
    for (auto y : v[x]) {
        int fx = sf(E[y].x), fy = sf(E[y].y);
        if (fx != fy) Union(fx, fy);
    }
    if (sf(1) == sf(n)) {
        ans += b[r] - b[l];
        back(t);
        return;
    }
    if (l + 1 == r) {
        back(t);
        return;
    }
    dfs(x << 1, l, mid);
    dfs(x << 1 | 1, mid, r);
    back(t);
}
int main() {
    scanf("%d%d", &n, &m);
    init(n);
    int cnt = 0;
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d%d", &x, &y, &l, &r);
        E[i] = {x, y, l, r + 1};
        b[++cnt] = l;
        b[++cnt] = r + 1;
    }
    sort(b + 1, b + cnt + 1);
    int siz = unique(b + 1, b + cnt + 1) - (b + 1);
    for (int i = 1; i <= m; i++) {
        E[i].l = lower_bound(b + 1, b + siz + 1, E[i].l) - b;
        E[i].r = lower_bound(b + 1, b + siz + 1, E[i].r) - b;
        upd(1, 1, siz, i);
    }
    dfs(1, 1, siz);
    printf("%d\n", ans);
    return 0;
}

/*
    Problem:
        给定一张无向边权图，要求维护三个操作
        OP1.[x,y,z]:在点x和点y之间加一条边权为z的边，保证之前没有边
        OP2.[x,y]:将点x和y之间的边删除，保证之前有边
        OP3.[x,y]:查询x到y的路径的异或最小值，可以是非简单路
    Solution：
        图上两点异或路径的最小值为生成树上异或距离和树上环的组合
        我们以OP3为时间线建线段树，将覆盖操作时间点的边保存在线段树节点
        对OP3的线段树进行DFS遍历，用并查集维护两点间的xor距离
        当成环时将环加入xor线性基，在叶节点查询xor线性基和xor距离组合的最小值即可
        线性基空间O(30)可以选择直接传参，并查集空间O(n)需回溯
        时间复杂度O(mlogt)，t为OP3的数量，m为总边数
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 400000 + 10;
using PII = pair<int, int>;
// Xor Base
struct Base {
    int p[31];
    void ins(int x) {
        for (int j = 30; ~j; --j)
            if ((x >> j) & 1) {
                if (!p[j]) {
                    p[j] = x;
                    return;
                }
                x ^= p[j];
            }
    }
    int ask(int x) {
        for (int j = 30; ~j; --j) x = min(x, x ^ p[j]);
        return x;
    }
} S;
// Union Find Set
int st[N << 1], top, f[N], val[N], d[N];
void init(int n) {
    for (int i = 1; i <= n; i++) f[i] = i, val[i] = 0, d[i] = 1;
    top = 0;
}
int sf(int x) { return f[x] == x ? x : sf(f[x]); }
int ask(int x) {
    int res = 0;
    for (; x != f[x]; x = f[x]) res ^= val[x];
    return res;
}
void back(int tag) {
    for (; top != tag; top--) {
        if (st[top] < 0)
            d[-st[top]]--;
        else {
            f[st[top]] = st[top];
            val[st[top]] = 0;
        }
    }
}
void Union(int x, int y, int _val) {
    if (d[x] > d[y]) swap(x, y);
    if (d[x] == d[y]) d[y]++, st[++top] = -y;
    f[x] = y;
    val[x] = _val;
    st[++top] = x;
}
// Edge
struct data1 {
    int x, y, w, l, r;
} E[N];
// Query
struct data2 {
    int x, y;
} Q[N];
// Segment Tree
vector<int> v[N << 1];
int idx(int l, int r) { return l + r | l != r; }
void upd(int l, int r, int y) {
    int x = idx(l, r), mid = (l + r) >> 1;
    if (E[y].l <= l && r <= E[y].r) {
        v[x].push_back(y);
        return;
    }
    if (E[y].l <= mid) upd(l, mid, y);
    if (E[y].r > mid) upd(mid + 1, r, y);
}
void dfs(int l, int r, Base S) {
    int x = idx(l, r), t = top, mid = (l + r) >> 1;
    for (auto y : v[x]) {
        int fx = sf(E[y].x), fy = sf(E[y].y);
        int dx = ask(E[y].x), dy = ask(E[y].y);
        if (fx == fy) {
            S.ins(dx ^ dy ^ E[y].w);
            continue;
        }
        Union(fx, fy, dx ^ dy ^ E[y].w);
    }
    if (l == r) {
        int dx = ask(Q[l].x), dy = ask(Q[l].y);
        printf("%d\n", S.ask(dx ^ dy));
        back(t);
        return;
    }
    dfs(l, mid, S);
    dfs(mid + 1, r, S);
    back(t);
}
map<PII, int> id;
int n, m, x, y, z, q, op, tot;
int main() {
    scanf("%d%d", &n, &m);
    init(n);
    tot = 0;
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d", &x, &y, &z);
        if (x > y) swap(x, y);
        E[i] = {x, y, z, tot + 1, -1};
        id[{x, y}] = i;
    }
    scanf("%d", &q);
    while (q--) {
        scanf("%d", &op);
        if (op == 1) {
            scanf("%d%d%d", &x, &y, &z);
            if (x > y) swap(x, y);
            E[++m] = {x, y, z, tot + 1, -1};
            id[{x, y}] = m;
        } else if (op == 2) {
            scanf("%d%d", &x, &y);
            if (x > y) swap(x, y);
            E[id[{x, y}]].r = tot;
        } else {
            scanf("%d%d", &x, &y);
            Q[++tot] = {x, y};
        }
    }
    for (int i = 1; i <= m; i++) {
        if (E[i].r == -1) E[i].r = tot;
        if (E[i].l <= E[i].r) upd(1, tot, i);
    }
    dfs(1, tot, S);
    return 0;
}
/*
    Problem：
        给出一张图，有些边只存在一段时间，问在一个每个时间段，
        这张图是否是二分图
    Solution：
        判断是否是二分图只要判断是否存在奇环即可，
        我们对时间进行分治，在操作树上加删边，
        保留涵盖时间区间的有效操作，将剩余操作按时间划分到两端的子树，
        退出子树的时候撤销加边操作。
        对于判断奇环，我们用并查集维护每个点与标兵的相对距离的奇偶性即可，
        由于需要撤销操作，我们放弃对并查集的压缩操作，
        采用按秩合并，保证查询的logn复杂度，同时保存每次合并过程即可。
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int N = 300010;
namespace Union_Find_Set {
int st[N], top, f[N], val[N], d[N];
void Initialize(int n) {
    for (int i = 1; i <= n; i++) f[i] = i, val[i] = 0, d[i] = 1;
    top = 0;
}
int sf(int x) { return f[x] == x ? x : sf(f[x]); }
int ask(int x) {
    int res = 0;
    for (; x != f[x]; x = f[x]) res ^= val[x];
    return res;
}
void back(int tag) {
    for (; top != tag; top--) {
        if (st[top] < 0)
            d[-st[top]]--;
        else {
            f[st[top]] = st[top];
            val[st[top]] = 0;
        }
    }
}
void Union(int x, int y, int _val) {
    if (d[x] > d[y]) swap(x, y);
    if (d[x] == d[y]) d[y]++, st[++top] = -y;
    f[x] = y;
    val[x] = _val;
    st[++top] = x;
}
}  // namespace Union_Find_Set
using namespace Union_Find_Set;
struct data {
    int x, y, l, r;
} E[N];
void dfs(int l, int r, int pos) {
    int t = top;
    for (int i = 1; i <= pos; i++) {
        int x = E[i].x, y = E[i].y;
        if (E[i].l <= l && E[i].r >= r) {
            int fx = sf(x), fy = sf(y);
            int val = ask(x) ^ ask(y) ^ 1;
            if (fx == fy) {
                if (val) {
                    for (int j = l; j <= r; j++) puts("No");
                    back(t);
                    return;
                }
            }
            Union(fx, fy, val);
            swap(E[i--], E[pos--]);
        }
    }
    if (l == r) {
        puts("Yes");
        back(t);
        return;
    }
    int mid = (l + r) >> 1, ppos = pos;
    for (int i = 1; i <= ppos; i++) {
        if (E[i].l > mid) swap(E[i--], E[ppos--]);
    }
    dfs(l, mid, ppos);
    ppos = pos;
    for (int i = 1; i <= ppos; i++) {
        if (E[i].r <= mid) swap(E[i--], E[ppos--]);
    }
    dfs(mid + 1, r, ppos);
    back(t);
}
int n, m, op, x, y, l, r, T;
int main() {
    scanf("%d%d%d", &n, &m, &T);
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d%d", &x, &y, &l, &r);
        E[i] = (data){x, y, ++l, r};
    }
    Initialize(n);
    for (int i = 1; i <= m; i++)
        if (E[i].l > E[i].r) swap(E[i], E[m--]);
    dfs(1, T, m);
    return 0;
}
