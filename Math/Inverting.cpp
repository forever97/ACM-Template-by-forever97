/*
    f(n)=∑[n|d]g(d)
    g(n)=∑[n|d]mu[d/n]f(d)
*/
//已知 g ,求 f
for (int i = 1; i <= n; ++i)
    for (int j = i + i; j <= n; j += i) f[i] += f[j];
//已知 f ,求 g
for (int i = n; i >= 1; --i)
    for (int j = i + i; j <= n; j += i) f[i] -= f[j];
/*
    f(n)=∑[d|n]g(d)
    g(n)=∑[d|n]mu[n/d]f(d)=∑[d|n]mu[d]f(n/d)
*/
//已知 g ,求 f
for (int i = n; i >= 1; --i)
    for (int j = i + i; j <= n; j += i) f[j] += f[i];
//已知 f ,求 g
for (int i = 1; i <= n; ++i)
    for (int j = i + i; j <= n; j += i) f[j] -= f[i];
/*
    Example1：
        ∑gcd(a[i],a[j],a[k],……)
        求子集gcd之和
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 1010, P = 1e8 + 7;
int T, n, x, f[N], pw[N];
int main() {
    for (int i = pw[0] = 1; i < N; i++) pw[i] = pw[i - 1] * 2 % P;
    scanf("%d", &T);
    while (T--) {
        memset(f, 0, sizeof(f));
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) {
            scanf("%d", &x);
            f[x]++;
        }
        n = 1000;
        for (int i = 1; i <= n; i++) {
            for (int j = i + i; j <= n; j += i) f[i] += f[j];
            f[i] = pw[f[i]] - 1;
        }
        for (int i = n; i; i--) {
            for (int j = i + i; j <= n; j += i) f[i] = (f[i] - f[j] + P) % P;
        }
        int ans = 0;
        for (int i = 1; i <= n; i++) ans = (ans + 1ll * i * f[i]) % P;
        printf("%d\n", ans);
    }
    return 0;
}
/*
    Example2：
        ∑∑gcd(a[i],a[j])*(gcd(a[i],a[j])-1)
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 10010, P = 10007;
int n, x, c[N], f[N];
int main() {
    while (~scanf("%d", &n)) {
        memset(c, 0, sizeof(c));
        memset(f, 0, sizeof(f));
        for (int i = 1; i <= n; i++) {
            scanf("%d", &x);
            c[x]++;
        }
        n = 10000;
        for (int i = 1; i <= n; i++) {
            for (int j = i; j <= n; j += i) f[i] += c[j];
            f[i] = f[i] * f[i] % P;
        }
        for (int i = n; i >= 1; --i)
            for (int j = i + i; j <= n; j += i) f[i] -= f[j];
        int ans = 0;
        for (int i = 2; i <= n; i++)
            ans = (ans + f[i] * i % P * (i - 1) % P) % P;
        printf("%d\n", ans);
    }
    return 0;
}
/*
    Example3：
        ∑∑∑∑gcd(a[i],a[j],a[k],a[l])
*/
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 10010;
int n, x, c[N];
long long f[N];
int main() {
    while (~scanf("%d", &n)) {
        memset(c, 0, sizeof(c));
        memset(f, 0, sizeof(f));
        for (int i = 1; i <= n; i++) {
            scanf("%d", &x);
            c[x]++;
        }
        n = 10000;
        for (int i = 1; i <= n; i++) {
            for (int j = i; j <= n; j += i) f[i] += c[j];
            if (f[i] > 3)
                f[i] = f[i] * (f[i] - 1) * (f[i] - 2) * (f[i] - 3) / 24;
            else
                f[i] = 0;
        }
        for (int i = n; i >= 1; --i)
            for (int j = i + i; j <= n; j += i) f[i] -= f[j];
        printf("%lld\n", f[1]);
    }
    return 0;
}
