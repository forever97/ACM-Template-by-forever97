/*
    最小点覆盖
    邻接矩阵给出覆盖关系
*/
#include <bitset>
#include <cstdio>
using namespace std;
const int N = 80;
bitset<N> a[N];
char s[N];
int n, limit, ans[N], cnt = 1;
bool dfs(int d, int pos, bitset<N> mp) {
    if (d >= limit) return mp.count() == n;
    for (int i = pos; i < n; i++)
        if (dfs(d + 1, (ans[d] = i + 1), mp | a[i])) return 1;
    return 0;
}
int main() {
    while (~scanf("%d", &n)) {
        for (int i = 0; i < n; i++) {
            a[i].reset();
            scanf("%s", s);
            for (int j = 0; j < n; j++)
                if (s[j] == '1' || i == j) a[i].set(j);
        }
        for (limit = 1; limit <= 80; limit++)
            if (dfs(0, 0, bitset<N>())) break;
        printf("Case %d: %d", cnt++, limit);
        for (int i = 0; i < limit; i++) printf(" %d", ans[i]);
        puts("");
    }
    return 0;
}