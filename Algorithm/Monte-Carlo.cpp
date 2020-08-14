/*
    蒙特卡罗
*/
/*
    Example:
        求图中的最大团
    Solution:
        我们随机一个排列，按照这个顺序求答案，对于一个放入答案集中的数，
        我们将其余不与其连接的数删除，顺序统计即可。
*/
int ans = 0, mp[55][55], del[55], n, b[55], x, y;
void r() {
    for (int i = 2; i <= n; i++) swap(b[i], b[rand() % i + 1]);
}
void cal() {
    memset(del, 0, sizeof(del));
    int t = 0;
    for (int i = 1; i <= n; i++)
        if (!del[i]) {
            t++;
            for (int j = i + 1; j <= n; j++)
                if (!mp[b[i]][b[j]]) del[j] = 1;
        }
    ans = max(t, ans);
}
int main() {
    scanf("%d", &n);
    while (~scanf("%d%d", &x, &y)) mp[x][y] = mp[y][x] = 1;
    for (int i = 1; i <= n; i++) b[i] = i;
    for (int i = 1; i <= 10000; i++) {
        r();
        cal();
    }
    printf("%d\n", ans);
    return 0;
}
/*
    Example:
        给一个长度为n的序列a。1≤a[i]≤n。
        m组询问，每次询问一个区间[l,r]，是否存在一个数在[l,r]中出现的次数大于(r-l+1)/2。
        如果存在，输出这个数，否则输出0。
*/
const int N = 500010;
vector<int> pos[N];
int a[N], l, r, n, m;
int main() {
    while (~scanf("%d%d", &n, &m)) {
        for (int i = 1; i <= n; i++) pos[i].clear();
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            pos[a[i]].push_back(i);
        }
        while (m--) {
            scanf("%d%d", &l, &r);
            int len = r - l + 1, find = 0;
            for (int k = 1; k <= 20; k++) {
                int u = a[l + rand() % len];
                int st = lower_bound(pos[u].begin(), pos[u].end(), l) -
                         pos[u].begin();
                int en = upper_bound(pos[u].begin(), pos[u].end(), r) -
                         pos[u].begin();
                if ((en - st) * 2 > len) {
                    find = u;
                    break;
                }
            }
            if (find)
                printf("%d\n", find);
            else
                puts("0");
        }
    }
    return 0;
}