/*
    回文自动机
    本质为回文后缀自动机，可处理回文后缀关系和回文统计
    attention: 使用时加入字符需要做char to int转化
    add(s[i] - 'a') 返回值为当前字符插入尾部形成的最长回文串后缀的id

    N：串长。
    S：字符集大小。
    text[1..all]：字符串。
    son[x][y]：第 x 个点所代表的回文串两边加上字符 y 后的回文串。
    fail[x]：第 x 个点所代表的回文串的最长回文后缀。
    cnt[x]：第 x 个点所代表的回文串的出现次数（需建完树后 count() 一遍才可以）。
    len[x]：第 x 个点所代表的回文串的长度。
    num[x]：第 x 个点所代表的回文串的后缀回文的数量
    l[x]：第 x 个点所代表的回文串的回文后缀的长度和（取模）
    half[x]：第 x 个点所代表的回文串的小于等于串长一半的最长回文后缀
    
    l数组和num数组的应用
    id = add(s[i] - 'a')
    R[i] = num[id] * (i + 1) - l[id]
    可以计算出插入一个字符后以当前位置为右端点的回文串左端点之和

    fail数组的应用
    可以fail[x]向x连边形成fail树，根为0
    一个点的祖先节点为它的回文后缀

*/
const int N = 1000010, S = 26;
int l[N], half[N];
int all, son[N][S], fail[N], len[N], text[N], num[N], cnt[N], lst, tot;
int newnode(int _l) {
    for (int i = 0; i < S; i++) son[tot][i] = 0;
    l[tot] = num[tot] = cnt[tot] = 0, len[tot] = _l;
    return tot++;
}
void init() {
    lst = tot = all = 0;
    newnode(0), newnode(-1);
    text[0] = -1, fail[0] = 1;
}
int getfail(int x) {
    while (text[all - len[x] - 1] != text[all]) x = fail[x];
    return x;
}
void gethalf(int y, int x, int w) {
    half[y] = son[getfail(half[x])][w];
    while (len[half[y]] * 2 > len[y]) half[y] = fail[half[y]];
}
int add(int w) {
    text[++all] = w;
    int x = getfail(lst);
    if (!son[x][w]) {
        int y = newnode(len[x] + 2);
        fail[y] = son[getfail(fail[x])][w];
        son[x][w] = y;
        l[y] = (1ll * l[fail[y]] + len[y]) % P;
        num[y] = num[fail[y]] + 1;
        gethalf(y, x, w);
    }
    lst = son[x][w];
    cnt[lst]++;
    return lst;
}
void count() {
    for (int i = tot - 1; ~i; i--) cnt[fail[i]] += cnt[i];
}
/*
    Example(回文串划分方案数)
    题目大意：给出一个串s，现在要求将其划分子串，并且划分结果呈回文
    求方案数，如abcdabcd划分为ab，cd，cd，ab，为回文。
    题解：我们将串后半部分倒序依次插入前半部分的后面，比如abcdcdab，
    将构成abbacddc，那么问题就转化为新串能拆分成回文子串的方案数，
    我们对新串边构建回文自动机，并在构建的同时计算答案，
    diff数组表示节点与其失配位置最长后缀回文的差值，
    记anc为将连续相同差值去除后的祖先节点，比如abbabbabba，
    在去除连续相同差值abb之后，得到祖先节点a，
    则对于一种差值长度的答案来说，若失配位置为其祖先节点，则答案等于失配节点的答案
    否则其值为拿掉祖先节点加一倍差值之后的位置的答案ans，加上其失配节点的答案g，
    对于不同差值长度的节点答案g累积到当前位置答案ans上去，
    最后输出结尾位置的答案即可。
*/
using namespace std;
const int N = 1000010;
const int mod = 1000000007;
int n, cnt, lst, s[N], l[N], f[N], diff[N], anc[N], g[N], ans[N], a[N][26];
char S[N];
void add(int &x, int y) {
    if ((x += y) >= mod) x -= mod;
}
void init() {
    lst = cnt = 1;
    f[0] = 1, f[1] = 0;
    l[1] = -1;
}
void extend(int np, int c) {
    int p = lst;
    while (s[np] != s[np - l[p] - 1]) p = f[p];
    if (!a[p][c]) {
        int x = ++cnt, fp = f[p];
        while (s[np] != s[np - l[fp] - 1]) fp = f[fp];
        l[x] = l[p] + 2;
        f[x] = a[fp][c];
        a[p][c] = x;
        diff[x] = l[x] - l[f[x]];
        anc[x] = diff[x] == diff[f[x]] ? anc[f[x]] : f[x];
    }
    lst = a[p][c];
}
int main() {
    scanf("%s", S + 1);
    n = strlen(S + 1);
    for (int i = 1; i * 2 <= n; i++) {
        s[i * 2 - 1] = S[i] - 'a';
        s[i * 2] = S[n - i + 1] - 'a';
    }
    s[0] = -1;
    ans[0] = 1;
    init();
    for (int i = 1; i <= n; i++) {
        extend(i, s[i]);
        for (int x = lst; x; x = anc[x]) {
            g[x] = ans[i - l[anc[x]] - diff[x]];
            if (anc[x] != f[x]) add(g[x], g[f[x]]);
            if (i % 2 == 0) add(ans[i], g[x]);
        }
    }
    printf("%d\n", ans[n]);
    return 0;
}