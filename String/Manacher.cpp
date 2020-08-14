/*
    Manacher算法
    r数组为奇回文半径
*/
int n, f[N << 1], r[N << 1];
char s[N], c[N << 1];
void manacher() {
    for (int i = 1; i <= n; i++) c[i << 1] = s[i], c[(i << 1) + 1] = '#';
    c[1] = '#';
    c[n << 1 | 1] = '#';
    c[0] = '&';
    c[(n + 1) << 1] = '$';
    int j = 0, k;
    n = n << 1 | 1;
    for (int i = 1; i <= n;) {
        while (c[i - j - 1] == c[i + j + 1]) j++;
        r[i] = j;
        for (k = 1; k <= j && r[i] - k != r[i - k]; k++)
            r[i + k] = min(r[i - k], r[i] - k);
        i += k;
        j = max(j - k, 0);
    }
}
/*
   求最长双倍回文
   （记Wr为W串的倒置，求最长的形如WWrWWr的串的长度）
*/
int ans;
int sf(int x) { return f[x] == x ? x : f[x] = sf(f[x]); }
int main() {
    while (~scanf("%d", &n)) {
        scanf("%s", s + 1);
        manacher();
        ans = 0;
        for (int i = 1; i <= n; i++) f[i] = (c[i] == '#') ? i : (i + 1);
        for (int i = 3; i < n; i += 2) {
            int j = sf(max(i - (r[i] >> 1), 1));
            for (; j < i && j + r[j] < i; f[j] = sf(j + 1), j = f[j])
                ;
            if (j < i)
                if ((i - j) << 1 > ans) ans = (i - j) << 1;
        }
        printf("%d\n", ans);
    }
    return 0;
}
/*
    Problem:
        求每一种旋转串的最长回文子串
    Solution:
        倍长字符串，转化为区间回文子串，manacher预处理倍长后的回文串
        对于查询区间[L,R]最长回文子串
        我们可以二分答案，检验区间[L+x-1,R-x+1]中是否存在长度大于x的回文中心即可
        ST表预处理r数组的区间最大值，复杂度O(nlogn)
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 1000000 + 10;
int n, f[N << 1], r[N << 1];
char s[N], c[N << 1];
int dp[N << 1][21], lg2[N << 1];
void Init(int n) {
    for (int i = 2; i <= n; i++) lg2[i] = lg2[i / 2] + 1;
    for (int i = 1; i <= n; i++) dp[i][0] = r[i];
    for (int j = 1; (1 << j) <= n; j++)
        for (int i = 1; i + (1 << j) - 1 <= n; i++)
            dp[i][j] = max(dp[i][j - 1], dp[i + (1 << (j - 1))][j - 1]);
}
int Max(int l, int r) {
    if (l > r) swap(l, r);
    int k = lg2[r - l + 1];
    return max(dp[l][k], dp[r - (1 << k) + 1][k]);
}
void manacher() {
    for (int i = 1; i <= n; i++) c[i << 1] = s[i], c[(i << 1) + 1] = '#';
    c[1] = '#';
    c[n << 1 | 1] = '#';
    c[0] = '&';
    c[(n + 1) << 1] = '$';
    int j = 0, k;
    n = n << 1 | 1;
    for (int i = 1; i <= n;) {
        while (c[i - j - 1] == c[i + j + 1]) j++;
        r[i] = j;
        for (k = 1; k <= j && r[i] - k != r[i - k]; k++)
            r[i + k] = min(r[i - k], r[i] - k);
        i += k;
        j = max(j - k, 0);
    }
}
int main() {
    scanf("%d%s", &n, s + 1);
    int m = n;
    n = strlen(s + 1);
    for (int i = n + 1; i <= 2 * n; i++) s[i] = s[i - n];
    n = n * 2;
    manacher();
    Init(n);
    // 区间[i,i+n-1]的最长回文子串
    for (int i = 1; i <= m; i++) {
        int l = 1, r = m, ans = 1;
        int L = 2 * i, R = 2 * (i + m - 1); 
        while (l <= r) {
            int mid = l + r >> 1;
            if (Max(L + mid - 1, R - mid + 1) >= mid)
                ans = mid, l = mid + 1;
            else
                r = mid - 1;
        }
        printf("%d\n", ans);
    }
    return 0;
}
