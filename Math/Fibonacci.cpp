/*
    斐波那契数列
    Fib(n)=Fib(n-1)+Fib(n-2)
    S(n)=SumFib
    S(n)+1=Fib(n+2)
    gcd(Fib(n),Fib(m))=Fib(gcd(n,m))
*/
/*
    log求fib
    x为第n位fib
*/
void fib(LL n, LL& x, LL& y) {
    if (n == 0) {
        x = 0;
        y = 1;
        return;
    }
    if (n & 1) {
        fib(n - 1, y, x);
        y = (y + x) % mod;
    } else {
        LL a, b;
        fib(n >> 1, a, b);
        y = (a * a + b * b) % mod;
        x = (a * b + a * (b - a + mod)) % mod;
    }
}
/*
    log求fib
    记忆化版本
*/
typedef long long LL;
const LL M = 1e9 + 7;
map<LL, LL> Fib;
LL fib(LL n) {
    LL k = n / 2;
    if (Fib.count(n)) return Fib[n];
    if (n % 2 == 0)
        return Fib[n] = (fib(k) * fib(k) + fib(k - 1) * fib(k - 1)) % M;
    else
        return Fib[n] = (fib(k) * fib(k + 1) + fib(k - 1) * fib(k)) % M;
}
int main() {
    LL n;
    Fib[0] = Fib[1] = 1;
    scanf("%lld", &n);
    printf("%lld\n", fib(n));
    return 0;
}
/*
    判断一个数是否是斐波那契数
*/
bool isSquare(long long n) { /* */
}
bool isFibonacci(int n) {
    return n >= 0 && isSquare(5 * n * n + 4) || isSquare(5 * n * n - 4);
}
