/*
    题目大意：查询最长的是x串和y串的公共后缀，且至少是一个给定串集中串的前缀
    求给定串集中有多少串满足前缀是他们的公共后缀
    题解：对所有串建立AC自动机，那么若前缀i是前缀j的后缀，说明i是Fail树上j的祖先。
    所以对于询问(x,y)，答案就是两点在Fail树上的LCA在原Trie中子树内的字符串总数。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 500010;
namespace AC_DFA {
const int Csize = 27;
int tot, son[N][Csize], val[N], fail[N], q[N];
void Initialize() {
    memset(fail, 0, sizeof(int) * (tot + 1));
    memset(val, 0, sizeof(int) * (tot + 1));
    for (int i = 0; i <= tot; i++)
        for (int j = 0; j < Csize; j++) son[i][j] = 0;
    tot = 0;
    fail[0] = -1;
}
inline int Tr(char ch) { return ch - 'a'; }
int Insert(char *s) {
    int x = 0;
    for (int l = strlen(s), i = 0, w; i < l; i++) {
        if (!son[x][w = Tr(s[i])]) son[x][w] = ++tot;
        x = son[x][w];
        val[x]++;
    }
    return x;
}
void MakeFail() {
    int h = 1, t = 0, i, x = 0;
    for (i = 0; i < Csize; i++)
        if (son[0][i]) q[++t] = son[0][i];
    while (h <= t)
        for (x = q[h++], i = 0; i < Csize; i++)
            if (son[x][i])
                fail[son[x][i]] = son[fail[x]][i], q[++t] = son[x][i];
            else
                son[x][i] = son[fail[x]][i];
}
}  // namespace AC_DFA
namespace Tree_Chain_Subdivision {
int ed, root, d[N], num[N], v[N << 1], vis[N], f[N], g[N << 1];
int nxt[N << 1], size[N], son[N], st[N], en[N], dfn, top[N];
void add_edge(int x, int y) {
    v[++ed] = y;
    nxt[ed] = g[x];
    g[x] = ed;
}
void dfs(int x) {
    size[x] = 1;
    for (int i = g[x]; i; i = nxt[i])
        if (v[i] != f[x]) {
            f[v[i]] = x, d[v[i]] = d[x] + 1;
            dfs(v[i]), size[x] += size[v[i]];
            if (size[v[i]] > size[son[x]]) son[x] = v[i];
        }
}
void dfs2(int x, int y) {
    if (x == -1) return;
    st[x] = ++dfn;
    top[x] = y;
    if (son[x]) dfs2(son[x], y);
    for (int i = g[x]; i; i = nxt[i])
        if (v[i] != son[x] && v[i] != f[x]) dfs2(v[i], v[i]);
    en[x] = dfn;
}
int lca(int x, int y) {
    for (; top[x] != top[y]; x = f[top[x]])
        if (d[top[x]] < d[top[y]]) {
            int z = x;
            x = y;
            y = z;
        }
    return d[x] < d[y] ? x : y;
}
void Initialize() {
    memset(g, dfn = ed = 0, sizeof(g));
    memset(v, 0, sizeof(v));
    memset(nxt, 0, sizeof(nxt));
    memset(son, -1, sizeof(son));
}
}  // namespace Tree_Chain_Subdivision
int n, m, pos[N];
char s[N];
int ask(int x, int y) {
    int lca = Tree_Chain_Subdivision::lca(pos[x], pos[y]);
    return AC_DFA::val[lca];
}
int main() {
    while (~scanf("%d", &n)) {
        using namespace AC_DFA;
        Initialize();
        for (int i = 1; i <= n; i++) {
            scanf("%s", s);
            pos[i] = Insert(s);
        }
        MakeFail();
        Tree_Chain_Subdivision::Initialize();
        for (int i = 1; i <= tot; i++)
            Tree_Chain_Subdivision::add_edge(fail[i], i);
        Tree_Chain_Subdivision::dfs(0);
        Tree_Chain_Subdivision::dfs2(0, 0);
        scanf("%d", &m);
        while (m--) {
            int x, y;
            scanf("%d%d", &x, &y);
            int ans = ask(x, y);
            if (!ans)
                puts("N");
            else
                printf("%d\n", ans);
        }
    }
    return 0;
}
/*
    题目大意：给出一个打印的过程，'a'-'z'表示输入字母，P表示打印该字符串
    B表示删去一个字符。问第x个打印的字符串在第y个打印的字符串中出现的次数
    题解：我们根据打印的过程建立trie树，
    当x是y的子串当且仅当y到根的链上有fail指针指向x的结尾，
    而x在y中的出现次数则取决于有几个这样的指针，
    我们根据fail指针建立fail树，按照fail树的dfs序进行统计，
    在每个y处记录其要查询的x，在y点用树状数组对x点求dfs序区间和即可。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
const int N = 100010;
int dfn, l[N], r[N], ans[N];
vector<int> v[N], Q[N], ID[N];
namespace BIT {
int c[N << 1];  // dfs
void Initialize() { memset(c, 0, sizeof(c)); }
void add(int x, int v) {
    while (x <= dfn) c[x] += v, x += x & -x;
}
int query(int x) {
    int res = 0;
    while (x) res += c[x], x -= x & -x;
    return res;
}
}  // namespace BIT
namespace AC_DFA {
const int Csize = 26;
int id, tot, son[N][Csize], sum[N], f[N], fail[N], q[N], pos[N], match[N];
void Initialize() {
    memset(sum, 0, sizeof(int) * (tot + 1));
    memset(fail, 0, sizeof(int) * (tot + 1));
    for (int i = 0; i <= tot; i++)
        for (int j = 0; j < Csize; j++) son[i][j] = 0;
    tot = 0;
    id = 0;
    fail[0] = -1;
}
inline int Tr(char ch) { return ch - 'a'; }
void Build(char *s) {
    int x = 0;
    for (int l = strlen(s), i = 0, w; i < l; i++) {
        if (s[i] == 'P')
            pos[++id] = x;
        else if (s[i] == 'B')
            x = f[x];
        else {
            if (!son[x][w = Tr(s[i])]) {
                son[x][w] = ++tot;
                f[tot] = x;
            }
            x = son[x][w];
        }
    }
}
void MakeFail() {
    int h = 1, t = 0, i, j, x = 0;
    for (i = 0; i < Csize; i++)
        if (son[0][i]) q[++t] = son[0][i];
    while (h <= t)
        for (x = q[h++], i = 0; i < Csize; i++)
            if (son[x][i]) {
                fail[son[x][i]] = son[fail[x]][i], q[++t] = son[x][i];
            } else
                son[x][i] = son[fail[x]][i];
}
void Solve(char *s) {
    using namespace BIT;
    BIT::Initialize();
    int x = 0, id = 0;
    add(l[0], 1);
    for (int L = strlen(s), i = 0; i < L; i++) {
        if (s[i] == 'P') {
            id++;
            for (int k = 0; k < Q[id].size(); k++) {
                int u = pos[Q[id][k]];
                ans[ID[id][k]] = query(r[u]) - query(l[u] - 1);
            }
        } else if (s[i] == 'B')
            add(l[x], -1), x = f[x];
        else
            x = son[x][Tr(s[i])], add(l[x], 1);
    }
}
}  // namespace AC_DFA
void Dfs(int x) {
    l[x] = ++dfn;
    for (int i = 0; i < v[x].size(); i++) Dfs(v[x][i]);
    r[x] = ++dfn;
}
char s[N];
int main() {
    using namespace AC_DFA;
    Initialize();
    scanf("%s", s);
    Build(s);
    MakeFail();
    for (int i = 1; i <= tot; i++) v[fail[i]].push_back(i);
    int m, x, y;
    scanf("%d", &m);
    for (int i = 1; i <= m; i++) {
        scanf("%d%d", &x, &y);
        Q[y].push_back(x);
        ID[y].push_back(i);
    }
    Dfs(0);
    Solve(s);
    for (int i = 1; i <= m; i++) printf("%d\n", ans[i]);
    return 0;
}
