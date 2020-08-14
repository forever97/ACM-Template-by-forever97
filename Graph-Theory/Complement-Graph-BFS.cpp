/*
    补图BFS
    给定一张n个点m条边的图
    求对于完全图的补图的连通块
*/
using namespace std;
const int N = 100010;
int f[N], n, m, vis[N], ans, cnt[N];
vector<int> v[N];
int sf(int x) { return f[x] == x ? x : f[x] = sf(f[x]); }
void bfs(int st) {
    queue<int> q;
    q.push(st);
    cnt[++ans] = 1;
    while (q.size()) {
        int x = q.front();
        q.pop();
        f[x] = sf(x + 1);
        for (int i = 0; i < v[x].size(); i++) vis[v[x][i]] = x;
        for (int i = sf(1); i <= n; i = sf(i + 1))
            if (vis[i] != x) {
                cnt[ans]++;
                f[i] = sf(i + 1);
                q.push(i);
            }
    }
}
int main() {
    while (~scanf("%d%d", &n, &m)) {
        ans = 0;
        while (m--) {
            int x, y;
            scanf("%d%d", &x, &y);
            v[x].push_back(y);
            v[y].push_back(x);
        }
        for (int i = 1; i <= n + 1; i++) f[i] = i;
        for (int i = 1; i <= n; i = sf(i + 1)) bfs(i);
        printf("%d\n", ans);
        sort(cnt + 1, cnt + ans + 1);
        for (int i = 1; i <= ans; i++) printf("%d ", cnt[i]);
        puts("");
    }
    return 0;
}