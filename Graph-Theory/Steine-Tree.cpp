// 斯坦纳树：只用包含关键点的最小生成树
/*
 * Problem: 给出一张图，要求i属于1~d号点与n-i+1号点对应连通，求最小边权和
 * Solution: f[i][msk]表示i为根，关键点连通状态为msk时候的最小代价
 * Ans[msk]表示连通态为msk时最小代价，如果对应点同时连通或不连通则可以更新
 */
// Demo1
#include <bits/stdc++.h>
using namespace std;
const int N = 10005;
const int MOD = 1e9 + 7;
int n, m, d, x, y, z;
int nxt[N << 1], g[N], v[N << 1], w[N << 1], tot;
int a[N], All, f[N][(1 << 8) + 10], INF, vis[N];
int Ans[(1 << 8) + 10];
void add_edge(int a, int b, int c) {
    nxt[++tot] = g[a];
    g[a] = tot;
    v[tot] = b;
    w[tot] = c;
}
void Add(int a, int b, int c) {
    add_edge(a, b, c);
    add_edge(b, a, c);
}
typedef pair<int, int> seg;
priority_queue<seg, vector<seg>, greater<seg>> q;
namespace Steiner {
void init() {
    memset(f, 63, sizeof(f));
    INF = f[0][0];
    int num = 0;
    for (int i = 1; i <= d; i++) f[i][1 << num] = 0, num++;
    for (int i = n - d + 1; i <= n; i++) f[i][1 << num] = 0, num++;
    All = (1 << num) - 1;
}
void Dijkstra(int msk) {
    while (q.size()) {
        int x = q.top().second;
        q.pop();
        for (int e = g[x]; e; e = nxt[e]) {
            int y = v[e];
            if (f[y][msk] > f[x][msk] + w[e]) {
                f[y][msk] = f[x][msk] + w[e];
                if (!vis[y]) {
                    vis[y] = 1;
                    q.push(make_pair(f[y][msk], y));
                }
            }
        }
        vis[x] = 0;
    }
}
void Deal() {
    for (int msk = 0; msk <= All; msk++) {
        for (int i = 1; i <= n; i++) {
            for (int sub = msk; sub; sub = (sub - 1) & msk)
                f[i][msk] = min(f[i][msk], f[i][sub] + f[i][msk ^ sub]);
            if (f[i][msk] != INF) {
                q.push(make_pair(f[i][msk], i));
                vis[i] = 1;
            }
        }
        Dijkstra(msk);
    }
}
}  // namespace Steiner
bool Check(int msk) {
    for (int i = 0, j = (d << 1) - 1; i < d; i++, j--)
        if (((msk & (1 << i)) == 0) != ((msk & (1 << j)) == 0)) return 0;
    return 1;
}
int main() {
    scanf("%d%d%d", &n, &m, &d);
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d", &x, &y, &z);
        Add(x, y, z);
    }
    Steiner::init();
    Steiner::Deal();
    memset(Ans, 63, sizeof(Ans));
    for (int msk = 0; msk <= All; msk++)
        if (Check(msk)) {
            for (int i = 1; i <= n; i++) Ans[msk] = min(Ans[msk], f[i][msk]);
        }
    for (int msk = 0; msk <= All; msk++)
        for (int sub = msk; sub; sub = (sub - 1) & msk)
            Ans[msk] = min(Ans[msk], Ans[sub] + Ans[msk ^ sub]);
    if (Ans[All] == INF)
        printf("-1");
    else
        printf("%d", Ans[All]);
    return 0;
}
// Demo2
#include <bits/stdc++.h>
const int MOD = 1e9 + 7;
using Info = std::pair<int, int>;
template <typename T>
using PQ = std::priority_queue<T, std::vector<T>, std::greater<T>>;
static const Info ONE{1, 1};
int lowbit(int msk) { return msk & -msk; }
Info add(const Info& a, const Info& b) {
    return {a.first + b.first, 1LL * a.second * b.second % MOD};
}
void update(Info& x, const Info& a) {
    if (a.first < x.first) {
        x = {a.first, 0};
    }
    if (a.first == x.first) {
        x.second += a.second;
        if (x.second >= MOD) {
            x.second -= MOD;
        }
    }
}
int n, m, l;
int Ans[(1 << 8) + 10];
bool Check(int msk) {
    for (int i = 0, j = (l << 1) - 1; i < l; i++, j--)
        if (((msk & (1 << i)) == 0) != ((msk & (1 << j)) == 0)) return 0;
    return 1;
}
int main() {
    while (~scanf("%d%d%d", &n, &m, &l)) {
        memset(Ans, 63, sizeof(Ans));
        int INF = Ans[0];
        std::vector<std::vector<int>> graph(n);
        std::vector<std::vector<int>> W(n);
        for (int i = 0, a, b, c; i < m; i++) {
            scanf("%d%d%d", &a, &b, &c);
            a--, b--;
            graph[a].push_back(b);
            W[a].push_back(c);
            graph[b].push_back(a);
            W[b].push_back(c);
        }
        int All = (1 << (l << 1)) - 1;
        std::vector<std::vector<Info>> dp(All + 1,
                                          std::vector<Info>(n, Info{INF, 0})),
            merged(n, std::vector<Info>(All + 1, Info{INF, 0}));
        int num = 0;
        for (int i = 0; i < l; i++) {
            dp[1 << num][i] = {0, 1};
            num++;
        }
        for (int i = n - l; i < n; i++) {
            dp[1 << num][i] = {0, 1};
            num++;
        }
        for (int msk = 0; msk < 1 << (l << 1); msk++) {
            for (int u = 0; u < n; u++) {
                auto& ref = merged.at(u);
                for (int subset = msk; subset > 0; subset = subset - 1 & msk) {
                    if (lowbit(subset) == lowbit(msk)) {
                        update(ref.at(msk),
                               add(dp.at(subset).at(u), ref.at(msk ^ subset)));
                    }
                }
            }
            for (int u = 0; u < n; u++) {
                for (int i = 0; i < graph[u].size(); i++) {
                    int v = graph[u][i];
                    int w = W[u][i];
                    update(dp.at(msk).at(v),
                           add(merged.at(u).at(msk), Info{w, 1}));
                }
            }
            auto& ref = dp.at(msk);
            PQ<std::pair<int, int>> pq;
            for (int u = 0; u < n; u++) {
                pq.emplace(ref.at(u).first, u);
            }
            while (!pq.empty()) {
                auto top = pq.top();
                pq.pop();
                int u = top.second;
                if (top.first == ref.at(u).first) {
                    for (int i = 0; i < graph[u].size(); i++) {
                        int v = graph[u][i];
                        int w = W[u][i];
                        Info todo = add(ref.at(u), Info{w, 1});
                        if (todo.first < ref.at(v).first) {
                            pq.emplace(todo.first, v);
                        }
                        update(ref.at(v), todo);
                    }
                }
            }
            for (int u = 0; u < n; u++) {
                update(merged.at(u).at(msk), dp.at(msk).at(u));
            }
        }
        for (int msk = 0; msk <= All; msk++)
            if (Check(msk)) {
                for (int i = 0; i < n; i++) {
                    int tmp = merged.at(i).at(msk).first;
                    Ans[msk] = std::min(Ans[msk], tmp);
                }
            }
        for (int msk = 0; msk <= All; msk++)
            for (int sub = msk; sub; sub = (sub - 1) & msk)
                Ans[msk] = std::min(Ans[msk], Ans[sub] + Ans[msk ^ sub]);
        if (Ans[All] == INF)
            printf("-1");
        else
            printf("%d", Ans[All]);
    }
    return 0;
}
/*
 * 超级源点题
 * 给出一些庙宇的位置，以及另一些位置，每个位置都可以打井，打井的费用不同，
 * 现在这些位置不连通，让两个位置连通需要修路，给出修每条路的代价，
 * 现在问这些庙宇都能喝上水(该处是水井或者能到有水井的地方)需要的最小总代价
 *
 * 建立一个超级源点表示水源，打井的代价转化为该地到水源的路的代价
 * 那么题目转化为将水源与所有庙宇连在一起需要的最小代价，斯坦纳树求解即可
 */
#include <bits/stdc++.h>
const int MOD = 1e9 + 7;
using Info = std::pair<int, int>;
template <typename T>
using PQ = std::priority_queue<T, std::vector<T>, std::greater<T>>;
static const Info ONE{1, 1};
int lowbit(int msk) { return msk & -msk; }
Info add(const Info& a, const Info& b) {
    return {a.first + b.first, 1LL * a.second * b.second % MOD};
}
void update(Info& x, const Info& a) {
    if (a.first < x.first) {
        x = {a.first, 0};
    }
    if (a.first == x.first) {
        x.second += a.second;
        if (x.second >= MOD) {
            x.second -= MOD;
        }
    }
}
int n, m, l;
int main() {
    while (~scanf("%d%d%d", &n, &m, &l)) {
        std::vector<std::vector<int>> graph(n + m + 1);
        std::vector<std::vector<int>> W(n + m + 1);
        for (int i = 1, c; i <= n + m; i++) {
            scanf("%d", &c);
            graph[i].push_back(0);
            W[i].push_back(c);
            graph[0].push_back(i);
            W[0].push_back(c);
        }
        int INF = 1 << 28;
        for (int i = 0, a, b, c; i < l; i++) {
            scanf("%d%d%d", &a, &b, &c);
            graph[a].push_back(b);
            W[a].push_back(c);
            graph[b].push_back(a);
            W[b].push_back(c);
        }
        int All = (1 << (n + 1)) - 1;
        std::vector<std::vector<Info>> dp(
            All + 1, std::vector<Info>(n + m + 1, Info{INF, 0})),
            merged(n + m + 1, std::vector<Info>(All + 1, Info{INF, 0}));
        int num = 0;
        for (int i = 0; i <= n; i++) {
            dp[1 << num][i] = {0, 1};
            num++;
        }
        for (int msk = 0; msk < 1 << (n + 1); msk++) {
            for (int u = 0; u <= n + m; u++) {
                auto& ref = merged.at(u);
                for (int subset = msk; subset > 0; subset = subset - 1 & msk) {
                    if (lowbit(subset) == lowbit(msk)) {
                        update(ref.at(msk),
                               add(dp.at(subset).at(u), ref.at(msk ^ subset)));
                    }
                }
            }
            for (int u = 0; u <= n + m; u++) {
                for (int i = 0; i < graph[u].size(); i++) {
                    int v = graph[u][i];
                    int w = W[u][i];
                    update(dp.at(msk).at(v),
                           add(merged.at(u).at(msk), Info{w, 1}));
                }
            }
            auto& ref = dp.at(msk);
            PQ<std::pair<int, int>> pq;
            for (int u = 0; u <= n + m; u++) {
                pq.emplace(ref.at(u).first, u);
            }
            while (!pq.empty()) {
                auto top = pq.top();
                pq.pop();
                int u = top.second;
                if (top.first == ref.at(u).first) {
                    for (int i = 0; i < graph[u].size(); i++) {
                        int v = graph[u][i];
                        int w = W[u][i];
                        Info todo = add(ref.at(u), Info{w, 1});
                        if (todo.first < ref.at(v).first) {
                            pq.emplace(todo.first, v);
                        }
                        update(ref.at(v), todo);
                    }
                }
            }
            for (int u = 0; u <= n + m; u++) {
                update(merged.at(u).at(msk), dp.at(msk).at(u));
            }
        }
        int ans = INF;
        for (int i = 0; i <= n + m; i++) {
            int tmp = merged.at(i).at(All).first;
            ans = std::min(ans, tmp);
        }
        printf("%d\n", ans);
    }
    return 0;
}
// 给出一张图，求斯坦纳树的方案数
#include <bits/stdc++.h>
const int MOD = 1e9 + 7;
using Info = std::pair<int, int>;
template <typename T>
using PQ = std::priority_queue<T, std::vector<T>, std::greater<T>>;
static const Info ONE{1, 1};
int lowbit(int msk) { return msk & -msk; }
Info add(const Info& a, const Info& b) {
    return {a.first + b.first, 1LL * a.second * b.second % MOD};
}
void update(Info& x, const Info& a) {
    if (a.first < x.first) {
        x = {a.first, 0};
    }
    if (a.first == x.first) {
        x.second += a.second;
        if (x.second >= MOD) {
            x.second -= MOD;
        }
    }
}
int main() {
    int n, m, l;
    while (~scanf("%d%d%d", &n, &m, &l)) {
        std::vector<std::vector<int>> graph(n);
        for (int i = 0, a, b; i < m; i++) {
            scanf("%d%d", &a, &b);
            a--, b--;
            graph[a].push_back(b);
            graph[b].push_back(a);
        }
        if (l == 1) {
            puts("1");
            continue;
        }
        l--;
        std::vector<std::vector<Info>> dp(1 << l,
                                          std::vector<Info>(n, Info{m + 1, 0})),
            merged(n, std::vector<Info>(1 << l, Info{m + 1, 0}));
        int root = l;
        for (int i = 0; i < l; i++) {
            dp[1 << i][i] = {0, 1};
        }
        for (int msk = 0; msk < 1 << l; msk++) {
            for (int u = 0; u < n; u++) {
                auto& ref = merged.at(u);
                for (int subset = msk; subset > 0; subset = subset - 1 & msk) {
                    if (lowbit(subset) == lowbit(msk)) {
                        update(ref.at(msk),
                               add(dp.at(subset).at(u), ref.at(msk ^ subset)));
                    }
                }
            }
            for (int u = 0; u < n; u++) {
                for (int v : graph[u]) {
                    update(dp.at(msk).at(v), add(merged.at(u).at(msk), ONE));
                }
            }
            auto& ref = dp.at(msk);
            PQ<std::pair<int, int>> pq;
            for (int u = 0; u < n; u++) {
                pq.emplace(ref.at(u).first, u);
            }
            while (!pq.empty()) {
                auto top = pq.top();
                pq.pop();
                int u = top.second;
                if (top.first == ref.at(u).first) {
                    for (int v : graph.at(u)) {
                        Info todo = add(ref.at(u), ONE);
                        if (todo.first < ref.at(v).first) {
                            pq.emplace(todo.first, v);
                        }
                        update(ref.at(v), todo);
                    }
                }
            }
            for (int u = 0; u < n; u++) {
                update(merged.at(u).at(msk), dp.at(msk).at(u));
            }
        }
        printf("%d\n", merged.at(root).at((1 << l) - 1).second);
    }
    return 0;
}