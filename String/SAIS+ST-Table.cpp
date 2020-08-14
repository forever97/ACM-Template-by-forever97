/*
    SAIS+ST_Table
    给定n个字符串，输出第x串跟第y串的最长公共前缀
    SA的ST判断的时候注意相同位置后缀判断会出错，要特判
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 4000010;
namespace SA {
int sa[N], rk[N], ht[N], s[N << 1], t[N << 1], p[N], cnt[N], cur[N];
#define pushS(x) sa[cur[s[x]]--] = x
#define pushL(x) sa[cur[s[x]]++] = x
#define inducedSort(v)                                   \
    fill_n(sa, n, -1);                                   \
    fill_n(cnt, m, 0);                                   \
    for (int i = 0; i < n; i++) cnt[s[i]]++;             \
    for (int i = 1; i < m; i++) cnt[i] += cnt[i - 1];    \
    for (int i = 0; i < m; i++) cur[i] = cnt[i] - 1;     \
    for (int i = n1 - 1; ~i; i--) pushS(v[i]);           \
    for (int i = 1; i < m; i++) cur[i] = cnt[i - 1];     \
    for (int i = 0; i < n; i++)                          \
        if (sa[i] > 0 && t[sa[i] - 1]) pushL(sa[i] - 1); \
    for (int i = 0; i < m; i++) cur[i] = cnt[i] - 1;     \
    for (int i = n - 1; ~i; i--)                         \
        if (sa[i] > 0 && !t[sa[i] - 1]) pushS(sa[i] - 1)
void sais(int n, int m, int *s, int *t, int *p) {
    int n1 = t[n - 1] = 0, ch = rk[0] = -1, *s1 = s + n;
    for (int i = n - 2; ~i; i--)
        t[i] = s[i] == s[i + 1] ? t[i + 1] : s[i] > s[i + 1];
    for (int i = 1; i < n; i++)
        rk[i] = t[i - 1] && !t[i] ? (p[n1] = i, n1++) : -1;
    inducedSort(p);
    for (int i = 0, x, y; i < n; i++)
        if (~(x = rk[sa[i]])) {
            if (ch < 1 || p[x + 1] - p[x] != p[y + 1] - p[y])
                ch++;
            else
                for (int j = p[x], k = p[y]; j <= p[x + 1]; j++, k++)
                    if ((s[j] << 1 | t[j]) != (s[k] << 1 | t[k])) {
                        ch++;
                        break;
                    }
            s1[y = x] = ch;
        }
    if (ch + 1 < n1)
        sais(n1, ch + 1, s1, t + n, p + n1);
    else
        for (int i = 0; i < n1; i++) sa[s1[i]] = i;
    for (int i = 0; i < n1; i++) s1[i] = p[sa[i]];
    inducedSort(s1);
}
template <typename T>
int mapCharToInt(int n, const T *str) {
    int m = *max_element(str, str + n);
    fill_n(rk, m + 1, 0);
    for (int i = 0; i < n; i++) rk[str[i]] = 1;
    for (int i = 0; i < m; i++) rk[i + 1] += rk[i];
    for (int i = 0; i < n; i++) s[i] = rk[str[i]] - 1;
    return rk[m];
}
// 最后一位添加符一定要保证是字典序最小的字符
template <typename T>
void suffixArray(int n, const T *str) {
    int m = mapCharToInt(++n, str);
    sais(n, m, s, t, p);
    for (int i = 0; i < n; i++) rk[sa[i]] = i;
    for (int i = 0, h = ht[0] = 0; i < n - 1; i++) {
        int j = sa[rk[i] - 1];
        while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
        if (ht[rk[i]] = h) h--;
    }
}
};  // namespace SA
namespace ST {
int f[N][30], lg2[N];
void Init(int n) {
    for (int i = 2; i <= n; i++) lg2[i] = lg2[i / 2] + 1;
    for (int i = 1; i <= n; i++) f[i][0] = SA::ht[i];
    for (int j = 1; (1 << j) <= n; j++)
        for (int i = 1; i + (1 << j) - 1 <= n; i++)
            f[i][j] = min(f[i][j - 1], f[i + (1 << (j - 1))][j - 1]);
}
int Min(int l, int r) {
    if (l > r) swap(l, r);
    l++;
    int k = lg2[r - l + 1];
    return min(f[l][k], f[r - (1 << k) + 1][k]);
}
};  // namespace ST
int T, n, len[N], pos[N];
char t[N], s[N];
int main() {
    scanf("%d", &T);
    for (int cas = 1; cas <= T; cas++) {
        printf("Case %d:\n", cas);
        scanf("%d", &n);
        int u = 0;
        for (int i = 1; i <= n; i++) {
            scanf("%s", &t);
            len[i] = strlen(t);
            pos[i] = u;
            for (int j = 0; j < len[i]; j++) s[u++] = t[j];
        }
        s[u] = 'a' - 1;
        SA::suffixArray(u, s);
        ST::Init(u);
        int q;
        scanf("%d", &q);
        while (q--) {
            int x, y;
            scanf("%d%d", &x, &y);
            int ans = min(len[x], len[y]);
            if (x == y)
                printf("%d\n", len[x]);
            else
                printf("%d\n",
                       min(ans, ST::Min(SA::rk[pos[x]], SA::rk[pos[y]])));
        }
    }
    return 0;
}
