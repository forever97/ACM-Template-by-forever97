/*
    插头DP
    维护连通块
    状态用最小表示法
    逐格递推清空哈希表
    当相同状态在hash中出现根据题目要求求极值或者累加
*/
/*
    Example
        左上角到右下角经过所有非障碍格的方案数
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long LL;
const int bit = 7, dig = 3;
const int N = 200010;
const int SIZE = 4001;
struct Hash_Table {
    int size, g[SIZE], nxt[N];
    LL state[N], value[N];
    void init() {
        size = 0;
        memset(g, -1, sizeof(g));
    }
    void push(LL S, LL V) {
        int id = S % SIZE;
        for (int i = g[id]; ~i; i = nxt[i]) {
            if (state[i] == S) {
                value[i] += V;
                return;
            }
        }
        state[size] = S, value[size] = V;
        nxt[size] = g[id], g[id] = size++;
    }
} dp[2];
int n, m;
int code[20], vis[20];
int cur, pre;
int mp[20][20];
int edx, edy;
LL ans;
inline void Decode(LL S, int m) {
    for (int i = 0; i <= m; i++) code[i] = S & bit, S >>= dig;
}
inline LL Encode(int m) {
    LL res = 0;
    int x = 1;
    memset(vis, -1, sizeof(vis));
    vis[0] = 0;
    for (int i = m; i >= 0; i--) {
        if (-1 == vis[code[i]]) vis[code[i]] = x++;
        code[i] = vis[code[i]];
        res <<= dig;
        res |= code[i];
    }
    return res;
}
inline bool check(int m) {
    for (int i = 0; i <= m; i++)
        if (code[i]) return 0;
    return 1;
}
inline void DP(int x, int y, int k) {
    Decode(dp[pre].state[k], m);
    int lft = code[y - 1], up = code[y];
    LL V = dp[pre].value[k], S = dp[pre].state[k];
    code[y] = code[y - 1] = 0;
    if ((x == n && y == 1) || (x == n && y == m)) {
        if ((lft && !up) || (!lft && up))
            dp[cur].push(Encode(m), V);
        else if (!lft && !up) {
            if (x < n && mp[x + 1][y]) {
                code[y - 1] = bit;
                dp[cur].push(Encode(m), V), code[y - 1] = 0;
            }
            if (y < m && mp[x][y + 1]) {
                code[y] = bit;
                dp[cur].push(Encode(m), V), code[y] = 0;
            }
        }
    } else if (!lft && !up) {
        if (!mp[x][y])
            dp[cur].push(Encode(m), V);
        else if (x < n && mp[x + 1][y] && y < m && mp[x][y + 1]) {
            code[y] = code[y - 1] = bit;
            dp[cur].push(Encode(m), V);
        }
    } else if (!lft || !up) {
        if (x < n && mp[x + 1][y]) {
            code[y - 1] = lft + up;
            dp[cur].push(Encode(m), V), code[y - 1] = 0;
        }
        if (y < m && mp[x][y + 1]) {
            code[y] = lft + up;
            dp[cur].push(Encode(m), V), code[y] = 0;
        }
    } else if (lft != up) {
        for (int i = 0; i <= m; i++)
            if (code[i] == lft) code[i] = up;
        dp[cur].push(Encode(m), V);
    }
}
LL PlugDp() {
    cur = 0;
    dp[0].init();
    dp[0].push(0, 1);
    for (int i = 1; i <= n; i++) {
        pre = cur, cur ^= 1;
        dp[cur].init();
        for (int k = 0; k < dp[pre].size; k++)
            dp[cur].push(dp[pre].state[k] << dig, dp[pre].value[k]);
        for (int j = 1; j <= m; j++) {
            pre = cur, cur ^= 1, dp[cur].init();
            for (int k = 0; k < dp[pre].size; k++) DP(i, j, k);
        }
    }
    ans = 0;
    for (int k = 0; k < dp[cur].size; k++) ans += dp[cur].value[k];
    return ans;
}
char s[30];
int main() {
    while (scanf("%d%d", &n, &m), n + m) {
        memset(mp, 0, sizeof(mp));
        for (int i = 1; i <= n; i++) {
            scanf("%s", s + 1);
            for (int j = 1; j <= m; j++) {
                if (s[j] == '.') mp[i][j] = 1;
            }
        }
        printf("%lld\n", PlugDp());
    }
    return 0;
}
/*
    Example
        给出一个格子图，'O'表示必须通过，'X'表示不能通过，'*'表示可通过
        问画一条经过所有'O'的哈密顿回路的方案数
    Solution
        对于可走可不走的格子我们可以将其分必须通过和不能通过状态讨论加入，
        我们对于连通块标号采用最小表示法，当发现不同标号的两个插头合并时，
        需要将其中一种的左右插头换成另一种的标号，
        相同标号的两个插头合并时必须在结束状态。
        由于可走可不走格子的存在导致终结位置不唯一，
        我们只要在最后一个O后面的格子判断和累加答案即可
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long LL;
const int bit = 7, dig = 3;
const int N = 200010;
const int SIZE = 4001;
struct Hash_Table {
    int size, g[SIZE], nxt[N];
    LL state[N], value[N];
    void init() {
        size = 0;
        memset(g, -1, sizeof(g));
    }
    void push(LL S, LL V) {
        int id = S % SIZE;
        for (int i = g[id]; ~i; i = nxt[i]) {
            if (state[i] == S) {
                value[i] += V;
                return;
            }
        }
        state[size] = S, value[size] = V;
        nxt[size] = g[id], g[id] = size++;
    }
} dp[2];
int n, m;
int code[20], vis[20];
int cur, pre;
char mp[20][20];
int edx, edy;
LL ans;
inline void Decode(LL S, int m) {
    for (int i = 0; i <= m; i++) code[i] = S & bit, S >>= dig;
}
inline LL Encode(int m) {
    LL res = 0;
    int x = 1;
    memset(vis, -1, sizeof(vis));
    vis[0] = 0;
    for (int i = m; i >= 0; i--) {
        if (-1 == vis[code[i]]) vis[code[i]] = x++;
        code[i] = vis[code[i]];
        res <<= dig;
        res |= code[i];
    }
    return res;
}
inline bool check(int m) {
    for (int i = 0; i <= m; i++)
        if (code[i]) return 0;
    return 1;
}
inline void DP(int x, int y, int k) {
    Decode(dp[pre].state[k], m);
    int lft = code[y - 1], up = code[y];
    LL V = dp[pre].value[k], S = dp[pre].state[k];
    code[y] = code[y - 1] = 0;
    if (mp[x][y] == 'X' || mp[x][y] == '*') {  // 不能走|可走可不走
        if (!lft && !up) dp[cur].push(S, V);
    }
    if (mp[x][y] == 'O' || mp[x][y] == '*') {  // 必经|可走可不走
        if (!lft && !up) {
            if (x < n && mp[x + 1][y] != 'X' && y < m && mp[x][y + 1] != 'X') {
                code[y] = code[y - 1] = bit;
                dp[cur].push(Encode(m), V);
            }
        } else if (!lft || !up) {
            if (x < n && mp[x + 1][y] != 'X') {
                code[y - 1] = lft + up;
                dp[cur].push(Encode(m), V), code[y - 1] = 0;
            }
            if (y < m && mp[x][y + 1] != 'X') {
                code[y] = lft + up;
                dp[cur].push(Encode(m), V), code[y] = 0;
            }
        } else if (lft != up) {
            for (int i = 0; i <= m; i++)
                if (code[i] == lft) code[i] = up;
            dp[cur].push(Encode(m), V);
        } else if ((x > edx || (x == edx && y >= edy)) && lft == up && check(m))
            ans += V;
    }
}
/*
    Example
        给出一张格子图，每个格子上有个权值，问从左上角到达右下角，
        可以获得的简单路最大路径权值和是多少
    Solution
        开始状态建立单插头，终结状态只接受单插头，因为每个格子不是必经，
        因此需要加入其成为障碍格和成为必经格两个状态，对于连通块的合并，
        必须标号不同，这样才能保证答案是简单路。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long LL;
const int bit = 7, dig = 3;
const int N = 200010;
const int SIZE = 4001;
struct Hash_Table {
    int size, g[SIZE], nxt[N];
    LL state[N], value[N];
    void init() {
        size = 0;
        memset(g, -1, sizeof(g));
    }
    void push(LL S, LL V) {
        int id = S % SIZE;
        for (int i = g[id]; ~i; i = nxt[i]) {
            if (state[i] == S) {
                value[i] = max(value[i], V);
                return;
            }
        }
        state[size] = S, value[size] = V;
        nxt[size] = g[id], g[id] = size++;
    }
} dp[2];
int n, m;
int code[20], vis[20];
int cur, pre;
int mp[20][20];
int edx, edy;
LL ans;
inline void Decode(LL S, int m) {
    for (int i = 0; i <= m; i++) code[i] = S & bit, S >>= dig;
}
inline LL Encode(int m) {
    LL res = 0;
    int x = 1;
    memset(vis, -1, sizeof(vis));
    vis[0] = 0;
    for (int i = m; i >= 0; i--) {
        if (-1 == vis[code[i]]) vis[code[i]] = x++;
        code[i] = vis[code[i]];
        res <<= dig;
        res |= code[i];
    }
    return res;
}
inline bool check(int m) {
    for (int i = 0; i <= m; i++)
        if (code[i]) return 0;
    return 1;
}
inline void DP(int x, int y, int k) {
    Decode(dp[pre].state[k], m);
    int lft = code[y - 1], up = code[y];
    LL V = dp[pre].value[k], S = dp[pre].state[k];
    code[y] = code[y - 1] = 0;
    if ((x == 1 && y == 1) || (x == n && y == m)) {
        if ((lft && !up) || (!lft && up)) ans = max(ans, V + mp[x][y]);
        if (!lft && !up) {
            if (x < n) {
                code[y - 1] = bit;
                dp[cur].push(Encode(m), V + mp[x][y]), code[y - 1] = 0;
            }
            if (y < m) {
                code[y] = bit;
                dp[cur].push(Encode(m), V + mp[x][y]), code[y] = 0;
            }
        }
    } else if (!lft && !up) {
        if (x < n && y < m) {
            code[y] = code[y - 1] = bit;
            dp[cur].push(Encode(m), V + mp[x][y]);
        }
        code[y] = code[y - 1] = 0;
        dp[cur].push(Encode(m), V);
    } else if (!lft || !up) {
        if (x < n) {
            code[y - 1] = lft + up;
            dp[cur].push(Encode(m), V + mp[x][y]), code[y - 1] = 0;
        }
        if (y < m) {
            code[y] = lft + up;
            dp[cur].push(Encode(m), V + mp[x][y]), code[y] = 0;
        }
    } else if (lft != up) {
        for (int i = 0; i <= m; i++)
            if (code[i] == lft) code[i] = up;
        dp[cur].push(Encode(m), V + mp[x][y]);
    }
}
LL PlugDp() {
    cur = 0;
    dp[0].init();
    dp[0].push(0, 0);
    for (int i = 1; i <= n; i++) {
        pre = cur, cur ^= 1;
        dp[cur].init();
        for (int k = 0; k < dp[pre].size; k++)
            dp[cur].push(dp[pre].state[k] << dig, dp[pre].value[k]);
        for (int j = 1; j <= m; j++) {
            pre = cur, cur ^= 1, dp[cur].init();
            for (int k = 0; k < dp[pre].size; k++) DP(i, j, k);
        }
    }
    return ans;
}
int T, cas = 0;
int main() {
    while (~scanf("%d%d", &n, &m)) {
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++) scanf("%d", &mp[i][j]);
        ans = -0x3f3f3f3f3f3f3f3f;
        edx = n, edy = m;
        if (n == 1 & m == 1) {
            printf("Case %d: %d\n", ++cas, mp[1][1]);
            continue;
        }
        printf("Case %d: %lld\n", ++cas, PlugDp());
    }
    return 0;
}
/*
    Example
        给出一个方格图，格子之间的边有一定的价值，
        求一条经过所有格子的回路使得穿过的边权和最小
    Solution
        插头dp，在每个状态加入hashtable的时候更新该状态的最小值，
        在终结状态更新答案即可
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long LL;
const int bit = 7, dig = 3;
const int N = 200010;
const int SIZE = 4001;
struct Hash_Table {
    int size, g[SIZE], nxt[N];
    LL state[N], value[N];
    void init() {
        size = 0;
        memset(g, -1, sizeof(g));
    }
    void push(LL S, LL V) {
        int id = S % SIZE;
        // 对于哈密顿最短回路的时候注意hash里面的累加改为取最小值
        for (int i = g[id]; ~i; i = nxt[i]) {
            if (state[i] == S) {
                value[i] = min(value[i], V);
                return;
            }
        }
        state[size] = S, value[size] = V;
        nxt[size] = g[id], g[id] = size++;
    }
} dp[2];
int n, m;
int code[20], vis[20];
int cur, pre;
char str[10010];
int rht[20][20], dwn[20][20];
int edx, edy;
LL ans;
inline void Decode(LL S, int m) {
    for (int i = 0; i <= m; i++) code[i] = S & bit, S >>= dig;
}
inline LL Encode(int m) {
    LL res = 0;
    int x = 1;
    memset(vis, -1, sizeof(vis));
    vis[0] = 0;
    for (int i = m; i >= 0; i--) {
        if (-1 == vis[code[i]]) vis[code[i]] = x++;
        code[i] = vis[code[i]];
        res <<= dig;
        res |= code[i];
    }
    return res;
}
inline bool check(int m) {
    for (int i = 0; i <= m; i++)
        if (code[i]) return 0;
    return 1;
}
inline void DP(int x, int y, int k) {
    Decode(dp[pre].state[k], m);
    int lft = code[y - 1], up = code[y];
    LL V = dp[pre].value[k], S = dp[pre].state[k];
    code[y] = code[y - 1] = 0;
    if (!lft && !up) {
        if (x < n && y < m) {
            code[y] = code[y - 1] = bit;
            dp[cur].push(Encode(m), V + dwn[x][y] + rht[x][y]);
        }
    } else if (!lft || !up) {
        if (x < n) {
            code[y - 1] = lft + up;
            dp[cur].push(Encode(m), V + dwn[x][y]), code[y - 1] = 0;
        }
        if (y < m) {
            code[y] = lft + up;
            dp[cur].push(Encode(m), V + rht[x][y]), code[y] = 0;
        }
    } else if (lft != up) {
        for (int i = 0; i <= m; i++)
            if (code[i] == lft) code[i] = up;
        dp[cur].push(Encode(m), V);
    } else if ((x == edx && y == edy) && lft == up && check(m))
        ans = min(ans, V);
}
LL PlugDp() {
    cur = 0;
    dp[0].init();
    dp[0].push(0, 0);
    for (int i = 1; i <= n; i++) {
        pre = cur, cur ^= 1;
        dp[cur].init();
        for (int k = 0; k < dp[pre].size; k++)
            dp[cur].push(dp[pre].state[k] << dig, dp[pre].value[k]);
        for (int j = 1; j <= m; j++) {
            pre = cur, cur ^= 1, dp[cur].init();
            for (int k = 0; k < dp[pre].size; k++) DP(i, j, k);
        }
    }
    return ans;
}
int T;
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        getchar();
        gets(str);
        for (int i = 1; i < n; i++) {
            gets(str);
            for (int j = 1; j < m; j++) rht[i][j] = str[2 * j] - '0';
            gets(str);
            for (int j = 1; j <= m; j++) dwn[i][j] = str[2 * j - 1] - '0';
        }
        gets(str);
        for (int i = 1; i < m; i++) rht[n][i] = str[2 * i] - '0';
        gets(str);
        ans = 1e10;
        edx = n, edy = m;
        printf("%lld\n", PlugDp());
    }
    return 0;
}
