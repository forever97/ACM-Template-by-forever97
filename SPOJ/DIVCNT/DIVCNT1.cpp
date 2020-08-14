/*
    求约数个数函数d的前缀和
    n<=1e18
*/
#include <bits/stdc++.h>
#define push(x) (stack[++top] = (x))
typedef long long ll;
typedef __int128 lll;
const int N = 1000005;
struct pr {
    ll x, y;
    pr(ll x0 = 0, ll y0 = 0) : x(x0), y(y0) {}
    inline pr operator+(const pr &B) const { return pr(x + B.x, y + B.y); }
};
ll n;
int top = 0;
pr stack[N];
inline void putint(lll x) {
    static char buf[36];
    if (!x) {
        putchar(48);
        return;
    }
    int i = 0;
    for (; x; buf[++i] = x % 10 | 48, x /= 10)
        ;
    for (; i; --i) putchar(buf[i]);
}
inline bool inner(ll x, ll y) { return n < x * y; }
inline bool steep(ll x, pr v) { return (lll)n * v.x <= (lll)x * x * v.y; }
lll S1() {
    int i, crn = cbrt(n);
    ll srn = sqrt(n), x = n / srn, y = srn + 1;
    lll ret = 0;
    pr L, R, M;
    push(pr(1, 0));
    push(pr(1, 1));
    for (;;) {
        for (L = stack[top--]; inner(x + L.x, y - L.y); x += L.x, y -= L.y)
            ret += x * L.y + (L.y + 1) * (L.x - 1) / 2;
        if (y <= crn) break;
        for (R = stack[top]; !inner(x + R.x, y - R.y); R = stack[--top]) L = R;
        for (; M = L + R, 1;)
            if (inner(x + M.x, y - M.y))
                push(R = M);
            else {
                if (steep(x + M.x, R)) break;
                L = M;
            }
    }
    for (i = 1; i < y; ++i) ret += n / i;
    return ret * 2 - srn * srn;
}
int main() {
    int T;
    for (scanf("%d", &T); T; --T) {
        scanf("%lld", &n);
        putint(S1());
        putchar(10);
    }
    return 0;
}