/*
    求k个串的最长公共子串
    并输出字典序最小的一个
*/
const int N = 4050, M = 210;
using namespace std;
int nxt[M], n;
char dict[N][M];
void get_nxt(char *a, int n) {
    int i, j;
    for (nxt[0] = j = -1, i = 1; i < n; nxt[i++] = j) {
        while (~j && a[j + 1] != a[i]) j = nxt[j];
        if (a[j + 1] == a[i]) j++;
    }
}
int LongestPre(char *s, int len) {
    get_nxt(s, len);
    for (int i = 1; i < n; i++) {
        char *p = dict[i];
        int ans = 0;
        for (int j = -1; *p; p++) {
            while (~j && s[j + 1] != *p) j = nxt[j];
            if (s[j + 1] == *p) {
                j++;
                ans = max(ans, j + 1);
            }
            if (j == len - 1) j = nxt[j];
        }
        len = min(len, ans);
    }
    return len;
}
int main() {
    while (scanf("%d", &n) && n) {
        getchar();
        for (int i = 0; i < n; i++) gets(dict[i]);
        int len = strlen(dict[0]), ans = 0, pos = 0;
        for (int i = 0; i < len; i++) {
            int tmp = LongestPre(dict[0] + i, len - i);
            if (tmp >= ans) {
                if (tmp > ans)
                    ans = tmp, pos = i;
                else {
                    bool flag = 1;
                    for (int t = 0; t < ans; t++) {
                        if (dict[0][pos + t] > dict[0][i + t])
                            break;
                        else if (dict[0][pos + t] < dict[0][i + t]) {
                            flag = 0;
                            break;
                        }
                    }
                    if (flag) pos = i;
                }
            }
        }
        if (ans) {
            for (int i = 0; i < ans; i++) putchar(dict[0][pos + i]);
            puts("");
        } else
            puts("IDENTITY LOST");
    }
    return 0;
}