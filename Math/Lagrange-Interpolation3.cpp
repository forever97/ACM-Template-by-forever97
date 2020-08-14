/*
    拉格朗日插值+FFT
    已知f(0)~f(n)求解f(m)~f(m+n)
*/
#include <bits/stdc++.h>
typedef unsigned long long ull;
const int maxN = 100005;
const int maxL = 1 << 18 | 1;
const int Mod = 998244353;
int fexp(int base, int exp) {
    int res = 1;
    for (; exp; exp >>= 1, base = (ull)base * base % Mod)
        if (exp & 1) res = (ull)res * base % Mod;
    return res;
}
int Rev[maxL], Wn[maxL];
void DFT_pre(int l, int bitcnt) {
    for (int i = 1; i != l; ++i)
        Rev[i] = (Rev[i >> 1] >> 1) | ((i & 1) << (bitcnt - 1));
}
void DFT(int *a, int n) {
    for (int i = 1; i != n; ++i)
        if (i < Rev[i]) std::swap(a[i], a[Rev[i]]);
    for (int l = 1; l != n; l <<= 1) {
        int w = fexp(3, (Mod - 1) / (l << 1));
        Wn[0] = 1;
        for (int i = 1; i != l; ++i) Wn[i] = (ull)Wn[i - 1] * w % Mod;
        for (int i = 0; i != n; i += l << 1)
            for (int j = 0; j != l; ++j) {
                int x = a[i + j], y = (ull)Wn[j] * a[i + l + j] % Mod;
                a[i + j] = (x + y) % Mod;
                a[i + l + j] = (x + Mod - y) % Mod;
            }
    }
}
void iDFT(int *a, int n) {
    for (int i = 1; i != n; ++i)
        if (i < Rev[i]) std::swap(a[i], a[Rev[i]]);
    for (int l = 1; l != n; l <<= 1) {
        int w = fexp((Mod + 1) / 3, (Mod - 1) / (l << 1));
        Wn[0] = 1;
        for (int i = 1; i != l; ++i) Wn[i] = (ull)Wn[i - 1] * w % Mod;
        for (int i = 0; i != n; i += l << 1)
            for (int j = 0; j != l; ++j) {
                int x = a[i + j], y = (ull)Wn[j] * a[i + l + j] % Mod;
                a[i + j] = (x + y) % Mod;
                a[i + l + j] = (x + Mod - y) % Mod;
            }
    }
    for (int i = 0, inv = fexp(n, Mod - 2); i != n; ++i)
        a[i] = (ull)a[i] * inv % Mod;
}
int N, M, F[maxN];
int Fac[maxN], invFac[maxN];
int A[maxL], B[maxL];
int main() {
    scanf("%d%d", &N, &M);
    for (int i = 0; i <= N; ++i) scanf("%d", F + i);
    Fac[0] = 1;
    for (int i = 1; i <= N; ++i) Fac[i] = (ull)Fac[i - 1] * i % Mod;
    invFac[N] = fexp(Fac[N], Mod - 2);
    for (int i = N; i; --i) invFac[i - 1] = (ull)invFac[i] * i % Mod;
    for (int i = 0; i <= N; ++i) {
        A[i] = (ull)invFac[i] * invFac[N - i] % Mod;
        if ((N - i) & 1) A[i] = Mod - A[i];
        A[i] = (ull)A[i] * F[i] % Mod;
    }
    int l = 1, bitcnt = 0;
    while (l <= N + N) l <<= 1, ++bitcnt;
    for (int i = -N; i <= N; ++i) B[(l + i) & (l - 1)] = fexp(M + i, Mod - 2);
    DFT_pre(l, bitcnt);
    DFT(A, l);
    DFT(B, l);
    for (int i = 0; i != l; ++i) A[i] = (ull)A[i] * B[i] % Mod;
    iDFT(A, l);
    int t = 1;
    for (int i = 0; i <= N; ++i) t = (ull)t * (M - i) % Mod;
    for (int i = 0; i <= N; ++i) {
        printf("%llu%c", (ull)t * A[i] % Mod, " \n"[i == N]);
        t = (ull)t * (M + i + 1) % Mod;
        t = (ull)t * fexp(M + i - N, Mod - 2) % Mod;
    }
    return 0;
}