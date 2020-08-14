/*
    Problem:
        给定一棵根为1的树，对于每个节点，求子树中，哪个距离下的节点数量最多，
        当数量相同时，取较小的那个距离值
    Solution:
        我们对树进行长链剖分，记录sum[x]数组表示节点x不同距离的点的数量，
        同一条长链中sum[fx][i+1]=sum[x][i]，直接继承长链，暴力轻儿子即可
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 1000010;
vector<int> G[N];
int n, x, y, len[N], son[N], ans[N], pos[N], dfn;
void dfs(int x, int fx) {
    len[x] = 0;
    for (int y : G[x]) {
        if (y == fx) continue;
        dfs(y, x);
        if (len[y] > len[son[x]]) {
            len[x] = len[y] + 1;
            son[x] = y;
        }
    }
}
int poor[N], *p = poor, *sum[N];
void dfs2(int x, int fx) {
    sum[x][0] = 1;
    if (son[x]) {
        sum[son[x]] = sum[x] + 1;
        dfs2(son[x], x);
        ans[x] = ans[son[x]] + 1;
    }
    for (int y : G[x]) {
        if (y == fx || y == son[x]) continue;
        sum[y] = p;
        p += len[y] + 1;
        dfs2(y, x);
        for (int i = 0; i <= len[y]; i++) {
            sum[x][i + 1] += sum[y][i];
            if ((sum[x][i + 1] > sum[x][ans[x]]) ||
                (sum[x][i + 1] == sum[x][ans[x]] && i + 1 < ans[x]))
                ans[x] = i + 1;
        }
    }
    if (sum[x][ans[x]] == 1) ans[x] = 0;
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i < n; i++) {
        scanf("%d%d", &x, &y);
        G[x].push_back(y);
        G[y].push_back(x);
    }
    len[0] = -1;
    dfs(1, 0);
    sum[1] = p;
    p += len[1] + 1;
    dfs2(1, 0);
    for (int i = 1; i <= n; i++) printf("%d\n", ans[i]);
    return 0;
}
/*
    Problem:
        求一棵树上三点距离两两相等的三元组对数
        树的边权均为1
    Solution:
        设f[i][j]表示i子树距离i为j的点数量
        设g[i][j]表示i子树两点lca距离彼此为d，且该lca距离i点为d-j的点对数
        当y子树并入父节点x时有
        g[x][j+1]+=f[x][j+1]*f[y][j]
        g[x][j-1]+=g[y][j]
        f[x][j+1]+=f[y][j]
        ans=f[x][j]*g[y][j+1]+g[x][j]*f[y][j-1]
        我们发现状态转移只跟节点深度有关，因此可以长链剖分优化
        同一条长链上对于f数组有f[fx][i+1]=f[x][i]，
        对于g数组有g[fx][i-1]=g[x][i]，
        因此g数组前后均要预留len[x]+1，需要开两倍空间，
        继承重儿子的g和f函数，暴力统计轻链，同时计算ans即可
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N = 100010;
vector<int> G[N];
int n, x, y, len[N], son[N];
void dfs(int x, int fx) {
    len[x] = 0;
    for (int i = 0; i < G[x].size(); i++) {
        int y = G[x][i];
        if (y == fx) continue;
        dfs(y, x);
        if (len[y] > len[son[x]]) {
            len[x] = len[y] + 1;
            son[x] = y;
        }
    }
}
ll poorf[N], poorg[N << 1], *pf = poorf, *pg = poorg, *f[N], *g[N], ans = 0;
void dfs2(int x, int fx) {
    f[x][0] = 1;
    if (son[x]) {
        f[son[x]] = f[x] + 1;
        g[son[x]] = g[x] - 1;
        dfs2(son[x], x);
    }
    ans += g[x][0];
    for (int i = 0; i < G[x].size(); i++) {
        int y = G[x][i];
        if (y == fx || y == son[x]) continue;
        f[y] = pf, pf += len[y] + 1;
        pg += len[y] + 1, g[y] = pg, pg += len[y] + 1;
        dfs2(y, x);
        for (int j = 0; j <= len[y]; j++) {
            if (j) ans += f[x][j - 1] * g[y][j];
            ans += g[x][j + 1] * f[y][j];
        }
        for (int j = 0; j <= len[y]; j++) {
            g[x][j + 1] += f[x][j + 1] * f[y][j];
            if (j) g[x][j - 1] += g[y][j];
            f[x][j + 1] += f[y][j];
        }
    }
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i < n; i++) {
        scanf("%d%d", &x, &y);
        G[x].push_back(y);
        G[y].push_back(x);
    }
    len[0] = -1;
    dfs(1, 0);
    f[1] = pf, pf += len[1] + 1;
    pg += len[1] + 1, g[1] = pg, pg += len[1] + 1;
    dfs2(1, 0);
    printf("%lld\n", ans);
    return 0;
}
/*
    Problem:
        给出一棵树，每个点上有一个点权，点权<=30000
        现在求有多少条链，链上点权的gcd>1
    Solution:
        考虑枚举i的倍数的点权，在i的倍数的点生成的诱导子图上求等于D的链长，
        因为点均为i的倍数，因此不需要考虑gcd的问题，
        枚举的i值包含倍数关系，需要容斥，有效的i值为miu值不为0的值，
        i对应的诱导子图计算出的长度等于D的链长对答案的贡献为-miu*sum
        诱导子图求链长为D的链数量可以用长链剖分来求，短链并入长链边合并边计算答案，
        则复杂度为每个诱导子图的点数和，考虑数字a[i]，对诱导子图的点数贡献为2^(loglog(a[i]))
        因此复杂度为O(n*2^(log(log(n))))
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 500010;
const int M = 30010;
typedef long long ll;
vector<int> E[M], P[M];
int v[N], cv;
int g[N], to[N << 1], nxt[N << 1], ed;
int m, mrk, n, D, a[N], x[N], y[N], len[N], son[N], vis[N];
ll ANS, Ans;
inline void add(int x, int y) {
    to[++ed] = y;
    nxt[ed] = g[x];
    g[x] = ed;
}
void dfs(int x, int fx) {
    len[x] = 0;
    for (int o = g[x]; o; o = nxt[o]) {
        int y = to[o];
        if (y == fx) continue;
        dfs(y, x);
        if (len[y] > len[son[x]]) {
            len[x] = len[y] + 1;
            son[x] = y;
        }
    }
}
int poor[N], *p = poor, *sum[N];
void dfs2(int x, int fx) {
    vis[x] = mrk;
    sum[x][0] = 1;
    if (son[x]) {
        sum[son[x]] = sum[x] + 1;
        dfs2(son[x], x);
    }
    for (int o = g[x]; o; o = nxt[o]) {
        int y = to[o];
        if (y == fx || y == son[x]) continue;
        sum[y] = p;
        p += len[y] + 1;
        dfs2(y, x);
        for (int i = 0; i <= len[y] && i < D - 1; i++)
            if (len[x] >= D - i - 1) Ans += 1ll * sum[y][i] * sum[x][D - i - 1];
        for (int i = 0; i <= len[y]; i++) sum[x][i + 1] += sum[y][i];
    }
    if (len[x] >= D) Ans += sum[x][D];
}
bool notp[M];
int prime[M], pnum, mu[M];
void sieve() {
    notp[0] = notp[1] = mu[1] = 1;
    for (int i = 2; i < M; i++) {
        if (!notp[i]) {
            prime[++pnum] = i;
            mu[i] = -1;
        }
        for (int j = 1; j <= pnum && prime[j] * i < M; j++) {
            notp[prime[j] * i] = 1;
            if (i % prime[j] == 0) {
                mu[prime[j] * i] = 0;
                break;
            }
            mu[prime[j] * i] = -mu[i];
        }
    }
}
inline char nc() {
    static char buf[100000], *p1 = buf, *p2 = buf;
    if (p1 == p2) {
        p2 = (p1 = buf) + fread(buf, 1, 100000, stdin);
        if (p1 == p2) return EOF;
    }
    return *p1++;
}
inline void read(int &x) {
    char c = nc(), b = 1;
    for (; !(c >= '0' && c <= '9'); c = nc())
        if (c == '-') b = -1;
    for (x = 0; c >= '0' && c <= '9'; x = x * 10 + c - '0', c = nc())
        ;
    x *= b;
}
int main() {
    int T;
    read(T);
    len[0] = -1;
    sieve();
    for (int cas = 1; cas <= T; cas++) {
        ANS = 0;
        for (int i = 1; i <= m; i++) {
            E[i].clear();
            P[i].clear();
        }
        m = 0;
        read(n);
        read(D);
        for (int i = 1; i <= n; i++) {
            vis[i] = 0;
            read(a[i]);
            m = max(m, a[i]);
            P[a[i]].push_back(i);
        }
        for (int i = 1; i < n; i++) {
            read(x[i]);
            read(y[i]);
            int gcd = __gcd(a[x[i]], a[y[i]]);
            E[gcd].push_back(i);
        }
        for (int i = 2; i <= m; i++) {
            if (!mu[i]) continue;
            mrk = i;
            cv = 0;  // i的倍数点数组
            for (int j = i; j <= m; j += i) {
                for (auto u : P[j]) {
                    v[++cv] = u;
                    son[u] = 0;  // 清空长儿子
                    g[u] = 0;    // 清空u的邻接表
                }
            }
            ed = 0;
            for (int j = i; j <= m; j += i) {
                for (auto id : E[j]) {
                    add(x[id], y[id]);
                    add(y[id], x[id]);
                }
            }
            for (int j = 1; j <= cv; j++) {
                int u = v[j];
                if (vis[u] == mrk) continue;
                Ans = 0;
                p = poor;
                dfs(u, 0);
                sum[u] = p;
                p += len[u] + 1;
                dfs2(u, 0);
                ANS -= mu[i] * Ans;
            }
        }
        printf("Case #%d: %lld\n", cas, ANS * 2);
    }
    return 0;
}
