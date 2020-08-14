/*
    高低位统计
*/
/*
    题目大意：
        给出一个随机数生成器，有m个询问，问第bi小的元素是啥
        询问中对于bi<bk,bj<bk,有bi+bj<=bk
    题解：(By Claris)
        对于所有的数字，我们将其按照高位分类，统计每个高16位有几个数字，
        然后定位每个询问的高16位是什么，因为只有100个高16位是被询问到的，
        把这100个高16位的数放入桶里，每次暴力查找，因为数据随机，
        因此每个高16位期望n/65536=153个数，一共只有15300个数字有用
*/
#include <algorithm>
#include <cstdio>
using namespace std;
int T, n, m, k[200], pos[200];
unsigned s[10000010], b[10000010], x, y, z;
unsigned q[65536], cnt[65536], u[65536], tot[65536];
unsigned xorshf96() {
    unsigned t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;
    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;
    return z;
}
int getpos(int x) {
    for (int i = 0;; i++)
        if (tot[i] >= x) return i;
}
unsigned query(int k, int p) {
    int i = p ? tot[p - 1] : 0, m = 0;
    for (k -= ++i; i <= tot[p]; i++) q[m++] = b[i];
    nth_element(q, q + k, q + m);
    return q[k];
}
int Cas = 1;
int main() {
    while (~scanf("%d%d%u%u%u", &n, &m, &x, &y, &z)) {
        for (int i = 0; i < 65536; i++) cnt[i] = u[i] = 0;
        for (int i = 1; i <= n; i++) {
            s[i] = xorshf96();
            cnt[s[i] >> 16]++;
        }
        for (int i = 1; i < 65536; i++) cnt[i] += cnt[i - 1];
        for (int i = 0; i < 65535; i++) tot[i] = cnt[i];
        for (int i = 1; i <= m; i++) {
            scanf("%d", &k[i]);
            pos[i] = getpos(++k[i]);
            u[pos[i]] = 1;
        }
        for (int i = 1; i <= n; i++)
            if (u[s[i] >> 16]) b[cnt[s[i] >> 16]--] = s[i];
        printf("Case #%d:", Cas++);
        for (int i = 1; i <= m; i++) printf(" %u", query(k[i], pos[i]));
        puts("");
    }
    return 0;
}
/*
    题目大意:
        给出一棵树，树上每个节点都有一个权值w，w不超过216，树的根为1，
        从一个点往根的方向走，可以得到他的祖先序列，
        现在需要从v1点的祖先序列中挑选出一定数量的点，
        组成数列v1，v2，v3……vm，要求vi是vi-1的祖先，
        求dp[v1]=max(dp[vi]+(w[v1] opt w[vi]))，
        opt是一种运算，在题目中可为xor，or或者and，
        最后求出ans=sum_{i=1}^{n}(i*(w[i]+dp[i]))
    题解:
        对于这道题，我们首先考虑它的简化版本，
        在一个一维数组上求dp[i]=max(dp[j]+(w[i] opt w[j])) (j<i)，
        显然枚举前缀的O(n2)的用脚趾头都能想出来的算法，出题人是不会给过的。
        那么我们观察一下题目，发现一个很奇巧的东西，w的值不超过216，
        难道说每次计算以w结尾的dp最大值，然后枚举二进制？
        一次6w多的计算量，明显也没有产生太大的优化，
        顺着这个思路下去，这道题采用了一种拆值DP的神奇的方式，
        dp[i]=max(dp[j]+([w[i]前八位]opt[w[j]前八位])<<8+[w[i]后八位]opt[w[j]后八位])
        记dp[A][B]=以前八位为A结尾，后八位以B结尾的dp值，于是就可以发现：
            dp[A][B]=max(dp[i][B]+([w[i]前八位]opt[w[A]前八位])<<8)
        那么，在知道了后八位的情况下，前八位就能轻松dp，
        既然这样，那我们就在计算完每个节点之后，预处理后八位的dp值：
            dp[A][i]=max(dp[A][j]+([w[i]后八位]opt[w[j]后八位]))
        这样子每次转移所需要的复杂度就只有28，可以接受。顺利完成。
        而这道题所处理的却是树上的问题，
        那么在每条链上DP的过程中预处理祖先节点dp数组，
        按照上述方法计算子节点的dp值即可，而对于不同的子节点，dp数组备份，然后回溯即可。
*/
#include <algorithm>
#include <cstdio>
#include <vector>
using namespace std;
typedef unsigned int UI;
const int N = 65540, mod = 1e9 + 7;
UI T, n, i, w[N], nxt[N], x, f[256][256], tmp[N][256], v[256], ans;
vector<UI> g[N];
char op[5];
UI opt(UI a, UI b) {
    if (op[0] == 'A') return a & b;
    if (op[0] == 'O') return a | b;
    if (op[0] == 'X') return a ^ b;
}
void dfs(UI x) {
    UI dp = 0, A = w[x] >> 8, B = w[x] & 255;
    for (int i = 0; i < 256; i++)
        if (v[i]) dp = max(dp, f[i][B] + (opt(A, i) << 8));
    ans = (1LL * x * (dp + w[x]) + ans) % mod;
    for (v[A]++, i = 0; i < 256; i++)
        tmp[x][i] = f[A][i], f[A][i] = max(f[A][i], opt(B, i) + dp);
    for (int i = 0; i < g[x].size(); i++) dfs(g[x][i]);
    for (v[A]--, i = 0; i < 256; i++) f[A][i] = tmp[x][i];
}
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d %s", &n, op);
        for (int i = 1; i <= n; i++) scanf("%d", &w[i]), g[i].clear();
        for (int i = 2; i <= n; i++) scanf("%d", &x), g[x].push_back(i);
        ans = 0;
        dfs(1);
        printf("%d\n", ans);
    }
    return 0;
}
