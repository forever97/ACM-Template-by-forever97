/*
    精确覆盖问题：给定一个由0-1组成的矩阵，是否能找到一个行的集合，使得集合中每一列都恰好包含一个1
    In_Test:
        4 4
        2 1 3 （行中1的个数和位置）
        1 2
        1 1
        2 3 4
    Out_Test:
        3 2 4 3 （找到的那个集合大小和集合输出）
*/
#include <cstdio>
const int N = 100100;
struct DLX {
    int L[N], R[N], U[N], D[N], row[N], col[N];
    int H[1010];
    int ansd, ans[1010];
    int s[1010];
    int tot, n, m;
    void init(int _n, int _m) {
        n = _n;
        m = _m;
        tot = m;
        for (int i = 0; i <= m; i++)
            L[i] = i - 1, R[i] = i + 1, U[i] = i, D[i] = i, s[i] = 0;
        L[0] = m;
        R[m] = 0;
        for (int i = 1; i <= n; i++) H[i] = -1;
    }
    void link(int i, int j) {
        s[j]++;
        ++tot;
        row[tot] = i;
        col[tot] = j;
        U[tot] = U[j];
        D[tot] = j;
        D[U[j]] = tot;
        U[j] = tot;
        if (H[i] < 0) {
            H[i] = tot;
            L[tot] = tot;
            R[tot] = tot;
        } else {
            L[tot] = L[H[i]];
            R[tot] = H[i];
            R[L[H[i]]] = tot;
            L[H[i]] = tot;
        }
    }
    void remove(int c) {
        R[L[c]] = R[c];
        L[R[c]] = L[c];
        for (int i = D[c]; i != c; i = D[i])
            for (int j = R[i]; j != i; j = R[j]) {
                s[col[j]]--;
                D[U[j]] = D[j];
                U[D[j]] = U[j];
            }
    }
    void resume(int c) {
        R[L[c]] = c;
        L[R[c]] = c;
        for (int i = U[c]; i != c; i = U[i])
            for (int j = L[i]; j != i; j = L[j]) {
                s[col[j]]++;
                D[U[j]] = j;
                U[D[j]] = j;
            }
    }
    bool dfs(int d) {
        if (R[0] == 0) {
            ansd = d - 1;
            return 1;
        }
        int c = R[0];
        for (int i = R[0]; i != 0; i = R[i])
            if (s[i] < s[c]) c = i;
        remove(c);
        for (int i = D[c]; i != c; i = D[i]) {
            ans[d] = row[i];
            for (int j = R[i]; j != i; j = R[j]) remove(col[j]);
            if (dfs(d + 1)) return 1;
            for (int j = L[i]; j != i; j = L[j]) resume(col[j]);
        }
        resume(c);
        return 0;
    }
} g;
int main() {
    int n, m, k, x;
    while (~scanf("%d%d", &n, &m)) {
        g.init(n, m);
        for (int i = 1; i <= n; i++) {
            scanf("%d", &k);
            for (int j = 1; j <= k; j++) scanf("%d", &x), g.link(i, x);
        }
        if (g.dfs(1)) {
            printf("%d", g.ansd);
            for (int i = 1; i <= g.ansd; i++) printf(" %d", g.ans[i]);
            printf("\n");
        } else
            puts("NO\n");
    }
    return 0;
}
/*
    重复覆盖问题：给定一个由0-1组成的矩阵，是否能找到k行，使得集合中每一列都至少包含一个1
    Problem：n城市，建k机场使得，使得每个城市最近机场的距离的最大值最小化，输出距离
    Solution：二分+DLX检测
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
using namespace std;
typedef long long ll;
const int N = 65, M = 65, V = 65 * 65;
int n, k;
struct DLX {
    int L[V], R[V], U[V], D[V], row[V], col[V];
    int H[N];
    int s[M];
    int tot, n, m;
    bool vis[M];
    void init(int _n, int _m) {
        n = _n;
        m = _m;
        tot = m;
        for (int i = 0; i <= m; i++)
            L[i] = i - 1, R[i] = i + 1, U[i] = i, D[i] = i, s[i] = 0;
        L[0] = m;
        R[m] = 0;
        for (int i = 1; i <= n; i++) H[i] = -1;
    }
    void link(int i, int j) {
        s[j]++;
        ++tot;
        row[tot] = i;
        col[tot] = j;
        U[tot] = U[j];
        D[tot] = j;
        D[U[j]] = tot;
        U[j] = tot;
        if (H[i] < 0) {
            H[i] = tot;
            L[tot] = tot;
            R[tot] = tot;
        } else {
            L[tot] = L[H[i]];
            R[tot] = H[i];
            R[L[H[i]]] = tot;
            L[H[i]] = tot;
        }
    }
    void remove(int c) {
        for (int i = D[c]; i != c; i = D[i]) R[L[i]] = R[i], L[R[i]] = L[i];
    }
    void resume(int c) {
        for (int i = U[c]; i != c; i = U[i]) R[L[i]] = L[R[i]] = i;
    }
    int h() {
        int ret = 0;
        for (int c = R[0]; c != 0; c = R[c]) {
            if (D[c] == c) return n + 1;
            vis[c] = 0;
        }
        for (int c = R[0]; c != 0; c = R[c])
            if (!vis[c]) {
                ret++;
                vis[c] = 1;
                for (int i = D[c]; i != c; i = D[i])
                    for (int j = R[i]; j != i; j = R[j]) vis[col[j]] = 1;
            }
        return ret;
    }
    bool dfs(int d) {
        if (d + h() > k) return 0;
        if (R[0] == 0) return 1;
        int c = R[0];
        for (int i = R[0]; i != 0; i = R[i])
            if (s[i] < s[c]) c = i;
        for (int i = D[c]; i != c; i = D[i]) {
            remove(i);
            for (int j = R[i]; j != i; j = R[j]) remove(j);
            if (dfs(d + 1)) return 1;
            for (int j = L[i]; j != i; j = L[j]) resume(j);
            resume(i);
        }
        return 0;
    }
} g;
struct P {
    int x, y;
    ll operator-(const P &t) { return 0ll + abs(x - t.x) + abs(y - t.y); }
} a[N];
bool check(ll len) {
    g.init(n, n);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (a[i] - a[j] <= len) g.link(i, j);
    return g.dfs(0);
}
ll dis[N * N];
int main() {
    int t;
    scanf("%d", &t);
    for (int cas = 1; cas <= t; cas++) {
        scanf("%d%d", &n, &k);
        for (int i = 1; i <= n; i++) scanf("%d%d", &a[i].x, &a[i].y);
        int cnt = 0;
        for (int i = 1; i < n; i++)
            for (int j = i + 1; j <= n; j++) dis[++cnt] = a[i] - a[j];
        dis[++cnt] = 0;
        sort(dis + 1, dis + cnt + 1);
        int l = 1, r = cnt + 1;
        while (l < r) {
            int mid = (l + r) / 2;
            if (!check(dis[mid]))
                l = mid + 1;
            else
                r = mid;
        }
        printf("Case #%d: %lld\n", cas, dis[l]);
    }
    return 0;
}