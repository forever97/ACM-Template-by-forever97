/*
    莫队算法
    求区间内所有数出现次数的平方和
*/
const int N = 50005;
using namespace std;
typedef long long LL;
int pos[N], num[N], n, m, k, limit, i, l, r, S[N];
LL ans;
struct Q {
    int l, r, id;
    LL ans;
    friend bool operator<(const Q &a, const Q &b) {
        return pos[a.l] < pos[b.l] || (pos[a.l] == pos[b.l] && a.r < b.r);
    }
} ask[N];
inline bool cmp(const Q &a, const Q &b) { return a.id < b.id; }
inline void read(int &a) {
    char ch;
    while (!((ch = getchar()) >= '0') && (ch <= '9'));
    a = ch - '0';
    while (((ch = getchar()) >= '0') && (ch <= '9')) a *= 10, a += ch - '0';
}
inline void modify(int p, LL &ans, int add) {
    ans = ans - (LL)S[num[p]] * S[num[p]];
    S[num[p]] += add;
    ans = ans + (LL)S[num[p]] * S[num[p]];
}
int main() {
    read(n);
    read(m);
    read(k);
    limit = (int)sqrt(n + 0.5);
    for (i = 1; i <= n; i++) {
        read(num[i]);
        pos[i] = (i - 1) / limit + 1;
    }
    for (i = 1; i <= m; i++) {
        read(ask[i].l);
        read(ask[i].r);
        ask[i].id = i;
    }
    ans = 0;
    sort(ask + 1, ask + m + 1);
    for (i = 1, l = 1, r = 0; i <= m; i++) {
        while (r < ask[i].r) modify(++r, ans, 1);
        while (r > ask[i].r) modify(r--, ans, -1);
        while (l < ask[i].l) modify(l++, ans, -1);
        while (l > ask[i].l) modify(--l, ans, 1);
        ask[i].ans = ans;
    }
    sort(ask + 1, ask + m + 1, cmp);
    for (i = 1; i <= m; i++) printf("%lld\n", ask[i].ans);
    return 0;
}