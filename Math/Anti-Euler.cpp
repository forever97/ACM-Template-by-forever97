/*
    反欧拉函数
    给定n(n≤10^12)求最小的满足φ(x)=n的x
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
using namespace std;
typedef long long LL;
using namespace std;
int const MxCnt = 1e4;
LL Y, Ans, Cnt, Pri[MxCnt + 10];
bool CPri(LL x) {
    int Mx = sqrt(x);
    for (int i = 2; i <= Mx; i++)
        if (x % i == 0) return 0;
    return 1;
}
void Prep(LL x) {
    Cnt = 0;
    int Mx = sqrt(x);
    for (int i = 1; i <= Mx; i++)
        if (x % i == 0) {
            if (CPri(i + 1)) Pri[++Cnt] = i + 1;
            if (CPri(x / i + 1)) Pri[++Cnt] = x / i + 1;
        }
}
bool Cmp(LL X, LL Y) { return X > Y; }
void Dfs(int Pos, LL Y, LL X) {
    if (Y == 1) Ans = min(Ans, X);
    if ((Pos > Cnt) || (Y == 1) || (X > Ans)) return;
    for (LL i = Pri[Pos] - 1; Y % i == 0; i *= Pri[Pos])
        Dfs(Pos + 1, Y / i, X / (Pri[Pos] - 1) * Pri[Pos]);
    Dfs(Pos + 1, Y, X);
}
int T;
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%lld", &Y);
        Prep(Y);
        sort(Pri + 1, Pri + Cnt + 1, Cmp);
        Ans = (Y == 1) ? Y : Y * 10;
        Dfs(1, Y, Y);
        printf("%lld\n", Ans);
    }
    return 0;
}
/*
    给定n(n≤10^10)求所有满足φ(x)=n的x
*/
#include <algorithm>
#include <cstdio>
using namespace std;
typedef long long ll;
const int N = 1000000, M = 5000;
int T, lim, m, d, cnt, i, j, p[N / 10], tot, small[N], big[N], g[M][700];
bool v[N];
ll n, a[M], b[M], q[N];
bool check(ll n) {
    if (n < N) return !v[n];
    for (int i = 2; 1LL * i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}
void dfs(int x, ll S, ll p) {
    if (p == 1) {
        q[cnt++] = S;
        return;
    }
    x = g[p <= lim ? small[p] : big[n / p]][x];
    if (x == m) return;
    dfs(x + 1, S, p);
    for (dfs(x + 1, S *= a[x], p /= a[x] - 1); p % a[x] == 0;
         dfs(x + 1, S *= a[x], p /= a[x]))
        ;
}
int main() {
    for (i = 2; i < N; i++) {
        if (!v[i]) p[tot++] = i;
        for (j = 0; j < tot && i * p[j] < N; j++) {
            v[i * p[j]] = 1;
            if (i % p[j] == 0) break;
        }
    }
    scanf("%d", &T);
    while (T--) {
        scanf("%lld", &n);
        if (n == 1) {
            puts("2\n1 2");
            continue;
        }
        for (lim = 1; 1LL * (lim + 1) * (lim + 1) <= n; lim++)
            ;
        for (cnt = m = d = 0, i = 1; i <= lim; i++)
            if (n % i == 0) {
                if (check(i + 1)) a[m++] = i + 1;
                b[d++] = i;
                if (1LL * i * i != n) {
                    if (check(n / i + 1)) a[m++] = n / i + 1;
                    b[d++] = n / i;
                }
            }
        std::sort(a, a + m), std::sort(b, b + d);
        for (i = 0; i < d; i++) {
            if (b[i] <= lim)
                small[b[i]] = i;
            else
                big[n / b[i]] = i;
            for (g[i][m] = m, j = m - 1; ~j; j--)
                g[i][j] = b[i] % (a[j] - 1) ? g[i][j + 1] : j;
        }
        dfs(0, 1, n);
        std::sort(q, q + cnt);
        printf("%d\n", cnt);
        if (cnt)
            for (printf("%lld", q[0]), i = 1; i < cnt; i++)
                printf(" %lld", q[i]);
        puts("");
    }
    return 0;
}
