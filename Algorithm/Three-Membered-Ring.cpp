/*
    三元环问题
    题目大意：
        N个点m条边，每个点有一个点权a。
        对于任意一个三元环(j，j，k)其贡献为max(a[i],a[j],a[k])，请你求出贡献值之和。
    题解：
        我们将无向边转化成从权值大的点指向权值小的点的有向边，
        按权值从小到大的顺序枚举起始点，枚举相连的点，
        如果其出度小于sqrt(m)，那么枚举与其相连的点，
        判断是否和起始点相连，否则，枚举起始点相连的点，判断是否和枚举点相连，
        由于边有向性，因此不会出现重复枚举的情况。
*/
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <set>
#include <vector>
using namespace std;
const int N = 250005;
vector<int> v[N];
int i, val[N], sa[N], x, y, n, m, size, Rank[N], Mark[N];
long long ans;
set<int> s[N];
bool cmp(int a, int b) { return val[a] < val[b]; }
int main() {
    scanf("%d%d", &n, &m);
    for (i = 1; i <= n; i++) scanf("%d", val + i), sa[i] = i;
    for (sort(sa + 1, sa + n + 1, cmp), i = 1; i <= n; i++) Rank[sa[i]] = i;
    for (int i = 1; i <= m; i++) {
        scanf("%d%d", &x, &y);
        if (Rank[x] < Rank[y]) swap(x, y);
        v[x].push_back(y);
    }
    size = sqrt(m);
    int cnt = 0;
    for (i = 1; i <= n; i++) {
        int x = sa[i];
        ++cnt;
        for (int j = 0; j < v[x].size(); j++) Mark[v[x][j]] = cnt;
        for (int j = 0; j < v[x].size(); j++) {
            int y = v[x][j];
            if (v[y].size() < size) {
                for (int k = 0; k < v[y].size(); k++)
                    ans += (Mark[v[y][k]] == cnt) ? val[x] : 0;
            } else {
                for (int k = 0; k < v[x].size(); k++)
                    ans += s[y].count(v[x][k]) ? val[x] : 0;
            }
            s[x].insert(y);
        }
    }
    printf("%lld\n", ans);
    return 0;
}
