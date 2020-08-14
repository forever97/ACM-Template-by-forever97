/*
    尺取法
    Example：
        给出一些签到区间和一些补签卡，问可以创造的最长连续签到区间
    Solution：
        如果我们知道选定的最左和最右的签到区间，
        我们就可以计算出需要补多少的补签卡，如果数量小于等于给定数量，
        那么这个左右可以用来更新答案。所以尺取法就显而易见了。
*/
const int N = 100010;
struct data {
    int l, r;
} p[N];
int l[N], r[N], cnt;
bool cmp(data a, data b) {
    if (a.l == b.l) return a.r < b.r;
    return a.l < b.l;
}
int n, m;
int main() {
    while (~scanf("%d%d", &n, &m)) {
        for (int i = 1; i <= n; i++) scanf("%d%d", &p[i].l, &p[i].r);
        sort(p + 1, p + n + 1, cmp);
        cnt = 1;
        l[cnt] = p[1].l, r[cnt] = p[1].r;
        for (int i = 2; i <= n; i++) {
            if (p[i].l - 1 > r[cnt]) {
                l[++cnt] = p[i].l;
                r[cnt] = p[i].r;
            }
            r[cnt] = max(r[cnt], p[i].r);
        }
        int ans = r[1] - l[1] + 1 + m, gap = 0, lft = 1;
        for (int rt = 2; rt <= cnt; rt++) {
            gap += l[rt] - 1 - r[rt - 1];
            while (gap > m) {
                lft++;
                gap -= l[lft] - 1 - r[lft - 1];
            }
            ans = max(ans, r[rt] - l[lft] + 1 + m - gap);
        }
        printf("%d\n", ans);
    }
    return 0;
}
/*
    Example：
        给出一个序列，求一个子段和，使得其绝对值最接近给出值，
        输出这个区间的左右端点和区间和。
    Solution：
        因为原序列的前缀和不具有单调性，难以处理，
        因此我们对前缀和进行排序，同时保留前缀和的右端点做标识作用，
        题目要求区段和的绝对值最接近目标，因此排序不会造成前后顺序变化造成的影响
        现在题目转化为在一个有序数列中，求一个数对，使得差值最接近给出数，
        利用单调性，可以尺取解决问题。
    

*/
const int N = 100010, INF = INT_MAX;
typedef pair<int, int> P;
int n, m, s, t, ans, ansl, ansr;
P p[N];
int main() {
    while (scanf("%d%d", &n, &m), n && m) {
        p[0] = P(s = 0, 0);
        for (int i = 1; i <= n; i++) scanf("%d", &t), p[i] = P((s += t), i);
        sort(p, p + n + 1);
        while (m--) {
            scanf("%d", &t);
            int l = 0, r = 1, Min = INF;
            while (l <= n && r <= n) {
                int tmp = p[r].first - p[l].first;
                if (abs(tmp - t) < Min) {
                    Min = abs(tmp - t);
                    ans = tmp;
                    ansl = p[l].second;
                    ansr = p[r].second;
                }
                if (tmp > t)
                    l++;
                else if (tmp < t)
                    r++;
                else
                    break;
                if (l == r) r++;
            }
            if (ansl > ansr) swap(ansl, ansr);
            printf("%d %d %d\n", ans, ansl + 1, ansr);
        }
    }
    return 0;
}