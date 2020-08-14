/*
    利用最短路求解丢番图方程
    Check：判断num是否能被表示为a[1]*x[1]+a[2]*x[2]+……+a[n]*x[n]，x为非负整数
    Cal：计算大于等于k的最小的能被表示的数
    Query: 求解x以内解的数量
*/
const int N = [the value of a[0]];
namespace DIJKSTRA {
typedef long long LL;
const LL INF = 0x3f3f3f3f3f3f3f3f;
typedef pair<LL, int> P;
priority_queue<P, vector<P>, greater<P> > Q;
int a[N], n, m;
LL d[N];
void Initialize() {
    int x, i;
    sort(a, a + n);
    m = a[0];
    d[0] = 0;
    for (i = 1; i < m; i++) d[i] = INF;
    Q.push(P(0, 0));
    while (!Q.empty()) {
        P t = Q.top();
        Q.pop();
        if (d[t.second] < t.first) continue;
        for (x = t.second, i = 1; i < n; i++) {
            if (d[x] + a[i] < d[(x + a[i]) % m])
                Q.push(P(d[(x + a[i]) % m] = d[x] + a[i], (x + a[i]) % m));
        }
    }
}
bool Check(LL x) { return d[x % m] <= x; }
int Cal(LL k) {
    for (int i = 0; i < m; i++)
        if (d[(k + i) % m] <= k + i) return k + i;
}
LL Query(LL x) {
    LL res = 0;
    for (int i = 0; i < m; i++)
        if (d[i] <= x) res += (x - d[i]) / m + 1;
    return res;
}
}  // namespace DIJKSTRA
/*
    Test
    求解L到R之间的非负整数解集个数
    Input ：2 5 10 3 5
    Output ：5
*/
long long L, R;
int main() {
    using namespace DIJKSTRA;
    scanf("%d%lld%lld", &n, &L, &R);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    Initialize();
    printf("%lld\n", Query(R) - Query(L - 1));
    return 0;
}