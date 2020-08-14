/*
    Manacher+BIT
    问最少用几个回文串可以构成给出串，重叠部分可以合并
*/
char s[100010], a[50010];
const int INF = 0x3f3f3f3f;
int cnt, n, c[100010], f[100010];
struct seg {
    int l, r;
} l[100010];
void add_edge(int x, int y) {
    x = x / 2 + 1;
    y = y / 2 - 1;
    if (x > y) return;
    l[++cnt] = (seg){x, y};
}
bool operator<(seg a, seg b) { return a.r < b.r; }
void update(int x, int val) {
    while (x) c[x] = min(c[x], val), x -= x & -x;
}
int query(int x) {
    if (!x) return 0;
    int res = INF;
    while (x <= n) res = min(c[x], res), x += x & -x;
    return res;
}
void manacher(char *a) {
    int m, r, p, i;
    for (i = 1; i <= n; i++) s[i << 1] = a[i], s[i << 1 | 1] = '#';
    s[0] = '$', s[1] = '#', s[m = (n + 1) << 1] = '@';
    for (r = p = 0, f[1] = 1, i = 2; i < m; i++) {
        for (f[i] = r > i ? min(r - i, f[p * 2 - i]) : 1;
             s[i - f[i]] == s[i + f[i]]; f[i]++)
            ;
        add_edge(i - f[i], i + f[i]);
        if (i + f[i] > r) r = i + f[i], p = i;
    }
}
int main() {
    while (~scanf("%s", a + 1)) {
        n = strlen(a + 1);
        cnt = 0;
        for (int i = 1; i <= n; i++) c[i] = INF;
        manacher(a);
        sort(l + 1, l + cnt + 1);
        int ans = INF, tmp;
        for (int i = 1; i <= cnt; i++) {
            update(l[i].r, tmp = query(l[i].l - 1) + 1);
            if (l[i].r == n) ans = min(ans, tmp);
        }
        printf("%d\n", ans - 1);
    }
    return 0;
}