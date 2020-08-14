/*
    动态凸包
    insert_line(m,b): 插入y=mx+b
    query(x): 查询max(mx+b)
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const LL is_query = -(1LL << 62);
struct Line {
    LL m, b;
    mutable function<const Line *()> succ;
    bool operator<(const Line &rhs) const {
        if (rhs.b != is_query) return m < rhs.m;
        const Line *s = succ();
        if (!s) return 0;
        LL x = rhs.m;
        return b - s->b < (s->m - m) * x;
    }
};
struct HullDynamic : public multiset<Line> {
    bool check(iterator y) {
        auto z = next(y);
        if (y == begin()) {
            if (z == end()) return 0;
            return y->m == z->m && y->b <= z->b;
        }
        auto x = prev(y);
        if (z == end()) return y->m == x->m && y->b <= x->b;
        return (x->b - y->b) * (long double)(z->m - y->m) >=
               (y->b - z->b) * (long double)(y->m - x->m);
    }
    void insert_line(LL m, LL b) {
        auto y = insert({m, b});
        y->succ = [=] { return next(y) == end() ? 0 : &*next(y); };
        if (check(y)) {
            erase(y);
            return;
        }
        while (next(y) != end() && check(next(y))) erase(next(y));
        while (y != begin() && check(prev(y))) erase(prev(y));
    }
    LL query(LL x) {
        auto l = *lower_bound((Line){x, is_query});
        return l.m * x + l.b;
    }
};
/*
    题目大意：给出一棵树，树的根为1，树上每个节点有两个值a和b，
    从一个节点i可以跳到子树上任意一个节点j，代价是a[i]*b[j]，
    从一个点到达另一个节点的代价是图中所有代价之和，
    问每个节点跳到任意一个叶节点的代价最小为多少。
    题解：我们递归计算答案，对于每个节点来说，他的答案为min(a[i]*b[j]+ans[j])，
    那么我们对于每个节点维护一个子树的动态凸包，求最大mx+b即可
    为避免过多的插入和比较，我们把信息都保存在节点最大子树的代表节点u[x]中，
    通过只改变标记u[x]以替代信息的大量转移。
*/
const int N = 100010;
int n, a[N], b[N], size[N], u[N], res[N], st[N], en[N];
vector<int> g[N];
LL ans[N];
HullDynamic T[N];
int dfn = 0;
void dfs(int x, int fx) {
    st[x] = ++dfn;
    res[dfn] = x;
    int t = -1;
    size[x] = 1;
    for (int y : g[x]) {
        if (y == fx) continue;
        dfs(y, x);
        size[x] += size[y];
        if (t == -1 || size[y] > size[t]) t = y;
    }
    if (t == -1) {
        T[x].insert_line(-b[x], 0);
        ans[x] = 0;
        u[x] = x;
    } else {
        u[x] = u[t];
        for (int y : g[x])
            if (y != t) {
                for (int i = st[y]; i <= en[y]; i++)
                    T[u[x]].insert_line(-b[res[i]], -ans[res[i]]);
            }
        ans[x] = -T[u[x]].query(a[x]);
        T[u[x]].insert_line(-b[x], -ans[x]);
    }
    en[x] = dfn;
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", a + i);
    for (int i = 1; i <= n; i++) scanf("%d", b + i);
    for (int i = 1; i < n; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        g[x].push_back(y);
        g[y].push_back(x);
    }
    dfs(1, 0);
    for (int i = 1; i <= n; i++) printf("%lld ", ans[i]);
    return 0;
}