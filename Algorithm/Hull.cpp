/*
    凸壳维护
    可解决形如 dp[i]=min( dp[j] + b[j]*a[i] )  for j<i 的问题
    要求 b[j]>=b[j+1] a[i]<=a[i+1]
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int N = 100010;
struct Hull {
    long long a[N], b[N];
    double x[N];
    int head, tail;
    Hull() : head(1), tail(0) {}
    long long get(long long xQuery) {
        if (head > tail) return 0;
        while (head < tail && x[head + 1] <= xQuery) head++;
        x[head] = xQuery;
        return a[head] * xQuery + b[head];
    }
    void add(long long aNew, long long bNew) {
        double xNew = -1e18;
        while (head <= tail) {
            if (aNew == a[tail]) return;
            xNew = 1.0 * (b[tail] - bNew) / (aNew - a[tail]);
            if (head == tail || xNew >= x[tail]) break;
            tail--;
        }
        a[++tail] = aNew;
        b[tail] = bNew;
        x[tail] = xNew;
    }
};
/*
    f[i][j]=min(f[i-1][k]+a[j]*(j-k)-(s[j]-s[k]))
    与k相关：
        斜率：-k
        截距：f[i-1][k]+s[k]
    求解方法：
        for(int i=1;i<=nFeed;i++){
            Hull hull;
            for(int j=1;j<=m;j++){
                f[i][j]=hull.get(a[j])+a[j]*j-s[j];
                if(i>1)hull.add(-j,f[i-1][j]+s[j]);
            }
        }
*/
int n, m, nFeed;
long long d[N], p[N], h[N], a[N], s[N], t[N];
long long f[110][N];
int main() {
    while (~scanf("%d%d%d", &n, &m, &nFeed)) {
        for (int i = 2; i <= n; i++) {
            scanf("%lld", &d[i]);
            p[i] = p[i - 1] + d[i];
        }
        for (int i = 1; i <= m; i++) {
            scanf("%lld%lld", &h[i], &t[i]);
            a[i] = t[i] - p[h[i]];
        }
        sort(a + 1, a + m + 1);
        for (int i = 1; i <= m; i++) s[i] = s[i - 1] + a[i];
        for (int i = 1; i <= nFeed; i++) {
            Hull hull;
            for (int j = 1; j <= m; j++) {
                f[i][j] = hull.get(a[j]) + a[j] * j - s[j];
                if (i > 1) hull.add(-j, f[i - 1][j] + s[j]);
            }
        }
        long long res = f[1][m];
        for (int i = 1; i <= nFeed; i++) res = min(res, f[i][m]);
        printf("%lld\n", res);
    }
    return 0;
}
