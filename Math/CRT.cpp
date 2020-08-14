/*
    中国剩余定理
    n个同余方程，第i个为x≡b[i](mod a[i])，要求a[i]两两互质
*/
LL CRT(LL* a, LL* b, int n) {
    LL ans, P = 1;
    for (int i = 0; i < n; i++) P *= a[i];
    for (int i = 0; i < n; i++)
        ans =
            (ans + (P / a[i]) * pow(P / a[i], a[i] - 2, a[i]) % P * b[i] % P) %
            P;
    while (ans < 0) ans += P;
    return ans;
}