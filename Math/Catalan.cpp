/*
    卡特兰数&&组合数
    打表计算Fact
    n,m,MOD数量级1e9
*/
/*--------------------Make Fact Table----------------------*/
typedef long long LL;
const LL MOD = 1000000007;
LL res = 1;
int main() {
    freopen("1.txt", "w", stdout);
    for (int i = 1; i <= 1000000000; i++) {
        res = res * i % MOD;
        if (i % 500000 == 0) printf(",%d", res);
    }
    return 0;
}
/*-------------------Calculate Catalan---------------------*/
typedef long long LL;
const LL MOD = 1000000007, blk = 500000;
const LL lst[2001] = {……};  //填入打出的Fact表
int T, n, s;
char ss[1000010];
LL Fact(LL x) {
    if (x < 0) return 0;
    LL res = lst[x / blk];
    for (LL i = (x / blk) * blk + 1; i <= x; i++) res = (res * i) % MOD;
    return res;
}
LL power(LL a, LL b) {
    if (b == 0) return 1;
    if (b & 1) return (power(a, b - 1) * a) % MOD;
    LL t = power(a, b / 2);
    return (t * t) % MOD;
}
LL C(LL a, LL b) {
    if (a < 0 || b < 0 || a < b) return 0;
    return (Fact(a) * power(Fact(a - b), MOD - 2) % MOD *
            power(Fact(b), MOD - 2)) %
           MOD;
}
LL Catalan(int x) { return (C(2 * x, x) - C(2 * x, x - 1) + MOD) % MOD; }
