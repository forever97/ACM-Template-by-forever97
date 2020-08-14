/* 最小树形图
 * 节点：0~n-1
 * variable用于计算不定根的最小树形图
 * 使用方法： init()
 *           add(from,to,cost)
 *           dmst(root)
 * 复杂度 O(VE)
 */
const int N = 10050, M = 50050, inf = 0x7fffffff;
struct DMST {
    int n, size, pre[N], id[N], vis[N], in[N];
    struct EDGE {
        int u, v, cost;
        EDGE() {}
        EDGE(int a, int b, int c) : u(a), v(b), cost(c) {}
    } E[M];
    void init(int _n) { n = _n, size = 0; }
    void add(int u, int v, int w) { E[size++] = EDGE(u, v, w); }
    int dmst(int root) {
        int u, v, cnt, ret = 0;
        while (1) {
            for (int i = 0; i < n; i++) in[i] = inf;
            for (int i = 0; i < size; i++) {
                u = E[i].u, v = E[i].v;
                if (E[i].cost < in[v] && u != v) {
                    pre[v] = u, in[v] = E[i].cost;
                    if (u == root) ROOT = i;
                }
            }
            for (int i = 0; i < n; i++)
                if (i != root && in[i] == inf) return -1;
            cnt = in[root] = 0;
            for (int i = 0; i < n; i++) id[i] = vis[i] = -1;
            for (int i = 0; i < n; i++) {
                ret += in[i], v = i;
                while (vis[v] != i && id[v] == -1 && v != root)
                    vis[v] = i, v = pre[v];
                if (v != root && id[v] == -1) {
                    for (u = pre[v]; u != v; u = pre[u]) id[u] = cnt;
                    id[v] = cnt++;
                }
            }
            if (!cnt) break;
            for (int i = 0; i < n; i++)
                if (id[i] == -1) id[i] = cnt++;
            for (int i = 0; v = E[i].v, i < size; i++) {
                E[i].u = id[E[i].u], E[i].v = id[E[i].v];
                if (E[i].u != E[i].v) E[i].cost -= in[v];
            }
            n = cnt, root = id[root];
        }
        return ret;
    }
} U;
void variable(int &cost, int &root) {
    for (int i = 0; i < n; i++)
        G.add(st, i, tot);  // st=n,tot=sum of Edge Wight+1
    int ans = G.dmst(st);
    if (ans == -1 || ans - tot > tot) return;
    cost = ans - tot, root = ROOT - m;
}
/* 限制n号点父亲编号
 * 给出一个有向图，求1点为根的最大树形图
 * 使得第n个点的直接父亲编号最小
 */
int T, n, m;
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        U.init(n);
        while (m--) {
            int x, y, z;
            scanf("%d%d%d", &x, &y, &z);
            z *= 1000;
            if (y == n) z += n - x;
            U.add(x - 1, y - 1, -z);
        }
        LL ans = -U.dmst(0);
        int fa = n - ans % 1000;
        ans /= 1000;
        printf("%lld %d\n", ans, fa);
    }
    return 0;
}