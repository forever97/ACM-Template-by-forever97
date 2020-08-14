/*
    黑白边最小生成树
    给你一个无向带权连通图，每条边是黑色或白色。
    让你求一棵最小权的恰好有need条白色边的生成树，题目保证一定有解
    我们对所有的白色边加一个权值，那么排序后做mst选取的白色边数量增减性单调，
    对于增加的权值进行二分，验证能否满足要求即可。
    单点度限制最小生成树可以将其点相连边染色后转化为黑白边最小生成树
*/
const int N = 100010;
struct data {
    int a, b, u, c;
} p[N];
bool cmp(data a, data b) { return a.u < b.u; }
int ans, res, v, e, need, f[N];
int sf(int x) { return f[x] == x ? x : f[x] = sf(f[x]); }
bool check(int x) {
    int cnt = 0;
    res = 0;
    for (int i = 0; i < v; i++) f[i] = i;
    for (int i = 1; i <= e; i++) {
        if (!p[i].c) p[i].u += x;
    }
    sort(p + 1, p + e + 1, cmp);
    for (int i = 1; i <= e; i++) {
        if (!p[i].c) p[i].u -= x;
    }
    for (int i = 1; i <= e; i++) {
        if (p[i].u < x) continue;
        if (sf(p[i].a) != sf(p[i].b)) {
            res += p[i].u;
            f[sf(p[i].a)] = sf(p[i].b);
            cnt += (p[i].c == 0);
        }
    }
    return cnt >= need;
}
int main() {
    while (~scanf("%d%d%d", &v, &e, &need)) {
        for (int i = 1; i <= e; i++) {
            scanf("%d%d%d%d", &p[i].a, &p[i].b, &p[i].u, &p[i].c);
        }
        int l = -100, r = 100;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (check(mid))
                l = mid + 1, ans = res;
            else
                r = mid - 1;
        }
        printf("%d\n", ans);
    }
    return 0;
}