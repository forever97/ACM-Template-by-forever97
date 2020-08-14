/*
    Endpos集维护
    Problem:
        我们定义一个字符串A比一个字符串B酷当B在A中作为子串出现至少两次
        现在给定一个字符串S，要求找到最长的子串序列S1,S2,S3……
        要求对于任意i，S[i+1]比S[i]酷
    Solution:
        我们发现，如果我们要找一个字符串b，使得字符串a作为子串在其中出现至少两次
        即a的endpos集合在[pos[b]-len[b]+len[a],pos[b]]中出现至少两次
        最短的b串一定是以a为前缀且以a为后缀的，可以得出结论a为b的border
        那么只要a的endpos集合在[pos[b]-len[b]+len[a],pos[b]-1]中出现即可
        用线段树维护endpos，将endpos按fail链线段树合并得到每个节点的endpos集合
        查询区间[pos[b]-len[b]+len[a],pos[b]-1]是否有值
        在fail链节点上保存最长子序列的最后一位，每次扩展时在这一位的线段树上查询
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 400000 + 10;
int Tr(char c) { return c - 'a'; }
int p, q, np, nq, cnt, lst, a[N][26], l[N], f[N], pos[N];
void extend(int c, int ps) {
    p = lst;
    np = lst = ++cnt;
    l[np] = l[p] + 1;
    pos[np] = ps;
    while (!a[p][c] && p) a[p][c] = np, p = f[p];
    if (!p)
        f[np] = 1;
    else {
        q = a[p][c];
        if (l[p] + 1 == l[q])
            f[np] = q;
        else {
            nq = ++cnt;
            l[nq] = l[p] + 1;
            pos[nq] = pos[q]; 
            memcpy(a[nq], a[q], sizeof(a[q]));
            f[nq] = f[q];
            f[np] = f[q] = nq;
            while (a[p][c] == q) a[p][c] = nq, p = f[p];
        }
    }
}
int ls[N * 30], rs[N * 30], rt[N], tot;
void upd(int &x, int l, int r, int p) {
    x = ++tot;
    if (l == r) return;
    int mid = l + r >> 1;
    p <= mid ? upd(ls[x], l, mid, p) : upd(rs[x], mid + 1, r, p);
}
int merge(int x, int y) {
    if (!x || !y) return x + y;
    int z = ++tot;
    ls[z] = merge(ls[x], ls[y]);
    rs[z] = merge(rs[x], rs[y]);
    return z;
}
int qry(int x, int l, int r, int ql, int qr) {
    if (!x) return 0;
    if (ql <= l && qr >= r) return 1;
    int mid = l + r >> 1, res = 0;
    if (ql <= mid) res |= qry(ls[x], l, mid, ql, qr);
    if (qr > mid) res |= qry(rs[x], mid + 1, r, ql, qr);
    return res;
}
char s[N];
int n, b[N], x[N], r[N], top[N], dp[N];
void solve() {
    int ans = 1;
    cnt = 0, lst = ++cnt;
    scanf("%d", &n);
    scanf("%s", s + 1);
    for (int i = 1; i <= n; i++) {
        extend(Tr(s[i]), i);
        upd(rt[lst], 1, n, i);
    }
    for (int i = 1; i <= cnt; i++) b[l[i]]++;
    for (int i = 1; i <= n; i++) b[i] += b[i - 1];
    for (int i = 1; i <= cnt; i++) x[b[l[i]]--] = i;
    for (int i = cnt; i > 1; i--) rt[f[x[i]]] = merge(rt[f[x[i]]], rt[x[i]]);
    for (int i = 2; i <= cnt; i++) {
        int u = x[i], fu = f[u];
        if (fu == 1) {
            dp[u] = 1, top[u] = u;
            continue;
        }
        int t = qry(rt[top[fu]], 1, n, pos[u] - l[u] + l[top[fu]], pos[u] - 1);
        dp[u] = dp[fu] + t;
        top[u] = t ? u : top[fu];
        ans = max(ans, dp[u]);
    }
    printf("%d\n", ans);
}
int main() {
    solve();
    return 0;
}
