/*
    欧拉函数
    对正整数n，欧拉函数是少于或等于n的数中与n互质的数的数目。
    例如Euler(8)=4，因为1,3,5,7均和8互质。
    通式：
        对于一个正整数N的素数幂分解
            N=(P1^q1)*(P2^q2)*...*(Pn^qn).
              phi(1)=1.
              phi(N)=N*(1-1/P1)*(1-1/P2)*...*(1-1/Pn).
    定理：
        1.欧拉函数是非完全积性函数: phi(m*n)=phi(n)*phi(m)
        当且仅当GCD(n,m)=1.
        2.一个数的所有质因子之和是Euler(n)*n/2.
        3.若n是素数p的k次幂，phi(n)=p^k-p^(k-1)=(p-1)p^(k-1)，
        因为除了p的倍数外，其他数都跟n互质.
    特殊性质：
        1.当n为奇数时，phi(2n)=phi(n).
        2.对于质数p，phi(p)=p-1
        3.除了N=2,phi(N)都是偶数.
    指数循环节：
        A^x=A^(x%phi(C)+phi(C))(mod C)(x>=phi(C))
*/
// 筛法求欧拉函数
void Get_Euler() {
    for (int i = 1; i <= n; i++) phi[i] = i;
    for (int i = 2; i <= n; i++)
        if (phi[i] == i)
            for (int j = i; j <= n; j += i) phi[j] = phi[j] / i * (i - 1);
}
int Euler(int n) {
    int t = 1, i;
    if (!(n & 1))
        for (n >>= 1; !(n & 1); n >>= 1, t <<= 1)
            ;
    for (i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            for (n /= i, t *= i - 1; n % i == 0; n /= i, t *= i)
                ;
    if (n > 1) t *= n - 1;
    return t;
}
/*
    求出∑gcd(i,N)(1<=i<=N)
    ∑_{i=1}^{N}gcd(i,N)
    =∑_{i=1}^{N}∑_{d|gcd(i,N)}\phi(d)
    =∑\phi(d)∑_{1=<i<=N&&d|i&&d|N}1
    =∑_{d|N}\phi(d)\floorl\frac{i}{d}\floorr
*/
int main() {
    int n;
    long long ans = 0;
    while (~scanf("%d", &n)) {
        for (int i = 1; i * i <= n; i++) {
            if (n % i == 0) {
                ans += 1LL * i * Euler(n / i);
                if (i * i < n) ans += 1LL * (n / i) * Euler(i);
            }
        }
        printf("%lld\n", ans);
    }
    return 0;
}
/*
    f(0) = 1
    f(n) = (n%10)^f(n/10)
    求f(n) % m
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll; 
int phi(int n) {
    int ans = n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            ans = ans / i * (i - 1);
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) ans = ans / n * (n - 1);
    return ans;
}
int pow(ll x, ll y, ll p) {
    ll cnt = 1, f1 = 0, f2 = 0;
    for (; y; y >>= 1) {
        if (y & 1) f1 |= (cnt * x >= p || f2), cnt = cnt * x % p;
        f2 |= (x * x >= p);
        x = x * x % p;
    }
    return cnt + f1 * p;
}
int T, n, m;
int Cal(int x, int p) {
    if (x == 0) return 1;
    return pow(x % 10, Cal(x / 10, phi(p)), p);
}
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        printf("%d\n", Cal(n, m) % m);
    }
    return 0;
}
/*
    Problem:
        求a1^a2^a3^……^an%m（^表示幂）
    Solution:
        b>=phi(p)时有a^b%p=a^(b%phi(p)+phi(p1))%p
        递归处理，每次取phi(p)，试乘判断是否超过phi(p)
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int read() {
    int x = 0, z = 1;
    char c = getchar();
    for (; c < '0' || c > '9'; c = getchar()) {
        if (c == '#') exit(0);
        z = c == '-' ? -1 : 1;
    }
    for (; c >= '0' && c <= '9'; c = getchar())
        x = (x << 1) + (x << 3) + (c ^ 48);
    return x * z;
}
int phi(int n) {
    int ans = n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            ans = ans / i * (i - 1);
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) ans = ans / n * (n - 1);
    return ans;
}
int pow(ll x, ll y, ll p) {
    ll cnt = 1, f1 = 0, f2 = 0;
    for (; y; y >>= 1) {
        if (y & 1) f1 |= (cnt * x >= p || f2), cnt = cnt * x % p;
        f2 |= (x * x >= p);
        x = x * x % p;
    }
    return cnt + f1 * p;
}
const int N = 15;
int n, m, a[N];
int Cal(int x, int p) {
    if (x == n || p == 2) return pow(a[x], 1, p);
    return pow(a[x], Cal(x + 1, phi(p)), p);
}
int main() {
    for (int T = 1;; T++) {
        m = read(), n = read();
        for (int i = 1; i <= n; i++) a[i] = read();
        printf("Case #%d: %d\n", T, Cal(1, m) % m);
    }
    return 0;
}
