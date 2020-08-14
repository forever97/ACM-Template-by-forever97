/*
    求原根
*/
int q[10000];
int pow(ll a, int b, int P) {
    ll t = 1;
    for (; b; b >>= 1, a = (ll)a * a % P)
        if (b & 1) t = t * a % P;
    return t;
}
int getG(int n) {
    int i, j, t = 0;
    for (i = 2; (ll)i * i < n - 1; i++)
        if ((n - 1) % i == 0) q[t++] = i, q[t++] = (n - 1) / i;
    for (i = 2;; i++) {
        for (j = 0; j < t; j++)
            if (pow(i, q[j], n) == 1) break;
        if (j == t) return i;
    }
}
/*
    对于给定的p，n次询问给出数x是否是其原根
*/
using namespace std;
typedef long long ll;
ll modPow(ll a, int n, int p) {
    ll res = 1;
    while (n) {
        if (n & 1) res = res * a % p;
        n >>= 1;
        a = a * a % p;
    }
    return res;
}
int fac[100000], cnt;
int main() {
    int p, n;
    while (scanf("%d%d", &p, &n), n || p) {
        cnt = 0;
        p--;
        int lim = sqrt(p + 1);
        for (int i = 2; i <= lim; i++)
            if (p % i == 0) fac[cnt++] = i, fac[cnt++] = p / i;
        while (n--) {
            int x;
            scanf("%d", &x);
            bool fg = 1;
            for (int i = 0; i < cnt && fg; i++)
                if (modPow(x, fac[i], p + 1) == 1) fg = 0;
            puts(fg ? "YES" : "NO");
        }
    }
    return 0;
}