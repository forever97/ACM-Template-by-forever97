/*
    特征多项式+快速幂加速递推
    a[n]=∑{1,m}_c[m-i]a[n-i],给定a[0],a[1],...,a[m−1],求a[n]
*/
#include <algorithm>
#include <cstdio>
using namespace std;
typedef long long ll;
const int M = 2000, P = 1000000007;
int n, m, i, j, x, w, b, t, a[M], c[M], v[M], u[M << 1], ans;
int main() {
    scanf("%d%d", &n, &m);
    for (i = m - 1; ~i; i--) scanf("%d", &c[i]), c[i] = (c[i] % P + P) % P;
    for (i = 0; i < m; i++) scanf("%d", &a[i]), a[i] = (a[i] % P + P) % P;
    for (i = 0; i < m; i++) v[i] = 1;
    for (w = !!n, i = n; i > 1; i >>= 1) w <<= 1;
    for (x = 0; w; copy(u, u + m, v), w >>= 1, x <<= 1) {
        fill_n(u, m << 1, 0), b = !!(n & w), x |= b;
        if (x < m)
            u[x] = 1;
        else {
            for (i = 0; i < m; i++)
                for (j = 0, t = i + b; j < m; j++, t++)
                    u[t] = ((ll)v[i] * v[j] + u[t]) % P;
            for (i = (m << 1) - 1; i >= m; i--)
                for (j = 0, t = i - m; j < m; j++, t++)
                    u[t] = ((ll)c[j] * u[i] + u[t]) % P;
        }
    }
    for (i = 0; i < m; i++) ans = ((ll)v[i] * a[i] + ans) % P;
    printf("%d", (ans + P) % P);
    return 0;
}
/*
    Example
    f[i]=f[i-1]-f[i-2]
    给出前两项，求n项
*/
#include <algorithm>
#include <cstdio>
using namespace std;
typedef long long ll;
const int M = 2000, P = 1000000007;
int n, m, i, j, x, w, b, t, a[M], c[M], v[M], u[M << 1], ans;
int main() {
    c[1] = 1;
    c[0] = P - 1;
    m = 2;  //自己填写参数的时候注意也要对P取模
    for (i = 0; i < m; i++) scanf("%d", &a[i]), a[i] = (a[i] % P + P) % P;
    for (i = 0; i < m; i++) v[i] = 1;
    scanf("%d", &n);
    n--;
    if (n < 2) {  //小于给出项数的特判
        printf("%d\n", a[n]);
        return 0;
    }
    for (w = !!n, i = n; i > 1; i >>= 1) w <<= 1;
    for (x = 0; w; copy(u, u + m, v), w >>= 1, x <<= 1) {
        fill_n(u, m << 1, 0), b = !!(n & w), x |= b;
        if (x < m)
            u[x] = 1;
        else {
            for (i = 0; i < m; i++)
                for (j = 0, t = i + b; j < m; j++, t++)
                    u[t] = ((ll)v[i] * v[j] + u[t]) % P;
            for (i = (m << 1) - 1; i >= m; i--)
                for (j = 0, t = i - m; j < m; j++, t++)
                    u[t] = ((ll)c[j] * u[i] + u[t]) % P;
        }
    }
    for (i = 0; i < m; i++) ans = ((ll)v[i] * a[i] + ans) % P;
    printf("%d\n", (ans + P) % P);
    return 0;
}