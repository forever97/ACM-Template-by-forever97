/*
    mu[i]: 莫比乌斯函数 
    phi[i]: 欧拉函数 
    d[i]: i的质因子个数
*/
bool notp[N];
int prime[N], pnum, mu[N], c[N], d[N], phi[N];
void sieve() {
    memset(notp, 0, sizeof(notp));
    notp[0] = notp[1] = mu[1] = phi[1] = 1;
    pnum = 0;
    for (int i = 2; i < N; i++) {
        if (!notp[i]) {
            prime[++pnum] = i;
            mu[i] = -1;
            phi[i] = i - 1;
            d[i] = 1;
        }
        for (int j = 1; j <= pnum && prime[j] * i < N; j++) {
            notp[prime[j] * i] = 1;
            d[prime[j] * i] = d[i] + 1;
            if (i % prime[j] == 0) {
                mu[prime[j] * i] = 0;
                phi[i * prime[j]] = phi[i] * prime[j];
                break;
            }
            mu[prime[j] * i] = -mu[i];
            phi[i * prime[j]] = phi[i] * (prime[j] - 1);
        }
    }
}