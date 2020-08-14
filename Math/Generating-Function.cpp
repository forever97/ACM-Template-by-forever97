/*
    Problem:
        构造一个长度为n(n \le 10^18)数列，每个数字属于[1,m](m \le 2*10^5)
        要求每个偶数出现次数均为偶数次
    Solution:
        多重集排列问题，用指数型生成函数处理
        (1+(x^1)/(1!)+(x^2)/(2!)+(x^3)/(3!)+……)
        本题的指数型生成函数为
        (1+(x^1)/(1!)+(x^2)/(2!)+(x^3)/(3!)+……)^((m+1)/2)*(1+(x^2)/(2!)+(x^4)/(4!)+(x^6)/(6!)+……)^(m/2)
        根据泰勒公式
        e^x=(1+(x^1)/(1!)+(x^2)/(2!)+(x^3)/(3!)+……)
        e^-x=(1-(x^1)/(1!)+(x^2)/(2!)-(x^3)/(3!)+……)
        得G(x)=(e^x)^((m+1)/2)*((e^x+e^-x)/2)^(m/2)
        第n项系数为\sum_{i=1}^{m/2}C(m/2, i)*(m-2*i)^n
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int P = 1e9 + 7;
const int U = 200000;
int f[U + 3], rf[U + 3];
ll inv(ll a, ll m) { return (a == 1 ? 1 : inv(m % a, m) * (m - m / a) % m); }
void init() {
    f[0] = 1;
    for (int i = 1; i <= U; i++) f[i] = (ll)f[i - 1] * i % P;
    rf[U] = inv(f[U], P);
    for (int i = U; i; i--) rf[i - 1] = (ll)rf[i] * i % P;
}
ll C(int n, int m) {
    if (m < 0 || m > n) return 0;
    return (ll)f[n] * rf[m] % P * rf[n - m] % P;
}
ll powmod(ll a, ll b, ll P) {
    ll t = 1;
    for (; b; b >>= 1, a = a * a % P)
        if (b & 1) t = t * a % P;
    return t;
}
int T, m;
ll n;
int main() {
    init();
    scanf("%d", &T);
    while (T--) {
        scanf("%lld%d", &n, &m);
        ll ans = 0;
        int t = m >> 1;
        for (int i = 0; i <= t; i++) {
            ans = (ans + C(t, i) * powmod(m - 2 * i, n, P) % P) % P;
        }
        ans = ans * powmod(powmod(2, t, P), P - 2, P) % P;
        printf("%lld\n", ans);
    }
    return 0;
}
/*
    Problem:
        给定F_1=1，F_n=a*F_{n-1}+1
        有一个有n个元素的多重集合S
        value(s)=F_{\sum_{s_i}}
        定义ans(k)=\sum_{s \in S \and |s|=k}value(s)
        求ans(1),ans(2),……,ans(n) mod 100003
        n \le 10^5，2 \le a \le 1000，1 \le s_i \le 1e9
    Solution：
        F_n=\frac{a^n-1}{a-1}
        对于给定的k，我们计算出不同方案下的a^sum之和，sum由k个s相加得到
        减去方案数C_n^k，然后除以(a-1)即可
        前者为母函数(x+a^{s_1})(x+a^{s_2})……(x+a^{s_n})的x^{n-k}前的系数
        所以我们计算这个母函数前的系数，fft计算即可
        我们可以通过分治区间优化多次等长数组的卷积，复杂度O(nlognlogn)
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N = 524300;
const int P = 100003;
#define double long double
int pos[N];
namespace FFT {
struct comp {
    double r, i;
    comp(double _r = 0, double _i = 0) : r(_r), i(_i) {}
    comp operator+(const comp &x) { return comp(r + x.r, i + x.i); }
    comp operator-(const comp &x) { return comp(r - x.r, i - x.i); }
    comp operator*(const comp &x) {
        return comp(r * x.r - i * x.i, i * x.r + r * x.i);
    }
    comp conj() { return comp(r, -i); }
} A[N], B[N];
const double pi = acos(-1.0);
void FFT(comp a[], int n, int t) {
    for (int i = 1; i < n; i++)
        if (pos[i] > i) swap(a[i], a[pos[i]]);
    for (int d = 0; (1 << d) < n; d++) {
        int m = 1 << d, m2 = m << 1;
        double o = pi * 2 / m2 * t;
        comp _w(cos(o), sin(o));
        for (int i = 0; i < n; i += m2) {
            comp w(1, 0);
            for (int j = 0; j < m; j++) {
                comp &A = a[i + j + m], &B = a[i + j], t = w * A;
                A = B - t;
                B = B + t;
                w = w * _w;
            }
        }
    }
    if (t == -1)
        for (int i = 0; i < n; i++) a[i].r /= n;
}
void mul(long long *a, long long *b, long long *c, int k) {
    int i, j;
    for (i = 0; i < k; i++) A[i] = comp(a[i], b[i]);
    j = __builtin_ctz(k) - 1;
    for (int i = 0; i < k; i++) {
        pos[i] = pos[i >> 1] >> 1 | ((i & 1) << j);
    }
    FFT(A, k, 1);
    for (int i = 0; i < k; i++) {
        j = (k - i) & (k - 1);
        B[i] = (A[i] * A[i] - (A[j] * A[j]).conj()) * comp(0, -0.25);
    }
    FFT(B, k, -1);
    for (int i = 0; i < k; i++) c[i] = (long long)(B[i].r + 0.5);
}
}  // namespace FFT
ll a[N], b[N], c[N];
int pw[N], s[N];
vector<int> v[N];
void solve(int x, int l, int r) {
    if (l == r) {
        v[x].push_back(1);
        v[x].push_back(pw[l]);
        return;
    }
    int mid = (l + r) >> 1;
    solve(x << 1, l, mid);
    solve(x << 1 | 1, mid + 1, r);
    int N = 1;
    while (N <= r - l + 1) N <<= 1;
    for (int i = 0; i < N; i++) a[i] = b[i] = 0;
    for (int i = 0; i <= mid - l + 1; i++) a[i] = v[x << 1][i];
    for (int i = 0; i <= r - mid; i++) b[i] = v[x << 1 | 1][i];
    FFT::mul(a, b, c, N);
    for (int i = 0; i <= r - l + 1; i++) v[x].push_back(c[i] % P);
}
namespace Comb {
const int U = P - 1;
int f[U + 3], rf[U + 3];
ll inv(ll a, ll m) { return (a == 1 ? 1 : inv(m % a, m) * (m - m / a) % m); }
void init() {
    f[0] = 1;
    for (int i = 1; i <= U; i++) f[i] = (ll)f[i - 1] * i % P;
    rf[U] = inv(f[U], P);
    for (int i = U; i; i--) rf[i - 1] = (ll)rf[i] * i % P;
}
ll C(int n, int m) {
    if (m < 0 || m > n) return 0;
    return (ll)f[n] * rf[m] % P * rf[n - m] % P;
}
}  // namespace Comb
ll powmod(ll a, ll b, ll P) {
    ll t = 1;
    for (; b; b >>= 1, a = a * a % P)
        if (b & 1) t = t * a % P;
    return t;
}
int n, A, k, q, ans[N];
int main() {
    Comb::init();
    scanf("%d%d%d", &n, &A, &q);
    for (int i = 1; i <= n; i++) scanf("%d", &s[i]);
    for (int i = 1; i <= n; i++) pw[i] = powmod(A, s[i], P);
    solve(1, 1, n);
    int t = powmod((A - 1 + P) % P, P - 2, P);
    for (int i = 1; i <= n; i++)
        ans[i] = 1ll * (v[1][i] - Comb::C(n, i) + P) % P * t % P;
    while (q--) {
        scanf("%d", &k);
        printf("%d\n", ans[k]);
    }
    return 0;
}
/*
    Problem：
        给定数组a，b_i=\sum_{j=i-kx}a_j，k等于1到3
        对于给定一个k做一次操作，用得到的数组b去替换a，
        问m次操作后得到的a数组是什么
    Solution:
        a数组的生成函数为a_0+a_1*x+a_2*x^2+……+a_n*x^n
        对于操作k相当于乘上生成函数1+x^k+x^(2k)+……x^(nk)
        多次操作k就相当于(1+x^k+x^(2k)+……x^(nk))^m
        x^(ik)前的系数是(1+x+x^2+……+x^n)^m展开的第i项的系数，
        x取0~1之间时(1+x+x^2+……+x^n)等于1/(1-x)，
        (1-x)^-m的第i项为C(m+i-1,i)*x^i
        则x^y的系数为C(m-1+y/k,m-1)，
        我们将每种操作分别考虑，得到系数，计算与a的卷积即可
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 1 << 19 | 7;
const int P = 998244353;
namespace NTT {
const int G = 3;
const int NUM = 20;
int wn[20];
int mul(int a, int b, int P) {
    return 1ll * a * b % P; 
}
int PowMod(int a, int b) {
    int res = 1;
    a %= P;
    while (b) {
        if (b & 1) res = mul(res, a, P);
        a = mul(a, a, P);
        b >>= 1;
    }
    return res;
}
void GetWn() {
    for (int i = 0; i < NUM; i++) {
        int t = 1 << i;
        wn[i] = PowMod(G, (P - 1) / t);
    }
}
void Change(int a[], int len) {
    int i, j, k;
    for (i = 1, j = len / 2; i < len - 1; i++) {
        if (i < j) swap(a[i], a[j]);
        k = len / 2;
        while (j >= k) {
            j -= k;
            k /= 2;
        }
        if (j < k) j += k;
    }
}
void NTT(int a[], int len, int on) {
    Change(a, len);
    int id = 0;
    for (int h = 2; h <= len; h <<= 1) {
        id++;
        for (int j = 0; j < len; j += h) {
            int w = 1;
            for (int k = j; k < j + h / 2; k++) {
                int u = a[k] % P;
                int t = mul(a[k + h / 2], w, P);
                a[k] = (u + t) % P;
                a[k + h / 2] = ((u - t) % P + P) % P;
                w = mul(w, wn[id], P);
            }
        }
    }
    if (on == -1) {
        for (int i = 1; i < len / 2; i++) swap(a[i], a[len - i]);
        int inv = PowMod(len, P - 2);
        for (int i = 0; i < len; i++) a[i] = mul(a[i], inv, P);
    }
}
void MUL(int *a, int *b, int n) {
    int len = 1;
    while (len < n * 2) len <<= 1;
    fill(a + n, a + len, 0);
    NTT::NTT(a, len, 1);
    fill(b + n, b + len, 0);
    NTT::NTT(b, len, 1);
    for (int i = 0; i < len; i++) a[i] = 1ll * a[i] * b[i] % P;
    NTT::NTT(a, len, -1);
}
}  // namespace NTT
namespace Comb {
const int U = 2000000;
int f[U + 3], rf[U + 3];
LL inv(LL a, LL m) { return (a == 1 ? 1 : inv(m % a, m) * (m - m / a) % m); }
void init() {
    f[0] = 1;
    for (int i = 1; i <= U; i++) f[i] = (LL)f[i - 1] * i % P;
    rf[U] = inv(f[U], P);
    for (int i = U; i; i--) rf[i - 1] = (LL)rf[i] * i % P;
}
LL C(int n, int m) {
    if (m < 0 || m > n) return 0;
    return (LL)f[n] * rf[m] % P * rf[n - m] % P;
}
}  // namespace Comb
int a[N], b[N], c[4];
void getc(int *a, int k, int t, int n) {
    fill(a, a + n, 0);
    for (int i = 0; i < n; i += k) a[i] = Comb::C(i / k + t - 1, t - 1);
}
int T, n, m;
int main() {
    scanf("%d", &T);
    Comb::init();
    NTT::GetWn();
    while (T--) {
        scanf("%d%d", &n, &m);
        memset(c, 0, sizeof(c));
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
            a[i] %= P;
        }
        for (int i = 1; i <= m; i++) {
            int k;
            scanf("%d", &k);
            c[k]++;
        }
        for (int i = 1; i <= 3; i++) {
            if (!c[i]) continue;
            getc(b, i, c[i], n);
            NTT::MUL(a, b, n);
        }
        long long ans = 0;
        for (int i = 0; i < n; i++) ans ^= ((i + 1ll) * a[i]);
        printf("%lld\n", ans);
    }
    return 0;
}
