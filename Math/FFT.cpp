/*
    FFT
    int N=1; while(N<len)N<<=1;
    不带模 FFT::mul(A,B,C,N);
    带模 FFT::mulmod(A,B,C,N);
*/
typedef long long LL;
const int N = 524300;
int n, pos[N];
namespace FFT {
const int P = MOD;
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
void mulmod(int *a, int *b, int *c, int k) {
    int i, j;
    for (i = 0; i < k; i++) a0[i] = a[i] / M, b0[i] = b[i] / M;
    j = __builtin_ctz(k) - 1;
    for (int i = 0; i < k; i++) {
        pos[i] = pos[i >> 1] >> 1 | ((i & 1) << j);
    }
    for (mul(a0, b0, a0), i = 0; i < k; i++) {
        c[i] = 1LL * a0[i] * M * M % P;
        a1[i] = a[i] % M, b1[i] = b[i] % M;
    }
    for (mul(a1, b1, a1), i = 0; i < k; i++) {
        c[i] = (a1[i] + c[i]) % P, a0[i] = (a0[i] + a1[i]) % P;
        a1[i] = a[i] / M + a[i] % M, b1[i] = b[i] / M + b[i] % M;
    }
    for (mul(a1, b1, a1), i = 0; i < k; i++)
        c[i] = (1LL * M * (a1[i] - a0[i] + P) + c[i]) % P;
}
}  // namespace FFT
