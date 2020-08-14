/*
`   类欧几里得算法
    Ti=floor((a*i+b)/c)
    f(a,b,c,n)=sum(Ti)
    g(a,b,c,n)=sum(i*Ti)
    h(a,b,c,n)=sum(Ti^2)
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
using namespace std;
const int mo = 1e9 + 7, inv2 = 500000004, inv6 = 166666668;
typedef long long LL;
int a, b, c, l, r;
struct data {
    int f, g, h;
};
data calc(int a, int b, int c, LL n) {
    data tmp;
    if (!a) {
        tmp.f = tmp.g = tmp.h = 0;
        return tmp;
    }
    if (a >= c || b >= c) {
        tmp = calc(a % c, b % c, c, n);
        n %= mo;
        tmp.h = (tmp.h +
                 n * (n + 1) % mo * (2 * n + 1) % mo * inv6 % mo * (a / c) %
                     mo * (a / c) % mo +
                 (n + 1) * (b / c) % mo * (b / c) % mo +
                 (LL)2 * (a / c) * tmp.g % mo + (LL)2 * (b / c) * tmp.f % mo +
                 n * (n + 1) % mo * (a / c) % mo * (b / c)) %
                mo;
        tmp.f =
            (tmp.f + n * (n + 1) / 2 % mo * (a / c) + (n + 1) * (b / c)) % mo;
        tmp.g =
            (tmp.g + n * (n + 1) % mo * (2 * n + 1) % mo * inv6 % mo * (a / c) +
             n * (n + 1) / 2 % mo * (b / c)) %
            mo;
        return tmp;
    }
    LL m = ((LL)a * n + b) / c;
    data nxt = calc(c, c - b - 1, a, m - 1);
    n %= mo;
    m %= mo;
    tmp.f = ((n * m - nxt.f) % mo + mo) % mo;
    tmp.g = (LL)((n * (n + 1) % mo * m - nxt.f - nxt.h) % mo + mo) * inv2 % mo;
    tmp.h =
        ((m * (m + 1) % mo * n - (LL)2 * (nxt.g + nxt.f) % mo - tmp.f) % mo +
         mo) %
        mo;
    return tmp;
}
// 求l到r的g函数
int main() {
    scanf("%d%d%d%d%d", &a, &c, &b, &l, &r);
    printf("%d\n", (calc(a, b, c, r).g - calc(a, b, c, l - 1).g + mo) % mo);
    return 0;
}
/*
    题目大意：求sum(-1^floor(sqrt(d*r*d))) [d from 1 to n]
    题解，我们设x=sqrt(r)，则题目为求sum(-1^floor(dx)) [d from 1 to n],
    -1^floor(dx)=1-2(floor(dx)%2)=1-2(floor(dx)-floor(dx/2)*2)
    =1-2*floor(dx)+4*floor(dx/2)
    对floor(dx)和floor(dx/2)类欧求解
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
using namespace std;
int T, n, r;
double t;
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }
int likegcd(int n, int a, int b, int c) { 
    if (!n) return 0;
    int f = gcd(gcd(a, b), c);
    a /= f, b /= f, c /= f;
    f = (t * b + c) / a;
    int res = 1LL * n * (n + 1) * f >> 1;
    c -= f * a;
    f = (b * t + c) * n / a;
    return res + n * f - likegcd(f, b * b * r - c * c, a * b, -a * c);
}
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &r);
        t = sqrt(r);
        if ((int)t == t) /*完全平方数*/
            printf("%d\n", (r & 1) ? ((n & 1) ? -1 : 0) : n);
        else
            printf("%d\n",
                   n + 4 * likegcd(n, 2, 1, 0) - 2 * likegcd(n, 1, 1, 0));
    }
    return 0;
}