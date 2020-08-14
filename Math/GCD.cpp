// GCD(Fib(n),Fib(m))=GCD(Fib(n-m),Fib(m))
// GCD(a^m−b^m,a^n−b^n) = a ^ GCD(m,n) − b ^ GCD(m,n) [GCD(a,b)==1]
// 区间GCD
/*
    Problem: 
        求区间GCD和与其区间GCD值相同的区间数量
    Solution：
        固定右端点，求出区间GCD值的左端点分界线，每个右端点只有logn个分界线，
        查询对应分界线和GCD值即可，用map保存GCD值的数量
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <map>
using namespace std;
map<int, long long> M;
const int N = 100005;
int Gcd, n, l[N], v[N], len[N], T, j, q, L, R, Cas, a[N];
struct data {
    int l, v;
} p[N][50];
int gcd(int x, int y) { return __gcd(x, y); }
int main() {
    scanf("%d", &T);
    while (T--) {
        printf("Case #%d:\n", ++Cas);
        M.clear();
        scanf("%d", &n);
        memset(len, 0, sizeof(len));
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        for (int i = 1; i <= n; i++)
            for (v[i] = a[i], j = l[i] = i; j; j = l[j] - 1) {
                v[j] = gcd(v[j], a[i]);
                while (l[j] > 1 && gcd(a[i], v[l[j] - 1]) == gcd(a[i], v[j]))
                    l[j] = l[l[j] - 1];
                p[i][len[i]].l = l[j];
                p[i][len[i]++].v = v[j];
                M[v[j]] += (j - l[j] + 1);
            }
        scanf("%d", &q);
        while (q--) {
            scanf("%d%d", &L, &R);
            for (int i = 0; i < len[R]; i++) {
                if (L >= p[R][i].l) {
                    Gcd = p[R][i].v;
                    break;
                }
            }
            printf("%d %lld\n", Gcd, M[Gcd]);
        }
    }
    return 0;
}
/*
    Problem：
        给定一个数列，多次查询区间[l,r]，询问其中不同的子区间gcd的数量
    Solution：
        将所有的查询按右区间从小到大排序，在树状数组上顺序更新每个右端点对应的GCD分界线，
        注意当一个 GCD值第二次出现的时候，需要将上一次出现位置消除，更新现在所在的位置，
        对树状数组区间求和即可得到答案
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 1000010;
int n, q, v[N], l[N], j, a[N], ans[N], pre[N], c[N];
struct Data {
    int l, r, id;
} p[N];
bool cmp(Data a, Data b) { return a.r < b.r; }
void upd(int x, int y) {
    for (; x <= n; x += x & -x) c[x] += y;
}
int query(int x) {
    int s = 0;
    while (x) {
        s += c[x];
        x -= x & -x;
    }
    return s;
}
int main() {
    while (~scanf("%d%d", &n, &q)) {
        memset(pre, -1, sizeof(pre));
        memset(c, 0, sizeof(c));
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        for (int i = 1; i <= q; i++) {
            scanf("%d%d", &p[i].l, &p[i].r);
            p[i].id = i;
        }
        sort(p + 1, p + q + 1, cmp);
        int pos = 1;
        for (int i = 1; i <= n; i++) {
            for (v[i] = a[i], j = l[i] = i; j; j = l[j] - 1) {
                v[j] = __gcd(v[j], a[i]);
                while (l[j] > 1 && __gcd(a[i], v[l[j] - 1]) == __gcd(a[i], v[j]))
                    l[j] = l[l[j] - 1];
                if (~pre[v[j]]) upd(pre[v[j]], -1);
                upd(j, 1);
                pre[v[j]] = j;
            }
            while (p[pos].r == i) {
                ans[p[pos].id] = query(p[pos].r) - query(p[pos].l - 1);
                pos++;
            }
        }
        for (int i = 1; i <= q; i++) printf("%d\n", ans[i]);
    }
    return 0;
}
