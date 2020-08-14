/*
    强连通分量
    点编号 0~n-1
    cmp记录强连通分量拓扑序
    即缩后的点标号
*/
namespace SCC {
const int MAX_V = 10000;
int V;                  //顶点数
vector<int> G[MAX_V];   //图的邻接表表示
vector<int> rG[MAX_V];  //反向图
vector<int> vs;         //后序遍历
bool used[MAX_V];
int cmp[MAX_V];  //所属强连通分量的拓扑序
void Initialize() {
    for (int i = 0; i < V; i++) G[i].clear();
    for (int i = 0; i < V; i++) rG[i].clear();
}
void add_edge(int from, int to) {
    G[from].push_back(to);
    rG[to].push_back(from);
}
void dfs(int v) {
    used[v] = 1;
    for (int i = 0; i < G[v].size(); i++) {
        if (!used[G[v][i]]) dfs(G[v][i]);
    }
    vs.push_back(v);
}
void rdfs(int v, int k) {
    used[v] = 1;
    cmp[v] = k;
    for (int i = 0; i < rG[v].size(); i++) {
        if (!used[rG[v][i]]) rdfs(rG[v][i], k);
    }
}
int scc() {
    memset(used, 0, sizeof(used));
    vs.clear();
    for (int v = 0; v < V; v++) {
        if (!used[v]) dfs(v);
    }
    memset(used, 0, sizeof(used));
    int k = 0;
    for (int i = vs.size() - 1; i >= 0; i--) {
        if (!used[vs[i]]) rdfs(vs[i], k++);
    }
    return k;
}
}  // namespace SCC
/*
    Example1
    询问一张图需要加入的最小有向边数量使得其构成强连通
    Ans=max(NO_IN,NO_OUT)
*/
const int MAX_M = 50000;
int N, M, x;
int A[MAX_M], B[MAX_M];
int in[MAX_V], out[MAX_V], NO_IN, NO_OUT;
void solve() {
    using namespace SCC;
    scanf("%d%d", &N, &M);
    V = N;
    Initialize();
    for (int i = 0; i < M; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        add_edge(x - 1, y - 1);
    }
    int n = scc();
    if (n == 1) {
        puts("0");
        return;
    }
    memset(in, 0, sizeof(in));
    memset(out, 0, sizeof(out));
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < G[i].size(); j++) {
            if (cmp[i] != cmp[G[i][j]]) {
                ++out[cmp[i]];
                ++in[cmp[G[i][j]]];
            }
        }
    }
    NO_IN = NO_OUT = 0;
    for (int i = 0; i < n; i++) {
        if (!in[i]) NO_IN++;
        if (!out[i]) NO_OUT++;
    }
    printf("%d\n", max(NO_IN, NO_OUT));
}
/*
    Example2
    求大小大于1的强连通数量
*/
const int MAX_M = 50000;
int N, M;
int A[MAX_M], B[MAX_M];
int cnt[MAX_M];
void solve() {
    using namespace SCC;
    scanf("%d%d", &N, &M);
    V = N;
    for (int i = 0; i < M; i++) scanf("%d%d", &A[i], &B[i]);
    for (int i = 0; i < M; i++) {
        add_edge(A[i] - 1, B[i] - 1);
    }
    int n = scc();
    int num = 0;
    memset(cnt, 0, sizeof(cnt));
    for (int i = 0; i < V; i++) ++cnt[cmp[i]];
    for (int i = 0; i < n; i++)
        if (cnt[i] > 1) num++;
    printf("%d\n", num);
}
/*
    Example3
    求能被所有点到达的点的数量
*/
const int MAX_M = 50000;
int N, M;
int A[MAX_M], B[MAX_M];
void solve() {
    using namespace SCC;
    scanf("%d%d", &N, &M);
    V = N;
    for (int i = 0; i < M; i++) scanf("%d%d", &A[i], &B[i]);
    for (int i = 0; i < M; i++) {
        add_edge(A[i] - 1, B[i] - 1);
    }
    int n = scc();
    int u = 0, num = 0;
    for (int v = 0; v < V; v++) {
        if (cmp[v] == n - 1) {
            u = v;
            num++;
        }
    }
    memset(used, 0, sizeof(used));
    rdfs(u, 0);
    for (int v = 0; v < V; v++) {
        if (!used[v]) {
            num = 0;
            break;
        }
    }
    printf("%d\n", num);
}