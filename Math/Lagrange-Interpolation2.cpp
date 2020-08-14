/*
    拉格朗日插值
    在线插值&求经过所有插入点的阶小于插入集的f(x)
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll P = 998244353;
const int N = 1 << 15 | 1;
int f[N], g[N];
int num = 0;
int query(int x) {
    int ans = 0;
    for (int i = num - 1; i >= 0; i--) {
        ans = (ll)ans * x % P;
        ans += f[i];
        if (ans >= P) ans -= P;
    }
    return ans;
}
int query2(int x) {
    int ans = 0;
    for (int i = num; i >= 0; i--) {
        ans = (ll)ans * x % P;
        ans += g[i];
        if (ans >= P) ans -= P;
    }
    return ans;
}
int mt(int a, int b) {
    if (b == 0) return 1;
    int c = mt(a, b >> 1);
    c = (ll)c * c % P;
    if (b & 1) c = (ll)c * a % P;
    return c;
}
bool addp(int x, int y) {
    y -= query(x);
    if (y < 0) y += P;
    int d = query2(x);
    if (d == 0) return 0;
    y = (ll)y * mt(d, P - 2) % P;
    for (int i = 0; i <= num; i++) {
        f[i] += (ll)g[i] * y % P;
        if (f[i] >= P) f[i] -= P;
    }
    x = -x;
    if (x < 0) x += P;
    for (int i = num; i >= 0; i--) {
        g[i + 1] += g[i];
        if (g[i + 1] >= P) g[i + 1] -= P;
        g[i] = (ll)g[i] * x % P;
    }
    num++;
    return 1;
}
int main() {
    f[0] = 0, g[0] = 1;
    int n;
    scanf("%d", &n);
    while (n--) {
        int opt, x;
        scanf("%d%d", &opt, &x);
        if (opt == 2) {
            printf("%d\n", query(x));
        } else {
            int y;
            scanf("%d", &y);
            addp(x, y);
        }
    }
    return 0;
}
