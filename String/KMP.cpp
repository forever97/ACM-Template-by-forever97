/*
    KMP
    计算a串在b串中的出现次数
*/
const int N = 1000005;
int T, nxt[N], ans;
char a[N], b[N];
void kmp(int n, char* a, int m, char* b) {
    int i, j;
    for (nxt[0] = j = -1, i = 1; i < n; nxt[i++] = j) {
        while (~j && a[j + 1] != a[i]) j = nxt[j];
        if (a[j + 1] == a[i]) j++;
    }
    for (j = -1, i = 0; i < m; i++) {
        while (~j && a[j + 1] != b[i]) j = nxt[j];
        if (a[j + 1] == b[i]) j++;
        if (j == n - 1) ans++, j = nxt[j];
    }
}
int main() {
    scanf("%d", &T);
    while (T--) {
        ans = 0;
        scanf("%s %s", a, b);
        kmp(strlen(a), a, strlen(b), b);
        printf("%d\n", ans);
    }
    return 0;
}