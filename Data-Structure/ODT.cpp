/*
    ODT
    擅长区间推平操作
    区间推平：
        在set里找到对应区间两端点，删除中间所有元素，插入一个新线段
        当出现端点在线段中时对线段进行拆分
    其余操作暴力
*/
#define IT set<node>::iterator
struct node {
    int ll, rr;
    mutable int val;
    node(int L, int R = -1, int V = 0) : ll(L), rr(R), val(V) {}
    bool operator<(const node& tt) const { return ll < tt.ll; }
};
set<node> st;
IT split(int pos) {
    IT it = st.lower_bound(node(pos));
    if (it != st.end() && it->ll == pos) return it;
    --it;
    int ll = it->ll, rr = it->rr, val = it->val;
    st.erase(it);
    st.insert(node(ll, pos - 1, val));
    return st.insert(node(pos, rr, val)).first;
}
void assign(int ll, int rr, int val) {
    // attention: first rr+1, then ll
    IT itr = split(rr + 1), itl = split(ll);
    st.erase(itl, itr);
    st.insert(node(ll, rr, val));
}
void other(int ll, int rr, int val) {
    // attention: first rr+1, then ll
    IT itr = split(rr + 1), itl = split(ll);
    for (; itl != itr; ++itl) {
        // doit
    }
}
/*
    Example：
        给定一棵树，每条边都有颜色，每次操作u,c,m
        要求将从u到根的所有边变成颜色c，然后统计对于这棵树，
        有多少种颜色恰好染了m条路
*/
#include <bits/stdc++.h>
using namespace std;
#define IT set<node>::iterator
const int N = 200010;
int c[N], cnt[N];
struct node {
    int l, r;
    mutable int val;
    node(int L, int R = -1, int V = 0) : l(L), r(R), val(V) {}
    bool operator<(const node& tt) const { return l < tt.l; }
};
set<node> st;
IT split(int pos) {
    IT it = st.lower_bound(node(pos));
    if (it != st.end() && it->l == pos) return it;
    --it;
    int l = it->l, r = it->r, val = it->val;
    st.erase(it);
    st.insert(node(l, pos - 1, val));
    return st.insert(node(pos, r, val)).first;
}
void assign(int l, int r, int val) {
    IT itr = split(r + 1), itl = split(l);
    st.erase(itl, itr);
    st.insert(node(l, r, val));
}
void update(int l, int r, int val) {
    // attention: first rr+1, then ll
    IT itr = split(r + 1), itl = split(l);
    for (; itl != itr; ++itl) {
        if (itl->val) {
            --cnt[c[itl->val]];
            c[itl->val] -= itl->r - itl->l + 1;
            ++cnt[c[itl->val]];
        }
    }
    --cnt[c[val]];
    c[val] += r - l + 1;
    ++cnt[c[val]];
    assign(l, r, val);
}
vector<int> v[N];
int id[N], dfn, top[N], fa[N], sz[N], son[N];
void dfs1(int x, int fx) {
    sz[x] = 1;
    fa[x] = fx;
    for (int y : v[x])
        if (y != fa[x]) {
            dfs1(y, x);
            sz[x] += sz[y];
            if (sz[son[x]] < sz[y]) son[x] = y;
        }
}
void dfs2(int x, int fx) {
    top[x] = fx;
    id[x] = ++dfn;
    if (son[x]) dfs2(son[x], fx);
    for (int y : v[x])
        if (y != fa[x] && y != son[x]) dfs2(y, y);
}
void path(int x, int val) {
	while (x > 1) {
		update(max(id[top[x]], 2), id[x], val);
		x = fa[top[x]];
	}
}
int n, m, q;
int main() {
    scanf("%d%d%d", &n, &m, &q);
    for(int i = 1; i < n; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        v[x].push_back(y);
        v[y].push_back(x);
    }
    cnt[0] = m;
    dfs1(1, 1);
    dfs2(1, 1);
    st.insert(node(2, n));
    while (q--) {
        int u, c, m;
        scanf("%d%d%d", &u, &c, &m); 
        path(u, c);
        printf("%d\n", cnt[m]);
    }
    return 0;
}
