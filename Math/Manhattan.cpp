/*
    曼哈顿最大距离
    D表示维度
    a[n][D]为D维点坐标
*/
#define inf 1e100
const int D = 2;
int n;
double a[N][D];
void solve() {
    double ans = 0, mi, mx, t;
    for (int s = 0; s < (1 << D); s++) {
        mi = inf, mx = -inf;
        for (int i = 0; i < n; i++) {
            t = 0;
            for (int j = 0; j < D; j++)
                if ((1 << j) & s)
                    t += a[i][j];
                else
                    t -= a[i][j];
            mi = min(mi, t);
            mx = max(mx, t);
        }
        ans = max(ans, mx - mi);
    }
    printf("%.0lf\n", ans);
}
/*
    Problem:
        有n个主武器和m个副武器，分别有个S值和k个x值，x1,x2……，xk。
        请选取一个主武器和一个副武器，使得Sz+Sf+∑|Xz-Xf|最大
    Solution:
        我们将其改为求|Sz-0|+|Sf-0|+∑|Xz-Xf|，
        那么问题就转化为从两个集合中各选取一个k+2维点，使其k+2维曼哈顿距离最大
        我们加入1e15作为集合识别，将其转化为k+3维曼哈顿问题
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll inf = 1e18, base = 1e15;
const int N = 300010;
const int D = 8;
int T, n, m, k;
ll a[N][D];
void solve() {
    ll ans = 0, mi, mx, t;
    for (int s = 0; s < (1 << k); s++) {
        mi = inf, mx = -inf;
        for (int i = 0; i < n + m; i++) {
            t = 0;
            for (int j = 0; j < k; j++)
                if ((1 << j) & s)
                    t += a[i][j];
                else
                    t -= a[i][j];
            mi = min(mi, t);
            mx = max(mx, t);
        }
        ans = max(ans, mx - mi);
    }
    printf("%lld\n", ans - base);
}
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d%d", &n, &m, &k);
        k += 3;
        for (int i = 0; i < n + m; i++) {
            if (i < n) {
                a[i][0] = base;
                scanf("%lld", &a[i][1]);
                a[i][2] = 0;
            } else {
                a[i][0] = 0;
                scanf("%lld", &a[i][2]);
                a[i][1] = 0;
            }
            for (int j = 3; j < k; j++) scanf("%lld", &a[i][j]);
        }
        solve();
    }
    return 0;
}