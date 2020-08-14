/*
    整体二分
    Example：
        有N个成员国。现在它发现了一颗新的星球，
        这颗星球的轨道被分为M份（第M份和第1份相邻），第i份上有第Ai个国家的太空站。
        这个星球经常会下陨石雨。BIU已经预测了接下来K场陨石雨的情况。
        BIU的第i个成员国希望能够收集Pi单位的陨石样本。
        你的任务是判断对于每个国家，它需要在第几次陨石雨之后，才能收集足够的陨石。
    Solution：
        如果枚举每场陨石雨，树状数组查询每个国家是否完成收集，
        复杂度O(Kmlogm),难以接受
        因此我们对答案整体二分，不同位置的答案不断划分到对应的陨石时段计算。
        将完成任务的国家放到左边区间，未达成的放到右边区间。复杂度O(mlogmlogK).
*/
const int N = 300010;
typedef long long LL;
int x, n, m, T, K, l[N], r[N], val[N], id[N], p[N], tmp[N], ans[N], mark[N];
LL c[N];
vector<int> v[N];
LL query(int x) {
    LL s = 0;
    while (x) s += c[x], x -= x & -x;
    return s;
}
void add(int x, int val) {
    while (x <= m) c[x] += val, x += x & -x;
}
void update(int x, int k) {
    if (l[x] <= r[x])
        add(l[x], k * val[x]), add(r[x] + 1, -k * val[x]);
    else {
        add(1, k * val[x]), add(r[x] + 1, -k * val[x]);
        add(l[x], k * val[x]);
    }
}
void solve(int l, int r, int L, int R) {
    if (l > r) return;
    if (L == R) {
        for (int i = l; i <= r; i++) ans[id[i]] = L;
        return;
    }
    int mid = (L + R) >> 1;
    while (T <= mid) update(++T, 1);
    while (T > mid) update(T--, -1);
    int cnt = 0, x;
    LL tot;
    for (int i = l; i <= r; i++) {
        tot = 0;
        x = id[i];
        for (int j = 0; j < v[x].size(); j++) {
            tot += query(v[x][j]);
            if (tot >= p[x]) break;
        }
        if (tot >= p[x])
            mark[x] = 1, cnt++;
        else
            mark[x] = 0;
    }
    int l1 = l, l2 = l + cnt;
    for (int i = l; i <= r; i++) {
        if (mark[id[i]])
            tmp[l1++] = id[i];
        else
            tmp[l2++] = id[i];
    }
    for (int i = l; i <= r; i++) id[i] = tmp[i];
    solve(l, l1 - 1, L, mid);
    solve(l1, l2 - 1, mid + 1, R);
}
int main() {
    while (~scanf("%d%d", &n, &m)) {
        // 记录每个国家都有哪些太空站
        for (int i = 1; i <= m; i++) scanf("%d", &x), v[x].push_back(i);
        for (int i = 1; i <= n; i++) scanf("%d", &p[i]);
        scanf("%d", &K);
        for (int i = 1; i <= K; i++) scanf("%d%d%d", &l[i], &r[i], &val[i]);
        l[++K] = 1;
        r[K] = m;
        val[K] = 0x3f3f3f3f;
        for (int i = 1; i <= n; i++) id[i] = i;
        solve(1, n, 1, K);
        for (int i = 1; i <= n; i++) {
            if (ans[i] == K)
                puts("NIE");
            else
                printf("%d\n", ans[i]);
        }
    }
    return 0;
}
/*
    Example：
        单点修改，区间求第k大
    Solution：
        我们对询问和修改进行整体二分，分治第k小的数值，在当前层完成分治数值以下的修改和查询，
        我们用下标权值树状数组维护数值，当一个数字满足要求时在其下标位置进行修改操作，
        查询只需查询区间的数字数量是否满足k个即可判定询问的分治区间
        我们按照数值划分修改操作，按照是否满足区间存在至少k个值来划分询问，
        最后分到各个叶节点的询问答案即分治层的数值
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 200010, INF = 0x3f3f3f3f;
int c[N], a[N], ans[N];
int T, n, m;
void add(int x, int val) {
    while (x <= n) c[x] += val, x += x & -x;
}
int query(int x) {
    int s = 0;
    while (x) s += c[x], x -= x & -x;
    return s;
}
struct data {
    int x, y, k, id, isq;
} q[N], ql[N], qr[N];
void solve(int l, int r, int L, int R) {
    if (l > r) return;
    if (L == R) {
        for (int i = l; i <= r; i++) {
            if (q[i].isq) ans[q[i].id] = L;
        }
        return;
    }
    int mid = (L + R) >> 1;
    int p1 = 0, p2 = 0;
    for (int i = l; i <= r; i++) {
        if (!q[i].isq) {
            if (q[i].x <= mid) {
                add(q[i].id, q[i].y);
                ql[p1++] = q[i];
            } else
                qr[p2++] = q[i];
        } else {
            int cnt = query(q[i].y) - query(q[i].x - 1);
            if (cnt >= q[i].k)
                ql[p1++] = q[i];
            else {
                q[i].k -= cnt;
                qr[p2++] = q[i];
            }
        }
    }
    for (int i = 0; i < p1; i++) {
        if (!ql[i].isq) add(ql[i].id, -ql[i].y);
        q[l + i] = ql[i];
    }
    for (int i = 0; i < p2; i++) q[l + p1 + i] = qr[i];
    solve(l, l + p1 - 1, L, mid);
    solve(l + p1, r, mid + 1, R);
}
int main() {
    scanf("%d", &T);
    while (T--) {
        memset(c, 0, sizeof(c));
        scanf("%d%d", &n, &m);
        int cnt = 0, cntq = 0;
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            q[++cnt] = (data){a[i], 1, INF, i, 0};
        }
        for (int i = 1; i <= m; i++) {
            int x, y, k;
            char op[10];
            scanf("%s%d%d", op, &x, &y);
            if (op[0] == 'Q') {
                scanf("%d", &k);
                q[++cnt] = (data){x, y, k, ++cntq, 1};
            } else {
                q[++cnt] = (data){a[x], -1, INF, x, 0};
                a[x] = y;
                q[++cnt] = (data){y, 1, INF, x, 0};
            }
        }
        solve(1, cnt, -INF, INF);
        for (int i = 1; i <= cntq; i++) printf("%d\n", ans[i]);
    }
    return 0;
}
