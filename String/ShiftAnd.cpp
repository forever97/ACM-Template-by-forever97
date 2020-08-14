/*
    ShiftAnd
    柔性字符串匹配
    给出一个字符串，找出其中所有的符合特定模式的子串位置，
    符合特定模式是指，该子串的长度为n，并且第i个字符需要在给定的字符集合Si中
*/
const int M = 510, N = 2000010, L = 65, U = 256;
bitset<M> dp[2], bt[U];
int n, m, id[U], cnt, l;
char s[N], t[L];
void init() {
    cnt = 0;
    for (int i = '0'; i <= '9'; i++) id[i] = ++cnt;
    for (int i = 'A'; i <= 'Z'; i++) id[i] = ++cnt;
    for (int i = 'a'; i <= 'z'; i++) id[i] = ++cnt;
}
void ShiftAnd(int n, int m) {
    int cur = 1, f = 0;
    dp[0].reset();
    dp[0].set(0);
    for (int i = 1; i <= n; i++, cur ^= 1) {
        dp[cur] = dp[cur ^ 1] << 1 & bt[id[s[i]]];
        dp[cur].set(0);
        if (dp[cur][m]) printf("%d\n", i - m + (f = 1));
    }
    if (!f) puts("NULL");
}
int main() {
    init();
    while (gets(s + 1)) {
        n = strlen(s + 1);
        scanf("%d", &m);
        for (int i = 1; i <= cnt; i++) bt[i].reset();
        for (int i = 1; i <= m; i++) {
            scanf("%d %s", &l, t);
            for (int j = 0; j < l; j++) bt[id[t[j]]].set(i);
        }
        ShiftAnd(n, m);
        gets(s);
    }
    return 0;
}