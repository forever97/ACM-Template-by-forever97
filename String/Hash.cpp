/*
    字符串哈希
    循环同构
    Problem：
        给出两个字符串，判断他们每一个前缀是否循环同构，
        循环同构的意思就是，字符串首位相接拼成一个环，两个环通过旋转可以相等。
    Solution：
        这道题用到了一个神奇的结论，如果S字符串和T字符串循环同构，
        那么必有S=u+v，T=v+u，而且u和v必有一个是最长匹配。
        那么根据这个结论，我们可以用KMP算法在T中找S的最长前缀，
        也就是每次匹配到i时候的j。那么对于T的前缀和S的后缀，
        我们直接用哈希来判断是否相等，这样子就可以计算出两个串的前缀是否循环同构了。
*/
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 10005, base = 233;
typedef long long ll;
int T, nxt[N], ans[N];
char a[N], b[N];
ll hash[2][N], p[N];
ll get_hash(int t, int L, int R) {
    return hash[t][R] - hash[t][L - 1] * p[R - L + 1];
}
int check(int t, int m, int n) {
    if (m == n) return 1;
    return get_hash(t, m + 1, n) == get_hash(t ^ 1, 1, n - m);
}
void kmp(int n, char* a, int m, char* b, int t) {
    int i, j;
    for (nxt[1] = j = 0, i = 2; i <= n; nxt[i++] = j) {
        while (j && a[j + 1] != a[i]) j = nxt[j];
        if (a[j + 1] == a[i]) j++;
    }
    for (j = 0, i = 1; i <= m; i++) {
        while (j && a[j + 1] != b[i]) j = nxt[j];
        if (a[j + 1] == b[i]) {
            j++;
            if (!ans[i]) ans[i] = check(t, j, i);
        }
        if (j == n) j = nxt[j];
    }
}
int main() {
    for (int i = p[0] = 1; i < N; i++) p[i] = p[i - 1] * base;
    while (~scanf(" %s %s", a + 1, b + 1)) {
        int n = strlen(a + 1);
        for (int i = 1; i <= n; i++) {
            ans[i] = 0;
            hash[0][i] = hash[0][i - 1] * base + a[i];
            hash[1][i] = hash[1][i - 1] * base + b[i];
        }
        kmp(n, a, n, b, 0);
        kmp(n, b, n, a, 1);
        for (int i = 1; i <= n; i++) printf("%d", ans[i]);
        puts("");
    }
    return 0;
}
/*
    集合哈希
    Problem:
        规定一个字符串和另一个字符串匹配的条件为首尾两个字符相同，且所有字符的出现次数相同
        现在给定一个母串，询问多个子串，问子串在母串中的匹配次数
        子串长度和 \le 10^5，母串长度 \le 10^5
    Solution:
        我们将询问串按照长度分组，对于每种分组，我们计算母串中对应长度的哈希值
        然后统计对应组询问在该长度子串的哈希值中出现次数即可
        我们对除串首尾做字符的集合哈希，串首尾特殊处理累加到哈希值上去
        考虑子串长度和限制，复杂度均摊，最坏情况O(n\sqrt{n})
*/
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
const ull base = 131;
const int N = 100000 + 10;
int n, T, L[N], ans[N];
ull p[125], Hash[N], h[N];
char s[N], t[N];
vector<int> id[N];
int main() {
    scanf("%d", &T);
    for (int i = p[0] = 1; i < 125; i++) p[i] = p[i - 1] * base;
    while (T--) {
        scanf("%s", s);
        scanf("%d", &n);
        int len = strlen(s);
        for (int i = 1; i <= n; i++) {
            scanf("%s", t);
            Hash[i] = 0;
            L[i] = strlen(t);
            id[L[i]].push_back(i);
            for (int j = 1; j < L[i] - 1; j++) Hash[i] += p[t[j]];
            Hash[i] += p[123] * t[0];
            Hash[i] += p[124] * t[L[i] - 1];
        }
        sort(L + 1, L + n + 1);
        int cnt = unique(L + 1, L + n + 1) - (L + 1);
        for (int i = 1; i <= cnt; i++) {
            ull H = 0;
            int tot = 0;
            for (int j = 0; j < len; j++) {
                if (j >= L[i] - 1) {
                    H -= p[s[j - L[i] + 1]];
                    h[++tot] = H + p[123] * s[j - L[i] + 1] + p[124] * s[j];
                }
                H += p[s[j]];
            }
            sort(h + 1, h + tot + 1);
            for (auto x : id[L[i]]) {
                int p1 = upper_bound(h + 1, h + tot + 1, Hash[x]) - h;
                int p2 = lower_bound(h + 1, h + tot + 1, Hash[x]) - h;
                ans[x] = p1 - p2;
            }
            id[L[i]].clear();
        }
        for (int i = 1; i <= n; i++) printf("%d\n", ans[i]);
    }
    return 0;
}
/*
    相似串哈希
    Problem：
        两个字符串a和b相似当且仅当两个串长度相等
        且a[i]等于a[j]时b[i]等于b[j]
        a[i]不等于a[j]时b[i]也不等于b[j]
        给定一个字符串，多次询问其某个子串在原串中相似匹配的次数
    Solution:
        对于每个子串，我们根据每个字母第一次出现的位置对其标号
        用标号来表示这个子串，那么如果标号序列的哈希值相同则子串相等
        当子串起始位置不同时，我们会得到不同的标号，我们发现标号序列存在后缀递推关系
        因此我们从后往前预处理这个标号，得到每个位置作为子串起点时标号对字母的映射
        维护每个字符的位置哈希，对于两个子串的比较，只要比较子串区间内所有对应标号的字符的位置哈希是否相同即可
        我们对所有后缀按照相似匹配的定义进行排序，那么包含与查询子串相似的串的后缀一定是连续的段
        对于查询一个子串相似匹配的次数，我们只要在后缀排名中二分找到这样的相似段的左右端点即可
        即两个后缀的最长匹配大于等于子串长度的最远位置，求最长匹配的过程可以二分加速
*/
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
const int N = 100000 + 10, C = 10;
const int base = 131;
int n, m, a[N][C], id[N], rk[N];
ull p[N], b[N][C];
char s[N];
int getmax(int x, int y) {
    if (x > y) swap(x, y);
    int r = n - y + 1, l = 0, ans = 0, mid, h = 0;
    while (l <= r) {
        mid = (l + r) / 2;
        h = 0;
        for (int i = 0; h == i && i < C; i++)
            h += (((b[x + mid - 1][a[x][i]] - b[x - 1][a[x][i]]) * p[y - x] -
                   b[y + mid - 1][a[y][i]] + b[y - 1][a[y][i]]) == 0);
        if (h == C)
            l = mid + 1, ans = mid;
        else
            r = mid - 1;
    }
    return ans;
}
bool cmp(int x, int y) {
    int d = getmax(x, y);
    if (d == n - max(x, y) + 1) return x > y;
    int _A = 0, _B = 0;
    for (int i = 0; i < C; i++) {
        if (s[x + d] == a[x][i]) _A = i;
        if (s[y + d] == a[y][i]) _B = i;
    }
    return _A < _B;
}
void init() {
    for (int i = p[0] = 1; i < N; i++) p[i] = p[i - 1] * base;
    for (int i = 1; i <= n; i++) s[i] -= 'a';
    for (int i = 0; i < C; i++) a[n + 1][i] = i;
    for (int i = n; i > 0; i--) {
        int pos = 0;
        for (int j = 0; j < C; j++) {
            a[i][j] = a[i + 1][j];
            if (a[i][j] == s[i]) pos = j;
        }
        while (pos--) swap(a[i][pos], a[i][pos + 1]);
    }
    for (int i = 1; i <= n; i++) {
        b[i][s[i]] = p[i];
        for (int j = 0; j < C; j++) b[i][j] = b[i][j] + b[i - 1][j];
    }
    for (int i = 1; i <= n; i++) id[i] = i;
    stable_sort(id + 1, id + n + 1, cmp);
    for (int i = 1; i <= n; i++) rk[id[i]] = i;
}
int main() {
    scanf("%d%d", &n, &m);
    scanf("%s", s + 1);
    init();
    while (m--) {
        int l, r, ans;
        scanf("%d%d", &l, &r);
        int x = rk[l];
        int d = r - l + 1;
        l = ans = 1;
        r = x;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (getmax(id[x], id[mid]) >= d)
                ans = mid, r = mid - 1;
            else
                l = mid + 1;
        }
        int L = ans;
        l = x;
        r = n;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (getmax(id[x], id[mid]) >= d)
                ans = mid, l = mid + 1;
            else
                r = mid - 1;
        }
        int R = ans;
        printf("%d\n", R - L + 1);
    }
    return 0;
}




