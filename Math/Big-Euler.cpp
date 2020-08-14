/*
    欧拉函数
    已知N，求phi(N)，N<=10^18
    Pollard_Rho
*/
#include <bits/stdc++.h>
#define C 2730
#define S 3
using namespace std;
typedef long long ll;
ll n;
vector<ll> v;
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
ll mul(ll a, ll b, ll n) {
    return (a * b - (ll)(a / (long double)n * b + 1e-3) * n + n) % n;
}
ll pow(ll a, ll b, ll n) {
    ll d = 1;
    a %= n;
    while (b) {
        if (b & 1) d = mul(d, a, n);
        a = mul(a, a, n);
        b >>= 1;
    }
    return d;
}
bool check(ll a, ll n) {
    ll m = n - 1, x, y;
    int i, j = 0;
    while (!(m & 1)) m >>= 1, j++;
    x = pow(a, m, n);
    for (i = 1; i <= j; x = y, i++) {
        y = pow(x, 2, n);
        if ((y == 1) && (x != 1) && (x != n - 1)) return 1;
    }
    return y != 1;
}
bool miller_rabin(int times, ll n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (!(n & 1)) return 0;
    while (times--)
        if (check(rand() % (n - 1) + 1, n)) return 0;
    return 1;
}
ll pollard_rho(ll n, int c) {
    ll i = 1, k = 2, x = rand() % n, y = x, d;
    while (1) {
        i++, x = (mul(x, x, n) + c) % n, d = gcd(y - x, n);
        if (d > 1 && d < n) return d;
        if (y == x) return n;
        if (i == k) y = x, k <<= 1;
    }
}
void findfac(ll n, int c) {
    if (n == 1) return;
    if (miller_rabin(S, n)) {
        v.push_back(n);
        return;
    }
    ll m = n;
    while (m == n) m = pollard_rho(n, c--);
    findfac(m, c), findfac(n / m, c);
}
int main() {
    while (~scanf("%lld", &n)) {
        findfac(n, C);
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());
        ll phi = n;
        for (int i = 0; i < v.size(); i++) {
            ll y = v[i];
            phi = (phi / y) * (y - 1);
        }
        printf("%lld\n", phi);
    }
    return 0;
}