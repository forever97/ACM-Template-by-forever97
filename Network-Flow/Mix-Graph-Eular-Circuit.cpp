/*
    混合图欧拉回路判定
    Example:
        给定一张图，每条边的两个方向有两个不同的权值，
        现在要求从1号节点出发遍历每条边一次且仅一次，最后回到1号节点，
        求最大边权的最小值
        二分答案+混合图欧拉回路判定
*/
const int M = 2020, S = 0, T = 2019, INF = 0x3f3f3f3f;
using namespace std;
struct edge {
    int x, y, z1, z2;
} edges[M];
int n, m, max_num;
namespace Euler_Circuit {
namespace Union_Find_Set {
int fa[M], size[M], cnt;
void Initialize() {
    memset(fa, 0, sizeof fa);
    cnt = n;
}
int Find(int x) {
    if (!fa[x]) fa[x] = x, size[x] = 1;
    if (fa[x] == x) return x;
    return fa[x] = Find(fa[x]);
}
void Union(int x, int y) {
    x = Find(x);
    y = Find(y);
    if (x == y) return;
    if (size[x] > size[y]) swap(x, y);
    fa[x] = y;
    size[y] += size[x];
    cnt--;
}
}  // namespace Union_Find_Set
namespace Max_Flow {
struct abcd {
    int to, f, next;
} table[1001001];
int head[M], tot = 1;
int dpt[M];
void Initialize() {
    memset(head, 0, sizeof head);
    tot = 1;
}
void Add(int x, int y, int z) {
    table[++tot].to = y;
    table[tot].f = z;
    table[tot].next = head[x];
    head[x] = tot;
}
void Link(int x, int y, int z) {
    Add(x, y, z);
    Add(y, x, 0);
}
bool BFS() {
    static int q[M];
    int i, r = 0, h = 0;
    memset(dpt, -1, sizeof dpt);
    q[++r] = S;
    dpt[S] = 1;
    while (r != h) {
        int x = q[++h];
        for (i = head[x]; i; i = table[i].next)
            if (table[i].f && !~dpt[table[i].to]) {
                dpt[table[i].to] = dpt[x] + 1;
                q[++r] = table[i].to;
                if (table[i].to == T) return true;
            }
    }
    return false;
}
int Dinic(int x, int flow) {
    int i, left = flow;
    if (x == T) return flow;
    for (i = head[x]; i && left; i = table[i].next)
        if (table[i].f && dpt[table[i].to] == dpt[x] + 1) {
            int temp = Dinic(table[i].to, min(left, table[i].f));
            left -= temp;
            table[i].f -= temp;
            table[i ^ 1].f += temp;
        }
    if (left) dpt[x] = -1;
    return flow - left;
}
}  // namespace Max_Flow
bool Judge(int x) {
    using namespace Union_Find_Set;
    using namespace Max_Flow;
    Union_Find_Set::Initialize();
    Max_Flow::Initialize();
    static int degree[M];
    //入度+1 出度-1
    int i;
    memset(degree, 0, sizeof degree);
    for (i = 1; i <= m; i++) {
        if (edges[i].z2 <= x) {
            Union(edges[i].x, edges[i].y);
            degree[edges[i].x]++;
            degree[edges[i].y]--;
            Link(edges[i].x, edges[i].y, 1);
        } else if (edges[i].z1 <= x) {
            Union(edges[i].x, edges[i].y);
            degree[edges[i].x]--;
            degree[edges[i].y]++;
        }
    }
    if (cnt >= 2) return false;
    for (i = 1; i <= n; i++) {
        if (degree[i] & 1) return false;
        if (degree[i] > 0)
            Link(S, i, degree[i] >> 1);
        else
            Link(i, T, -degree[i] >> 1);
    }
    while (BFS()) Dinic(S, INF);
    for (i = head[S]; i; i = table[i].next)
        if (table[i].f) return false;
    return true;
}
}  // namespace Euler_Circuit
int Bisection(int l, int r) {
    using namespace Euler_Circuit;
    while (l + 1 < r) {
        int mid = l + r >> 1;
        if (Judge(mid))
            r = mid;
        else
            l = mid;
    }
    return Judge(l) ? l : r;
}
int main() {
    int i, x, y, z1, z2;
    cin >> n >> m;
    for (i = 1; i <= m; i++) {
        scanf("%d%d%d%d", &x, &y, &z1, &z2);
        if (z1 > z2) swap(x, y), swap(z1, z2);
        edges[i].x = x;
        edges[i].y = y;
        edges[i].z1 = z1;
        edges[i].z2 = z2;
        max_num = max(max_num, z1);
        max_num = max(max_num, z2);
    }
    int ans = Bisection(1, max_num + 1);
    if (ans == max_num + 1)
        cout << "NIE" << endl;
    else
        cout << ans << endl;
    return 0;
}