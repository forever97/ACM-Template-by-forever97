/*
    群论
    polya定理： 一个置换的不动置换方案数=k^(这个置换的循环个数)
    burnside定理 ：不等价方案数=每个置换的不动置换方案数的和 / 置换个数
*/
/*
    Example：
        给出一个序列
        交换值为x和y的两个数代价为x+y
        求从小排到大的代价
    Solution：
        1.找出初始状态和目标状态。明显，目标状态就是排序后的状态。
        2.画出置换群，在里面找循环。例如，数字是8 4 5 3 2 7
        明显，目标状态是2 3 4 5 7 8，能写为两个循环：
        (8 2 7)(4 3 5)。
        3.观察其中一个循环，明显地，要使交换代价最小，
        应该用循环里面最小的数字2，去与另外的两个数字，7与8交换。这样交换的代价是：
            sum – min + (len – 1) * min
        化简后为：
            sum + (len – 2) * min
        其中，sum为这个循环所有数字的和，len为长度，min为这个环里面最小的数字。
        4.考虑到另外一种情况，我们可以从别的循环里面调一个数字，
        进入这个循环之中，使交换代价更小。例如初始状态：
        1 8 9 7 6
        可分解为两个循环：
        (1)(8 6 9 7)，明显，第二个循环为(8 6 9 7)，最小的数字为6。、
        我们可以抽调整个数列最小的数字1进入这个循环。
        使第二个循环变为：(8 1 9 7)。让这个1完成任务后，
        再和6交换，让6重新回到循环之后。这样做的代价明显是：
        sum + min + (len + 1) * smallest
        其中，sum为这个循环所有数字的和，len为长度，
        min为这个环里面最小的数字，smallest是整个数列最小的数字。
        5.因此，对一个循环的排序，其代价是sum – min + (len – 1) * min
        和sum + min + (len + 1) * smallest之中小的那个数字。
        6.我们在计算循环的时候，不需要记录这个循环的所有元素，
        只需要记录这个循环的最小的数及其和。
        7.在储存数目的时候，我们可以使用一个hash结构，
        将元素及其位置对应起来，以达到知道元素，
        可以快速反查元素位置的目的。这样就不必要一个个去搜索。
*/
const int N = 10005, inf = 1000000000;
int n, gmn, cnt, ans, v[N], id[N], disc[N], mn[N], sum[N], len[N];
bool vis[N];
int find(int x) {
    int l = 1, r = n;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (disc[mid] > x)
            r = mid - 1;
        else if (disc[mid] == x)
            return mid;
        else
            l = mid + 1;
    }
}
void solve(int x) {
    vis[x] = 1;
    cnt++;
    len[cnt] = 1;
    sum[cnt] += v[x];
    mn[cnt] = min(mn[cnt], v[x]);
    int now = x;
    while (v[id[now]] != v[x]) {
        now = id[now];
        vis[now] = 1;
        len[cnt]++;
        sum[cnt] += v[now];
        mn[cnt] = min(mn[cnt], v[now]);
    }
}
int main() {
    memset(mn, 127 / 3, sizeof(mn));
    gmn = inf;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &v[i]), disc[i] = v[i], gmn = min(gmn, v[i]);
    sort(disc + 1, disc + n + 1);
    for (int i = 1; i <= n; i++) id[i] = find(v[i]);
    for (int i = 1; i <= n; i++)
        if (!vis[i] && i != id[i]) solve(i);
    for (int i = 1; i <= cnt; i++) {
        int t1 = (len[i] - 2) * mn[i];
        int t2 = mn[i] + (len[i] + 1) * gmn;
        ans += sum[i] + min(t1, t2);
    }
    return printf("%d", ans), 0;
}
/*
    Example:
        给定M种颜色的珠子，每种颜色珠子的个数均不限，将这些珠子做成长度为N的项链。
        问能做成多少种不重复的项链，最后的结果不会超过int类型数据的表示范围。
        并且两条项链相同，当且仅当两条项链通过旋转或是翻转后能重合在一起，
        且对应珠子的颜色相同。
*/
int N, M;
int main() {
    while (~scanf("%d%d", &M, &N) && (M || N)) {
        int ans = 0;
        for (int i = 1; i <= N; i++) {
            int tmp = __gcd(N, i);
            ans += (int)(pow(M * 1.0, tmp * 1.0));
        }
        if (N & 1)
            ans += (int)(N * pow(M * 1.0, (N + 1) / 2.0));
        else {
            ans += (int)((N / 2) * pow(M * 1.0, (N + 2) / 2.0));
            ans += (int)((N / 2) * pow(M * 1.0, N / 2.0));
        }
        ans = ans / (2 * N);
        printf("%d\n", ans);
    }
    return 0;
}
/*
    Example:
        一个手镯，用三种颜色上色，可以旋转和翻转，求有多少方案。
*/
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }
int main() {
    int c = 3, s;
    while (scanf("%d", &s) && s != -1) {
        if (s == 0) {
            puts("0");
            continue;
        }
        long long p[64];
        p[0] = 1;
        for (int k = 0; k < s; k++) p[k + 1] = p[k] * c;
        long long cnt =
            s & 1 ? p[s / 2 + 1] * s : (p[s / 2] + p[s / 2 + 1]) * (s / 2);
        for (int k = 1; k <= s; k++) cnt += p[gcd(k, s)];
        printf("%lld\n", cnt /= 2 * s);
    }
    return 0;
}