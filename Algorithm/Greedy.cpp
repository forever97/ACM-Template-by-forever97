/*
    经典贪心问题
*/
/*
    题目大意：
        给出一列数，求最多取m段连续的数字，使得总和最大
    题解：
        首先我们对数据进行合并处理，连续的一段正数或者连续的一段负数处理成一个数字，
        之后我们发现，如果正数的个数小于等于m，那么直接输出正数的总和即可，
        如果大于m，我们有些正数不选，或者选择一些负数把左右两端的正数并起来。
        这个负数的选择过程相当于减去这个数的绝对值，
        正数选择拿出去的过程也相当于减去这个数的绝对值，
        在选择一个负数合并的过程中，两边的正数必须是没有被操作过的，
        同样，选择一个正数删去的过程中，两边的负数肯定也必须是没有操作过的，
        那么问题就转化为，给你一些数，请你选择其中k个不相邻的数，使得其和最小，
*/
#include <algorithm>
#include <cstdio>
#include <queue>
using namespace std;
typedef pair<int, int> P;
const int N = 100010;
const int INF = 0x3f3f3f3f;
int n, m, a[N], b[N], l[N], r[N];
int main() {
    while (~scanf("%d%d", &n, &m)) {
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        while (a[n] <= 0) n--;
        int st = 1;
        while (a[st] <= 0) st++;
        int cnt = 0, ans = 0;
        for (; st <= n; st++) {
            if (!((a[st] > 0) ^ (a[st - 1] > 0)))
                b[cnt] += a[st];
            else
                b[++cnt] = a[st];
        }
        for (int i = 1; i <= cnt; i++)
            if (b[i] > 0) {
                ans += b[i];
                m--;
            } else
                b[i] = -b[i];
        if (m >= 0) {
            printf("%d\n", ans);
            continue;
        }
        priority_queue<P, vector<P>, greater<P> > Q;
        for (int i = 1; i <= cnt; i++)
            l[i] = i - 1, r[i] = i + 1, Q.push(P(b[i], i));
        r[cnt] = 0;
        for (int i = 1; i <= -m; i++) {
            while (b[Q.top().second] != Q.top().first) Q.pop();
            int x = Q.top().second;
            Q.pop();
            ans -= b[x];
            if (!l[x]) {
                b[r[x]] = INF;
                l[r[x]] = 0;
            } else if (!r[x]) {
                b[l[x]] = INF;
                r[l[x]] = 0;
            } else {
                b[x] = b[l[x]] + b[r[x]] - b[x];
                b[l[x]] = b[r[x]] = INF;
                r[l[x] = l[l[x]]] = l[r[x] = r[r[x]]] = x;
                Q.push(P(b[x], x));
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
/*
    题目大意：
        现在有一些线段[l,r]的需求需要满足，i位置最多允许a[i]条线段堆叠,
        问最多能满足多少条线段的需求
    题解：
        我们将所有的线段按照右端点排序，那么从头到尾考虑能不能满足需求一定能得到最优解，
        因为对于相同右端点的来说，先后顺序不影响放入，
        而对于右端点不同的来说，右端点靠前的先处理一定比靠后的先处理更优。
        处理方式相当于线段树的区间查询和区间修改。
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int N = 100010;
const int INF = 0x3f3f3f3f;
struct data {
    int l, r;
} p[N];
bool cmp(data a, data b) { return a.r < b.r; }
int tag[N << 2], T[N << 2], a[N];
void up(int x) { T[x] = min(T[x << 1], T[x << 1 | 1]); }
void pd(int x) {
    if (tag[x]) {
        T[x << 1] += tag[x];
        T[x << 1 | 1] += tag[x];
        tag[x << 1] += tag[x];
        tag[x << 1 | 1] += tag[x];
        tag[x] = 0;
    }
}
void build(int x, int l, int r) {
    int mid = (l + r) >> 1;
    if (l == r) {
        T[x] = a[l];
        tag[x] = 0;
        return;
    }
    build(x << 1, l, mid);
    build(x << 1 | 1, mid + 1, r);
    up(x);
}
void update(int x, int l, int r, int L, int R, int p) {
    int mid = (l + r) >> 1;
    if (L <= l && r <= R) {
        T[x] += p;
        tag[x] += p;
        return;
    }
    pd(x);
    if (L <= mid) update(x << 1, l, mid, L, R, p);
    if (R > mid) update(x << 1 | 1, mid + 1, r, L, R, p);
    up(x);
}
int query(int x, int l, int r, int L, int R) {
    int mid = (l + r) >> 1;
    if (L <= l && r <= R) return T[x];
    pd(x);
    int res = INF;
    if (L <= mid) res = min(res, query(x << 1, l, mid, L, R));
    if (R > mid) res = min(res, query(x << 1 | 1, mid + 1, r, L, R));
    return res;
}
int n, m;
int main() {
    while (~scanf("%d%d", &n, &m)) {
        for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
        build(1, 1, n);
        for (int i = 1; i <= m; i++) scanf("%d%d", &p[i].l, &p[i].r);
        sort(p + 1, p + m + 1, cmp);
        int ans = 0;
        for (int i = 1; i <= m; i++) {
            int x = query(1, 1, n, p[i].l, p[i].r);
            if (x) {
                ans++;
                update(1, 1, n, p[i].l, p[i].r, -1);
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
