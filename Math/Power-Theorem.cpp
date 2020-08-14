/*
    素数唯一分解定理
*/
const int M = 50010;
const int mod = 1e9 + 7;
int a[M], p[M], pNum, Num[M];
bool f[M];
void Prime() {
    int i, j;
    for (i = 2; i < M; i++) {
        if (!f[i]) {
            p[pNum++] = i;
        }
        for (j = 0; j < pNum && p[j] * i < M; j++) {
            f[p[j] * i] = 1;
            if (!(i % p[j])) break;
        }
    }
}
/*
    用于计算对于每个素数，在n的阶乘里的幂次
*/
void Power(int n, int a[]) {
    for (int i = 0; i < pNum; i++) {
        if (p[i] > n) break;
        int t = p[i];
        while (t <= n) {
            a[i] += n / t;
            t = t * p[i];
        }
    }
}
/*
    求一个数的阶乘有几个约数
*/
int T, n;
int main() {
    Prime();
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        memset(a, 0, sizeof(a));
        Power(n, a);
        int ans = 1;
        for (int i = 0; i < pNum; i++) {
            if (p[i] > n) break;
            ans = 1ll * ans * (a[i] + 1) % mod;
        }
        printf("%d\n", ans);
    }
    return 0;
}