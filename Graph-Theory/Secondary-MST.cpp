/*
    次小生成树
    复杂度O(nm)
    Example: 判定是否存在唯一最小生成树
*/
const int N = 10005;
int cnt, n, m, f[N], mst[N], T, tot, ans;
struct data {
    int l, r, c;
} e[N];
int sf(int x) { return x == f[x] ? x : f[x] = sf(f[x]); }
bool cmp(data a, data b) { return a.c < b.c; }
int init() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= m; i++) scanf("%d%d%d", &e[i].l, &e[i].r, &e[i].c);
}
void kruskal() {
    sort(e + 1, e + m + 1, cmp);
    for (int i = 0; i <= n; i++) f[i] = i;
    cnt = ans = 0;
    for (int i = 1; i <= m && cnt < n - 1; i++) {
        if (sf(e[i].l) != sf(e[i].r)) {
            f[f[e[i].l]] = f[e[i].r];
            ans += e[i].c;
            mst[++cnt] = i;
        }
    }
    for (int i = 1; i < n; i++) {
        tot = cnt = 0;
        for (int j = 0; j <= n; j++) f[j] = j;
        for (int j = 1; j <= m; j++) {
            if (j == mst[i]) continue;
            if (sf(e[j].l) != sf(e[j].r)) {
                f[f[e[j].l]] = f[e[j].r];
                tot += e[j].c;
                cnt++;
            }
        }
        if (cnt != n - 1) continue;
        if (tot == ans) {
            puts("Not Unique!");
            return;
        }
    }
    printf("%d\n", ans);
}
int main() {
    scanf("%d", &T);
    while (T--) {
        init();
        kruskal();
    }
    return 0;
}