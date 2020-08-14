/*
    诱导排序SA
    复杂度O(n)
    rk：该位置[0~len-1]的后缀排名
    sa：对应排名在原串[0~len-1]中的位置
    ht：排名相邻的后缀最长公共前缀[2~len]
*/
const int N = 100010;
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
int n;
char s[100010];
// Test
int main() {
    scanf("%s", s);
    n = strlen(s);
    s[n] = 'a' - 1;
    SA::suffixArray(n, s);
    for (int i = 1; i <= n; i++)
        printf(i == n ? "%d\n" : "%d ", SA::sa[i] + 1);  // 输出在原串中的位置
    for (int i = 2; i <= n; i++)
        printf(i == n ? "%d\n" : "%d ",
               SA::ht[i]);  // 排名相邻的后缀的最长公共前缀
    return 0;
}
/*
    Example1
    求最长可允许重叠的出现次数不小于k的子串
*/
int n, s[N];
int k;
bool check(int x) {
    int num = 0;
    for (int i = 1; i <= n; i++) {
        if (SA::ht[i] >= x)
            num++;
        else
            num = 1;
        if (num >= k) return 1;
    }
    return 0;
}
int main() {
    while (~scanf("%d%d", &n, &k)) {
        for (int i = 0; i < n; i++) scanf("%d", &s[i]);
        s[n] = 0;
        SA::suffixArray(n, s);
        int l = 0, r = n, ans = 0;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (check(mid))
                ans = mid, l = mid + 1;
            else
                r = mid - 1;
        }
        printf("%d\n", ans);
    }
    return 0;
}
/*
    Example2
    求最长不允许重叠的出现次数大于1的子串
    示例为差分数组求解(升降调视为相同旋律)
*/
vector<int> v[N];
bool check(int x) {
    int cnt = -1;
    for (int i = 1; i <= n; i++) {
        if (SA::ht[i] < x) v[++cnt].clear();
        v[cnt].push_back(i);
    }
    for (int i = 0; i <= cnt; i++) {
        int L = N, R = -1;
        if (v[i].size() > 1) {
            for (int j = 0; j < v[i].size(); j++) {
                R = max(R, SA::sa[v[i][j]]);
                L = min(L, SA::sa[v[i][j]]);
            }
            if (R - L >= x) return 1;
        }
    }
    return 0;
}
int main() {
    while (scanf("%d", &n), n) {
        for (int i = 0; i < n; i++) scanf("%d", &s[i]);
        for (int i = 0; i < n - 1; i++) s[i] = s[i + 1] - s[i] + 90;
        s[--n] = 0;
        SA::suffixArray(n, s);
        int l = 0, r = n, ans = 0;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (check(mid))
                ans = mid, l = mid + 1;
            else
                r = mid - 1;
        }
        printf("%d\n", ans >= 4 ? ans + 1 : 0);
    }
    return 0;
}
/*
    Example3
    求至少在k个字符串中出现的最长公共子串
*/
int first = 0, len[N], u, K;
vector<int> S[N];
bool vis[N];
bool check(int L) {
    int cur = -1;
    for (int i = 1; i <= u; i++) {
        if (h[i] < L) S[++cur].clear();
        S[cur].push_back(i);
    }
    for (int i = 0; i <= cur; i++) {
        if (S[i].size() > K) {
            memset(vis, 0, sizeof(vis));
            for (int j = 0; j < S[i].size(); j++) {
                int k = S[i][j];
                int x = upper_bound(a, a + n + 1, sa[S[i][j]]) - a - 1;
                vis[x] = 1;
            }
            int count = 0;
            for (int j = 0; j < n; j++)
                if (vis[j]) count++;
            if (count > K) return 1;
        }
    }
    return 0;
}
void Print(int L) {
    int cur = -1;
    for (int i = 1; i <= u; i++) {
        if (SA::ht[i] < L) S[++cur].clear();
        S[cur].push_back(i);
    }
    for (int i = 0; i <= cur; i++) {
        if (S[i].size() > K) {
            memset(vis, 0, sizeof(vis));
            for (int j = 0; j < S[i].size(); j++) {
                int k = S[i][j];
                int x = upper_bound(a, a + n + 1, SA::sa[S[i][j]]) - a - 1;
                vis[x] = true;
            }
            int count = 0;
            for (int j = 0; j < n; j++)
                if (vis[j]) count++;
            if (count > K) {
                for (int j = 0; j < L; j++)
                    printf("%c", char(s[SA::sa[S[i][0]] + j]));
                puts("");
            }
        }
    }
}
int main() {
    while (~scanf("%d", &n), n) {
        if (first++) puts("");
        int tmp = 200;
        u = 0;
        for (int i = 0; i < n; i++) {
            scanf("%s", str);
            len[i] = strlen(str);
            for (int j = 0; j < len[i]; j++) s[u++] = (int)str[j];
            s[u++] = tmp++;
        }
        tmp = 0;
        s[u] = 0;
        for (int i = 0; i <= n; i++) {
            a[i] = tmp;
            if (i < n) tmp = tmp + (i == 0 ? len[i] : len[i] + 1);
        }
        SA::suffixArray(u, s);
        int l = 1, r = 1000, ans = 0;
        K = n / 2;  // 求至少在一半字符串中出现的最长公共子串
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (check(mid))
                ans = mid, l = mid + 1;
            else
                r = mid - 1;
        }
        if (ans == 0)
            puts("?");
        else
            Print(ans);
    }
    return 0;
}
/*
    Example4
    求在每个字符串中出现至少两次的最长的子串
*/
int first = 0, len[N], u, K;
vector<int> S[N];
int Min[15], Max[15];
bool check(int L) {
    int cur = -1;
    for (int i = 1; i <= u; i++) {
        if (SA::ht[i] < L) S[++cur].clear();
        S[cur].push_back(i);
    }
    for (int i = 0; i <= cur; i++) {
        if (S[i].size() >= 2 * n) {
            memset(Min, -1, sizeof(Min));
            memset(Max, -1, sizeof(Max));
            for (int j = 0; j < S[i].size(); j++) {
                int k = S[i][j];
                int x = upper_bound(a, a + n + 1, SA::sa[k]) - a - 1;
                Min[x] = Min[x] == -1 ? sa[k] : min(Min[x], SA::sa[k]);
                Max[x] = Max[x] == -1 ? sa[k] : max(Max[x], SA::sa[k]);
            }
            bool flag = 1;
            for (int i = 0; i < n; i++) {
                if (Min[i] == -1 || Max[i] - Min[i] < L) {
                    flag = 0;
                    break;
                }
            }
            if (flag) return 1;
        }
    }
    return 0;
}
int T;
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        int tmp = 200;
        u = 0;
        for (int i = 0; i < n; i++) {
            scanf("%s", str);
            len[i] = strlen(str);
            for (int j = 0; j < len[i]; j++) s[u++] = (int)str[j];
            s[u++] = tmp++;
        }
        tmp = 0;
        s[u] = 0;
        for (int i = 0; i <= n; i++) {
            a[i] = tmp;
            if (i < n) tmp = tmp + (i == 0 ? len[i] : len[i] + 1);
        }
        SA::suffixArray(u, s);
        int l = 1, r = 10000, ans = 0;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (check(mid))
                ans = mid, l = mid + 1;
            else
                r = mid - 1;
        }
        printf("%d\n", ans);
    }
    return 0;
}
