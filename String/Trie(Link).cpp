/*
    Trie树
    链表版本，节省内存
    选出一些字符串，使得字符串的最长公共前缀*字符串的总个数最大化
*/
const int S = 5000100;
struct Trie {
    int tot, cnt[S], ans, head[S], nxt[S], to[S], m;
    char c[S];
    void insert() {
        char C = getchar();
        for (int x = 0, i = 0; C != '\n'; i++, C = getchar()) {
            int y = -1;
            for (int e = head[x]; e; e = nxt[e])
                if (c[e] == C) {
                    y = to[e];
                    break;
                }
            if (y < 0)
                to[++m] = ++tot, c[m] = C, nxt[m] = head[x], head[x] = m,
                y = tot;
            cnt[x = y]++;
            ans = max(ans, cnt[x] * (i + 1));
        }
    }
} T;
int n;
int main() {
    scanf("%d", &n);
    getchar();
    for (int i = 1; i <= n; i++) T.insert();
    printf("%d\n", T.ans);
    return 0;
}