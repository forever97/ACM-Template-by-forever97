/*
    反对称子串
    将一个01串的01取反之后将串反过来跟原串一样
    求一个字符串的反对称子串的数量
*/
int n, m, i, r, p, f[N << 1];
long long ans;
char a[N], s[N << 1];
int min(int a, int b) { return a < b ? a : b; }
bool check(char x, char y) {
    if (x == '#' && y == '#') return 1;
    if ((x - '0') + (y - '0') == 1) return 1;
    return 0;
}
void manacher(char *a) {
    for (i = 1; i <= n; i++) s[i << 1] = a[i], s[i << 1 | 1] = '#';
    s[0] = '$', s[1] = '#', s[m = (n + 1) << 1] = '&';
    for (r = p = 0, f[1] = 1, i = 1; i<m; ans += f[i++]>> 1) {
        for (f[i] = r > i ? min(r - i, f[p * 2 - i]) : 0;
             check(s[i - f[i]], s[i + f[i]]); f[i]++)
            ;
        if (i + f[i] > r) r = i + f[i], p = i;
    }
}
int main() {
    scanf("%d", &n);
    scanf(" %s", a + 1);
    manacher(a);
    printf("%lld\n", ans);
    return 0;
}