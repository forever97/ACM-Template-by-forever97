/*
    莫队+分块
    查询[l,r]中[a,b]范围内的数字种类数
*/
const int N = 100001, M = 1000001;
using namespace std;
typedef long long LL;
int size[N], ans[M], pos[N], color[N], c[400], n, m, limit, bl[400], br[400];
struct Q {
    int l, r, id, a, b;
    friend bool operator<(const Q &a, const Q &b) {
        return pos[a.l] < pos[b.l] || (pos[a.l] == pos[b.l] && a.r < b.r);
    }
} ask[M];
bool cmp(const Q &a, const Q &b) { return a.id < b.id; }
void read(int &a) {
    char ch;
    while (!((ch = getchar()) >= '0') && (ch <= '9'))
        ;
    a = ch - '0';
    while (((ch = getchar()) >= '0') && (ch <= '9')) a *= 10, a += ch - '0';
}
int query(int x, int y) {
    int res = 0;
    int L = pos[x], R = pos[y];
    for (int i = L + 1; i < R; i++) res += c[i];
    if (L == R) {
        for (int i = x; i <= y; i++)
            if (size[i]) res++;
    } else {
        for (int i = x; i <= br[L]; i++)
            if (size[i]) res++;
        for (int i = bl[R]; i <= y; i++)
            if (size[i]) res++;
    }
    return res;
}
void modify(int u, int x) {
    if (size[color[u]] == 1 && x == -1) c[pos[color[u]]]--;
    if (size[color[u]] == 0 && x == 1) c[pos[color[u]]]++;
    size[color[u]] += x;
}
int main() {
    read(n);
    read(m);
    limit = (int)sqrt(n + 0.5);
    for (int i = 1; i <= n; i++) {
        read(color[i]);
        pos[i] = (i - 1) / limit + 1;
    }
    for (int i = 1; i <= n; i++) {
        br[pos[i]] = i;
        if (!bl[pos[i]]) bl[pos[i]] = i;
    }
    for (int i = 1; i <= m; i++) {
        read(ask[i].l);
        read(ask[i].r);
        read(ask[i].a);
        read(ask[i].b);
        ask[i].id = i;
    }
    sort(ask + 1, ask + m + 1);
    for (int i = 1, l = 1, r = 0; i <= m; i++) {
        while (r < ask[i].r) modify(++r, 1);
        while (r > ask[i].r) modify(r--, -1);
        while (l < ask[i].l) modify(l++, -1);
        while (l > ask[i].l) modify(--l, 1);
        ans[ask[i].id] = query(ask[i].a, ask[i].b);
    }
    for (int i = 1; i <= m; i++) printf("%d\n", ans[i]);
    return 0;
}