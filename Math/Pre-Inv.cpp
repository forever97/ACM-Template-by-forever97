/*
    预处理的对于模P的逆元
*/
int inv[N], mp[N];
void pre_inv() {
    inv[1] = 1;
    for (int i = 2; i < P; i++) {
        int a = P / i, b = P % i;
        inv[i] = (1LL * inv[b] * (-a) % P + P) % P;
    }
}