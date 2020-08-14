/*
    法雷序列
    求a/b<p/q<c/d的q最小的分数
*/
using namespace std;
typedef long long ll;
typedef pair<ll, ll> P;
ll a, b, c, d, t;
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
P cal(ll a, ll b, ll c, ll d) {
    ll x = a / b + 1;
    if (x * d < c) return P(x, 1);
    if (!a) return P(1, d / c + 1);
    if (a <= b && c <= d) {
        P t = cal(d, c, b, a);
        swap(t.first, t.second);
        return t;
    }
    x = a / b;
    P t = cal(a - b * x, b, c - d * x, d);
    t.first += t.second * x;
    return t;
}
int main() {
    while (~scanf("%lld%lld%lld%lld", &a, &b, &c, &d)) {
        t = gcd(a, b), a /= t, b /= t;
        t = gcd(c, d), c /= t, d /= t;
        P p = cal(a, b, c, d);
        printf("%lld/%lld\n", p.first, p.second);
    }
    return 0;
}