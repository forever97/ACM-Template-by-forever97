/*
    2-SAT
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
using namespace SCC;
int N, M;
int x, y, c;
char op[10];
int solve() {
    V = N * 2;
    // 0~N-1 表示 x取0
    // N~N+N-1 表示 x取1
    for (int i = 0; i < M; i++) {
        scanf("%d%d%d%s", &x, &y, &c, op);
        if (op[0] == 'A') {
            // x and y = 0
            if (c == 0) {
                add_edge(y + N, x);
                add_edge(x + N, y);
            }
            // x and y = 1
            else {
                add_edge(x, x + N);
                add_edge(y, y + N);
            }
        } else if (op[0] == 'O') {
            // x or y = 0
            if (c == 0) {
                add_edge(x + N, x);
                add_edge(y + N, y);
            }
            // x or y = 1
            else {
                add_edge(x, y + N);
                add_edge(y, x + N);
            }
        } else if (op[0] == 'X') {
            // x xor y = 0
            if (c == 0) {
                add_edge(x, y);
                add_edge(y, x);
                add_edge(x + N, y + N);
                add_edge(y + N, x + N);
            }
            // x xor y = 1
            else {
                add_edge(x, y + N);
                add_edge(y, x + N);
                add_edge(x + N, y);
                add_edge(y + N, x);
            }
        }
    }
    // 判断满足条件下解集是否存在
    int n = scc();
    int flag = 1;
    for (int i = 0; i < N; i++)
        if (cmp[i] == cmp[i + N]) flag = 0;
    puts(flag ? "YES" : "NO");
}