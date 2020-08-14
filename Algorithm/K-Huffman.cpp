/*
    k叉哈夫曼树编码问题
    首先将不足的用0补齐，保证每次都能选出k个来合并
    然后以权值为第一关键字，最大深度为第二关键字维护小根堆，
    每次取出最小的k个元素进行合并
*/
#include <algorithm>
#include <cstdio>
using namespace std;
typedef long long ll;
typedef pair<ll, int> P;
int n, k, i, b, l;
ll x, a, ans;
P y, h[100010];
void read(ll& a) {
    char c;
    while (!(((c = getchar()) >= '0') && (c <= '9')))
        ;
    a = c - '0';
    while (((c = getchar()) >= '0') && (c <= '9')) (a *= 10) += c - '0';
}
void put(P x) {
    h[++l] = x;
    for (int i = l; i > 1 && h[i] < h[i >> 1]; i >>= 1) swap(h[i], h[i >> 1]);
}
void get() {
    y = h[1], h[1] = h[l--];
    for (int i = 1;;) {
        P tmp = h[i];
        int j = 0;
        if ((i << 1) <= l && h[i << 1] < tmp) tmp = h[j = i << 1];
        if ((i << 1 | 1) <= l && h[i << 1 | 1] < tmp) j = i << 1 | 1;
        if (j)
            swap(h[i], h[j]), i = j;
        else
            return;
    }
}
int main() {
    for (scanf("%d%d", &n, &k); n--; put(P(x, 0))) read(x);
    if (k > 2)
        while (l % (k - 1) != 1) put(P(0, 0));
    for (; l > 1; ans += a, put(P(a, b + 1)))
        for (i = a = b = 0; i < k; i++)
            get(), a += y.first, b = max(b, y.second);
    return printf("%lld\n%d", ans, h[1].second), 0;
}
