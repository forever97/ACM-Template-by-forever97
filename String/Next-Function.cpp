/*
    nxt函数的应用
*/
int nxt[N];
void get_nxt(int n, char* a) {
    int i, j;
    for (nxt[0] = j = -1, i = 1; i < n; nxt[i++] = j) {
        while (~j && a[j + 1] != a[i]) j = nxt[j];
        if (a[j + 1] == a[i]) j++;
    }
}
/*
    求补上最少字母数量使得这是个循环串
*/
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%s", s);
        get_nxt(len = strlen(s), s);
        int L = len - (nxt[len - 1] + 1);
        if (L < len && len % L == 0)
            puts("0");
        else
            printf("%d\n", L - len % L);
    }
    return 0;
}
/*
    求循环节数量
*/
int main() {
    while (~scanf(" %s", &s)) {
        if (s[0] == '.') break;
        int n = strlen(s);
        get_nxt(n, s);
        printf("%d\n", n % (n - nxt[n - 1] - 1) ? 1 : n / (n - nxt[n - 1] - 1));
    }
    return 0;
}
/*
    求同时是前缀和后缀的串长
*/
int main() {
    while (~scanf(" %s", &s)) {
        int cnt = 0;
        get_nxt(s, nxt);
        for (t = nxt[(n = strlen(s)) - 1]; t != -1; t = nxt[t]) {
            if (s[t] == s[n - 1]) ans[cnt++] = t + 1;
        }
        for (int i = cnt - 1; i >= 0; i--) printf("%d ", ans[i]);
        printf("%d\n", n);
    }
    return 0;
}
/*
    求出每个循环节的数量和终点位置
*/
int main() {
    while (~scanf("%d", &n) && n) {
        printf("Test case #%d\n", ++cas);
        scanf("%s", s);
        get_nxt(strlen(s), s);
        for (int i = 2; i <= n; i++) {
            if (nxt[i - 1] != -1 && (i % (i - nxt[i - 1] - 1) == 0))
                printf("%d %d\n", i, i / (i - nxt[i - 1] - 1));
        }
        puts("");
    }
    return 0;
}
/*
    求第一个串的前缀和第二个串的后缀的最大匹配
*/
int main() {
    while (~scanf("%s", s)) {
        int L1 = strlen(s);
        scanf("%s", s + L1);
        int L = strlen(s), L2 = L - L1;
        get_nxt(L, s);
        for (k = nxt[L - 1]; k >= L1 || k >= L2; k = nxt[k])
            ;
        if (k == -1)
            puts("0");
        else {
            for (int i = 0; i <= k; i++) printf("%c", s[i]);
            printf(" %d\n", k + 1);
        }
    }
    return 0;
}
/*
    求字符串每个前缀和串匹配成功的次数和
*/
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%s", &len, s);
        get_nxt(len, s);
        ans = 0;
        for (int i = 0; i < len; i++) {
            if (nxt[i] < 0)
                dp[i] = 1;
            else
                dp[i] = (dp[nxt[i]] + 1) % mod;
            ans = (ans + dp[i]) % mod;
        }
        printf("%d\n", ans);
    }
    return 0;
}