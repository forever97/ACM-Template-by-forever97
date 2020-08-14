/*
    区间逆序对
    莫队+树状数组
*/
const int N = 50100;
using namespace std;
typedef long long LL;
int pos[N], a[N], disc[N], n, m, limit, i, l, r, c[N];
struct Q {
    int l, r, id;
    LL ans;
    friend bool operator<(const Q &a, const Q &b) {
        return pos[a.l] < pos[b.l] || (pos[a.l] == pos[b.l] && a.r < b.r);
    }
} ask[N];
bool cmp(const Q &a, const Q &b) { return a.id < b.id; }
void read(int &a) {
    char ch;
    while (!((ch = getchar()) >= '0') && (ch <= '9'))
        ;
    a = ch - '0';
    while (((ch = getchar()) >= '0') && (ch <= '9')) a *= 10, a += ch - '0';
}
void update(int x, int val) {
    while (x <= n) c[x] += val, x += x & -x;
}
LL query(int x) {
    LL res = 0;
    while (x) res += c[x], x -= x & -x;
    return res;
}
int main() {
    read(n);
    limit = (int)sqrt(n + 0.5);
    for (i = 1; i <= n; i++) {
        read(a[i]);
        pos[i] = (i - 1) / limit + 1;
    }
    for (int i = 1; i <= n; i++) disc[i] = a[i];
    sort(disc + 1, disc + n + 1);
    for (int i = 1; i <= n; i++)
        a[i] = lower_bound(disc + 1, disc + n + 1, a[i]) - disc;
    read(m);
    memset(c, 0, sizeof(c));
    for (int i = 1; i <= m; i++) {
        read(ask[i].l);
        read(ask[i].r);
        ask[i].id = i;
    }
    sort(ask + 1, ask + m + 1);
    LL ans = 0;
    for (int i = 1, l = 1, r = 0; i <= m; i++) {
        while (r < ask[i].r)
            r++, update(a[r], 1), ans += r - l + 1 - query(a[r]);
        while (r > ask[i].r) update(a[r], -1), ans -= r - l - query(a[r]), r--;
        while (l < ask[i].l) update(a[l], -1), ans -= query(a[l] - 1), l++;
        while (l > ask[i].l) l--, update(a[l], 1), ans += query(a[l] - 1);
        ask[i].ans = ans;
    }
    sort(ask + 1, ask + m + 1, cmp);
    for (int i = 1; i <= m; i++) printf("%lld\n", ask[i].ans);
    return 0;
}