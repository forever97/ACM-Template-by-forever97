/*
    单增莫队
    对于左端点在同一个块内的询问，每次还原左端点到块的最右端
    然后移动左端点和右端点到对应位置
    这样就只有区间扩增操作，没有区间缩减操作
    用于非可加类答案的统计
    Example：求区间内数字*出现次数的最大值
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N = 100000 + 10;
int limit, block, pos[N], a[N], cnt[N];
ll lst_ans, ans, Ans[N];
int T, n, m;
struct Q {
    int l, r, id;
    friend bool operator<(const Q &a, const Q &b) {
        return pos[a.l] < pos[b.l] || (pos[a.l] == pos[b.l] && a.r < b.r);
    }
} ask[N];
int b[N], sa[N];
void Discretization() {
    sort(b + 1, b + n + 1);
    int siz = unique(b + 1, b + n + 1) - (b + 1);
    for (int i = 1; i <= n; i++)
        sa[i] = lower_bound(b + 1, b + siz + 1, a[i]) - b;
}
void modify(int x) {
    cnt[sa[x]]++;
    if (1ll * cnt[sa[x]] * a[x] > ans) {
        ans = 1ll * cnt[sa[x]] * a[x];
    }
}
int main() {
    scanf("%d%d", &n, &m);
    block = (int)sqrt(n + 0.5);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        b[i] = a[i];
        pos[i] = (i - 1) / block + 1;
    }
    Discretization();
    for (int i = 1; i <= m; i++) {
        scanf("%d%d", &ask[i].l, &ask[i].r);
        ask[i].id = i;
    }
    sort(ask + 1, ask + m + 1);
    int L = 1, R = 0;
    lst_ans = ans = 0;
    ask[0].l = 0;
    pos[0] = 0;
    for (int i = 1; i <= m; ++i) {
        if (pos[ask[i - 1].l] != pos[ask[i].l]) {
            memset(cnt, 0, sizeof(cnt));
            limit = pos[ask[i].l] * block;
            R = limit;
            ans = lst_ans = 0;
        }
        L = min(limit + 1, ask[i].r + 1);
        while (R < ask[i].r) modify(++R);
        lst_ans = ans;
        while (L > ask[i].l) modify(--L);
        Ans[ask[i].id] = ans;
        for (int j = L; j <= ask[i].r && j <= limit; ++j) {
            cnt[sa[j]]--;
        }
        ans = lst_ans;
    }
    for (int i = 1; i <= m; ++i) printf("%lld\n", Ans[i]);
    return 0;
}