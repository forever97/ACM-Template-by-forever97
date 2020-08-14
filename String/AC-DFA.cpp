/*
    AC自动机
*/
namespace AC_DFA {
const int Csize = 27;  // 预留一位'z'+1作为分隔符
int tot, son[N][Csize], sum[N], fail[N], q[N], ans[N], dph[N], match[N];
void Initialize() {
    memset(sum, 0, sizeof(int) * (tot + 1));
    memset(dph, 0, sizeof(int) * (tot + 1));
    memset(ans, 0, sizeof(int) * (tot + 1));
    memset(match, 0, sizeof(int) * (tot + 1));
    memset(fail, 0, sizeof(int) * (tot + 1));
    for (int i = 0; i <= tot; i++)
        for (int j = 0; j < Csize; j++) son[i][j] = 0;
    tot = 0;
    fail[0] = -1;
}
inline int Tr(char ch) { return ch - 'a'; }
int Insert(char *s) {
    int x = 0;
    for (int l = strlen(s), i = 0, w; i < l; i++) {
        if (!son[x][w = Tr(s[i])]) {
            son[x][w] = ++tot;
            dph[tot] = i + 1;
        }
        x = son[x][w];
    }
    sum[x]++;
    return x;
}
void MakeFail() {
    int h = 1, t = 0, i, j, x = 0;
    // 注意单个字符的处理
    for (i = 0; i < Csize; i++)
        if (son[0][i]) {
            q[++t] = son[0][i];
            match[son[0][i]] =
                sum[son[0][i]] ? son[0][i] : match[fail[son[0][i]]];
        }
    while (h <= t)
        for (x = q[h++], i = 0; i < Csize; i++)
            if (son[x][i]) {
                fail[son[x][i]] = son[fail[x]][i], q[++t] = son[x][i];
                match[son[x][i]] =
                    sum[son[x][i]] ? son[x][i] : match[fail[son[x][i]]];
            } else
                son[x][i] = son[fail[x]][i];
}
/*
    查询有多少串在模式串中出现
    破坏性查询
    sum置0防止被重复统计
*/
int Query(char *s) {
    int res = 0;
    for (int l = strlen(s), x = 0, i = 0; i < l; i++) {
        x = son[x][Tr(s[i])];
        int t = x;
        while (t) {
            res += sum[t];
            sum[t] = 0;
            t = fail[t];
        }
    }
    return res;
}
/*
    在AC_DFA上统计每个串在多少个模式串中出现
    注意最后答案需要累加到fail链的前继
*/
void Search(char *s, int len) {
    for (int l = strlen(s), i = 0, x = 0, w; i < l; i++) {
        x = son[x][Tr(s[i])];
        while (dph[x] > len) x = fail[x];
        ans[match[x]]++;
    }
}
int d[N], st[N];
void Solve() {
    int k = 0;
    memset(d, 0, sizeof(int) * (tot + 1));
    for (int i = 1; i <= tot; i++) d[fail[i]]++;
    for (int i = 1; i <= tot; i++)
        if (!d[i]) st[k++] = i;
    for (int i = 0; i < k; i++) {
        int j = fail[st[i]];
        ans[j] += ans[st[i]];
        if (!--d[j]) st[k++] = j;
    }
}
for (int i = 0; i < q; i++) pos[i] = Insert(t[i]);  // 插入询问串
MakeFail();
for (int i = 0; i < n; i++) Search(s[i], len[i]);  // 处理模式串
Solve();
for (int i = 0; i < q; i++) printf("%d\n", ans[pos[i]]);
/*
    给出一些串，询问第x个串和第y个串的公共子串，
    同时要求该公共子串为某个串的前缀。求最长符合要求的答案
    我们对所有串构建AC自动机，将两个询问串之一在AC自动机上mark所有的匹配位置
    另一个串在mark的地方寻找最长匹配即可
*/
int Ans = 0;
void Cal(char *s) {
    memset(vis, 0, sizeof(vis));
    for (int l = strlen(s), i = 0, x = 0, w; i < l; i++) {
        while (!son[x][Tr(s[i])]) x = fail[x];
        x = son[x][Tr(s[i])];
        for (int j = x; j; j = fail[j]) vis[j] = 1;
    }
}
void Find(char *s) {
    for (int l = strlen(s), i = 0, x = 0, w; i < l; i++) {
        while (!son[x][Tr(s[i])]) x = fail[x];
        x = son[x][Tr(s[i])];
        for (int j = x; j; j = fail[j])
            if (vis[j]) Ans = max(Ans, dph[j]);
    }
}
void Solve() {
    int x, y;
    Ans = 0;
    scanf("%d%d", &x, &y);
    Cal(s[x]);
    Find(s[y]);
    printf("%d\n", Ans);
}
}  // namespace AC_DFA