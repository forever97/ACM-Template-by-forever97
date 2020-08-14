/*
    任意模FFT
    mod(1e8~1e9级别)
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
typedef double db;
#define upmo(a, b) \
    (((a) = ((a) + (b)) % mod) < 0 ? (a) += mod : (a))  // 相加后取模
int n, mod;
//前面需要有 mod(1e8~1e9级别),upmo(a,b) 的定义
namespace FFT_MO {
const int FFT_MAXN = 1 << 18;
const db pi = 3.14159265358979323846264338327950288L;
struct cp {
    db a, b;
    cp(double a_ = 0, double b_ = 0) { a = a_, b = b_; }
    cp operator+(const cp &rhs) const { return cp(a + rhs.a, b + rhs.b); }
    cp operator-(const cp &rhs) const { return cp(a - rhs.a, b - rhs.b); }
    cp operator*(const cp &rhs) const {
        return cp(a * rhs.a - b * rhs.b, a * rhs.b + b * rhs.a);
    }
    cp operator!() const { return cp(a, -b); }
} nw[FFT_MAXN + 1], f[FFT_MAXN], g[FFT_MAXN], t[FFT_MAXN];  // a<->f,b<->g,t<~>c
int bitrev[FFT_MAXN];
//初始化 nw[],bitrev[]
void fft_init() {
    int L = 0;
    while ((1 << L) != FFT_MAXN) L++;
    for (int i = 1; i < FFT_MAXN; i++)
        bitrev[i] = bitrev[i >> 1] >> 1 | ((i & 1) << (L - 1));
    for (int i = 0; i <= FFT_MAXN; i++)
        nw[i] = cp((db)cosl(2 * pi / FFT_MAXN * i),
                   (db)sinl(2 * pi / FFT_MAXN * i));
}
// n已保证是2的整数次幂
// flag=1:DFT |  flag=-1: IDFT
void dft(cp *a, int n, int flag = 1) {
    int d = 0;
    while ((1 << d) * n != FFT_MAXN) d++;
    for (int i = 0; i < n; i++)
        if (i < (bitrev[i] >> d)) swap(a[i], a[bitrev[i] >> d]);  // NOTICE!
    for (int l = 2; l <= n; l <<= 1) {
        int del =
            FFT_MAXN / l *
            flag;  // 决定 wn是在复平面是顺时针还是逆时针变化，以及变化间距
        for (int i = 0; i < n; i += l) {
            cp *le = a + i, *ri = a + i + (l >> 1);
            cp *w = flag == 1 ? nw : nw + FFT_MAXN;  // 确定wn的起点
            for (int k = 0; k < (l >> 1); k++) {
                cp ne = *ri * *w;
                *ri = *le - ne, *le = *le + ne;
                le++, ri++, w += del;
            }
        }
    }
    if (flag != 1)
        for (int i = 0; i < n; i++) a[i].a /= n, a[i].b /= n;
}
// convo(a,n,b,m,c) a[0..n]*b[0..m] -> c[0..n+m]
void convo(LL *a, int n, LL *b, int m, LL *c) {
    for (int i = 0; i <= n + m; i++) c[i] = 0;
    int N = 2;
    while (N <= n + m) N <<= 1;  // N+1是c扩展后的长度
    //扩展 a[],b[],存入f[],g[],注意取模
    for (int i = 0; i < N; i++) {
        LL aa = i <= n ? a[i] : 0, bb = i <= m ? b[i] : 0;
        aa %= mod, bb %= mod;
        f[i] = cp(db(aa >> 15), db(aa & 32767));
        g[i] = cp(db(bb >> 15), db(bb & 32767));
    }
    dft(f, N), dft(g, N);
    // 频域求积
    for (int i = 0; i < N; i++) {
        int j = i ? N - i : 0;
        t[i] = ((f[i] + !f[j]) * (!g[j] - g[i]) +
                (!f[j] - f[i]) * (g[i] + !g[j])) *
               cp(0, 0.25);
    }
    dft(t, N, -1);
    for (int i = 0; i <= n + m; i++) upmo(c[i], (LL(t[i].a + 0.5)) % mod << 15);
    // 频域求积
    for (int i = 0; i < N; i++) {
        int j = i ? N - i : 0;
        t[i] = (!f[j] - f[i]) * (!g[j] - g[i]) * cp(-0.25, 0) +
               cp(0, 0.25) * (f[i] + !f[j]) * (g[i] + !g[j]);
    }
    dft(t, N, -1);
    for (int i = 0; i <= n + m; i++)
        upmo(c[i], LL(t[i].a + 0.5) + (LL(t[i].b + 0.5) % mod << 30));
}
}  // namespace FFT_MO