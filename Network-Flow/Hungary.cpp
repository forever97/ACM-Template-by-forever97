/*
    二分图匹配
*/
namespace Hungary {
const int MAX_V = 2000;
const int INF = 0x3f3f3f3f;
int V, match[MAX_V];
vector<int> G[MAX_V];
bool used[MAX_V];
void add_edge(int u, int v) {
    G[u].push_back(v);
    G[v].push_back(u);
}
bool dfs(int v) {
    used[v] = 1;
    for (int i = 0; i < G[v].size(); i++) {
        int u = G[v][i], w = match[u];
        if (w < 0 || !used[w] && dfs(w)) {
            match[v] = u;
            match[u] = v;
            return 1;
        }
    }
    return 0;
}
int bipartite_matching() {
    int res = 0;
    memset(match, -1, sizeof(match));
    for (int v = 0; v < V; v++) {
        if (match[v] < 0) {
            memset(used, 0, sizeof(used));
            if (dfs(v)) res++;
        }
    }
    return res;
}
void Initialize(int n) {
    V = n;
    for (int i = 0; i < V; i++) G[i].clear();
}
}  // namespace Hungary