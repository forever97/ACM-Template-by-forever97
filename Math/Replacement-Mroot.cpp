/*
    置换开m次方
    长度为n的置换开m次方，solve返回值表示能否表示为m次方，
    ans数组保存开方后的结果
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 1000010;
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }
int cal(int x, int m) {
    int i, k = m, t = 1;
    for (i = 2; i * i <= x; i++)
        if (x % i == 0) {
            while (x % i == 0) x /= i, t *= i;
            while (k % i == 0) k /= i, t *= i;
        }
    if (x > 1)
        for (t *= x; k % x == 0; k /= x, t *= x)
            ;
    return t;
}
bool v[N];
int a[N], g[N], nxt[N], q[N], b[N], ans[N];
bool solve(int n, int m) {
    memset(v, 0, sizeof(v));
    memset(g, 0, sizeof(g));
    int x, i, j, k, o, l, t;
    for (i = 1; i <= n; i++)
        if (!v[i]) {
            t = v[i] = 1;
            for (int j = a[i]; j != i; j = a[j]) v[j] = 1, t++;
            nxt[i] = g[t], g[t] = i;
        }
    for (i = 1; i <= n; i++)
        if (g[i]) {
            for (t = 0, j = g[i]; j; j = nxt[j]) q[++t] = j;
            int d = gcd(l = cal(i, m), m);
            if (t % d) return 0;
            for (x = 1; x <= t; x += d) {
                for (j = 0; j < d; j++)
                    for (k = 0, o = q[x + j]; k < i; k++, o = a[o])
                        b[(j + 1LL * k * m) % l] = o;
                for (j = 0; j < l; j++) ans[b[j]] = b[(j + 1) % l];
            }
        }
    return 1;
}
/*
    判断给出置换能否开方
*/
int T, n, m;
char s[30];
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%s", s + 1);
        for (int i = 1; i <= 26; i++) a[i] = s[i] - 'A' + 1;
        int ans = solve(26, 2);
        if (ans)
            puts("Yes");
        else
            puts("No");
    }
    return 0;
}
