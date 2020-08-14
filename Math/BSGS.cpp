/*
    BSGS
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <map>
#include <tr1/unordered_map>
using namespace std::tr1;
using namespace std;
typedef long long ll;
typedef pair<int, int> P;
int phi(int n) {
    int t = 1, i;
    for (i = 2; i * i <= n; i++)
        if (n % i == 0)
            for (n /= i, t *= i - 1; n % i == 0; n /= i, t *= i)
                ;
    if (n > 1) t *= n - 1;
    return t;
}
int pow(ll a, int b, int m) {
    ll t = 1;
    for (; b; b >>= 1, a = a * a % m)
        if (b & 1) t = t * a % m;
    return t;
}
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }
int exgcd(int a, int b, int& x, int& y) {
    if (!b) return x = 1, y = 0, a;
    int d = exgcd(b, a % b, x, y), t = x;
    return x = y, y = t - a / b * y, d;
}
int bsgs(int a, int r, int m) {
    if (r >= m) return -1;
    int i, g, x, c = 0, at = int(2 + sqrt(m));
    for (i = 0, x = 1 % m; i < 50; i++, x = ll(x) * a % m)
        if (x == r) return i;
    for (g = x = 1; __gcd(int(ll(x) * a % m), m) != g; c++)
        g = __gcd(x = ll(x) * a % m, m);
    if (r % g) return -1;
    if (x == r) return c;
    unordered_map<int, int> u;
    g = phi(m / g), u[x] = 0;
    g = pow(a, g - at % g, m);
    for (i = 1; i < at; i++) {
        u.insert(P(x = ll(x) * a % m, i));
        if (x == r) return c + i;
    }
    for (i = 1; i < at; i++) {
        unordered_map<int, int>::iterator t = u.find(r = ll(r) * g % m);
        if (t != u.end()) return c + i * at + t->second;
    }
    return -1;
}
// 计算 Y^Z Mod P 的值
void solve1(ll y, int z, int p) { printf("%d\n", pow(y, z, p)); }
// 计算满足 xy≡ Z ( mod P ) 的最小非负整数
void solve2(int y, int z, int p) {
    p = -p;
    int t = gcd(y, p);
    if (z % t) {
        puts("Orz, I cannot find x!");
        return;
    }
    y /= t;
    z /= t;
    p /= t;
    int a, b;
    exgcd(y, p, a, b);
    a = (ll)a * z % p;
    while (a < 0) a += p;
    printf("%d\n", a);
}
// 计算满足 Y^x ≡ Z ( mod P) 的最小非负整数
void solve3(int y, int z, int p) {
    y %= p;
    z %= p;
    int t = bsgs(y, z, p);
    if (t == -1) {
        puts("Orz, I cannot find x!");
        return;
    } else
        printf("%d\n", t);
}
int main() {
    int T, k, y, z, p;
    while (~scanf("%d%d", &T, &k)) {
        while (T--) {
            scanf("%d%d%d", &y, &z, &p);
            if (k == 1) solve1(y, z, p);
            if (k == 2) solve2(y, z, p);
            if (k == 3) solve3(y, z, p);
        }
    }
    return 0;
}
/*
    Problem:
        x_i=ax_{i-1}+b
        给定一个数字，问其最早在x序列的哪一位出现
    Solution:
        Ax+B=a(a(a……(ax+b)……+b)+b)+b
        我们记录k阶式子为Ax+B
        那么答案可以被分为y=A(A(A……(Ax+B)……+B)+B)+B
        和ans=a(a(a……(ay+b)……+b)+b)+b
        保存一个逆运算，BSGS即可
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> PI;
const int K = 1000000, mod = 1 << 22;
ll n;
int head[mod], cnt;
int x, a, b, P, A[K + 1], B[K + 1];
struct data {
    int x, nxt, s;
} g[mod];
inline int Hash(int x) { return (x + mod) & (mod - 1); }
void insert(int x, int y) {
    int key = Hash(x);
    for (int i = head[key]; i != -1; i = g[i].nxt) {
        if (g[i].x == x) return;
    }
    g[cnt].s = y;
    g[cnt].x = x;
    g[cnt].nxt = head[key];
    head[key] = cnt++;
}
int search(int x) {
    int key = Hash(x);
    for (int i = head[key]; i != -1; i = g[i].nxt) {
        if (g[i].x == x) return g[i].s;
    }
    return -1;
}
inline ll ask(int x) {
    for (int i = 0; i * K <= n && i * K <= P; i++) {
        int t = 1LL * (x - B[i] + P) * A[i] % P;
        auto it = search(t);
        if (it != -1) return (i * K + it) >= n ? -1 : i * K + it;
    }
    return -1;
}
ll pwmod(ll a, ll b, ll P) {
    ll res = 1;
    while (b) {
        if (b & 1) res = res * a % P;
        a = a * a % P;
        b >>= 1;
    }
    return res;
}
inline void init() {
    cnt = 0;
    memset(head, -1, sizeof(head));
    A[0] = 1, B[0] = 0;
    insert(x, 0);
    for (int i = 1; i <= K; i++) {
        x = (1ll * x * a + b) % P;
        insert(x, i);
        A[i] = 1ll * A[i - 1] * a % P;
        B[i] = (1ll * B[i - 1] * a + b) % P;
    }
    ll inv = pwmod(A[K], P - 2, P);
    for (int i = 1; 1ll * i * K <= P; i++) {
        A[i] = 1ll * A[i - 1] * inv % P;
        B[i] = (1ll * B[i - 1] * A[K] + B[K]) % P;
    }
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%lld%d%d%d%d", &n, &x, &a, &b, &P);
        init();
        int q;
        scanf("%d", &q);
        while (q--) {
            scanf("%d", &x);
            printf("%lld\n", ask(x));
        }
    }
    return 0;
}
