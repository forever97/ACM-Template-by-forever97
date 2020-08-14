/*
    最小割树
    GomoryHu Tree
    计算出任意两点间的最小割(最大流)
    Example: 给出一张无向图，求任意两点间最小割minCut(s,t)<=x的点对数
*/
#include <bits/stdc++.h>
using namespace std;
#define REP(i, a) for (int i = 0, _a = (a); i < _a; i++)
#define FOR(i, a, b) for (int i = (a), _b = (b); i <= _b; i++)
const int INF = 1000000000;
struct Edge {
    int a, b, cap, flow;
};
struct MaxFlow {
    // MaxFlow flow(n)
    // For each edge: flow.addEdge(u, v, c)
    // Index from 0
    int n, s, t;
    vector<int> d, ptr, q;
    vector<Edge> e;
    vector<vector<int> > g;
    MaxFlow(int n) : n(n), d(n), ptr(n), q(n), g(n) {
        e.clear();
        REP(i, n) {
            g[i].clear();
            ptr[i] = 0;
        }
    }
    void addEdge(int a, int b, int cap) {
        Edge e1 = {a, b, cap, 0};
        Edge e2 = {b, a, 0, 0};
        g[a].push_back((int)e.size());
        e.push_back(e1);
        g[b].push_back((int)e.size());
        e.push_back(e2);
    }
    int getMaxFlow(int _s, int _t) {
        s = _s;
        t = _t;
        int flow = 0;
        for (;;) {
            if (!bfs()) break;
            REP(i, n) ptr[i] = 0;
            while (int pushed = dfs(s, INF)) flow += pushed;
        }
        return flow;
    }

   private:
    bool bfs() {
        int qh = 0, qt = 0;
        q[qt++] = s;
        REP(i, n) d[i] = -1;
        d[s] = 0;
        while (qh < qt && d[t] == -1) {
            int v = q[qh++];
            REP(i, g[v].size()) {
                int id = g[v][i], to = e[id].b;
                if (d[to] == -1 && e[id].flow < e[id].cap) {
                    q[qt++] = to;
                    d[to] = d[v] + 1;
                }
            }
        }
        return d[t] != -1;
    }
    int dfs(int v, int flow) {
        if (!flow) return 0;
        if (v == t) return flow;
        for (; ptr[v] < (int)g[v].size(); ++ptr[v]) {
            int id = g[v][ptr[v]], to = e[id].b;
            if (d[to] != d[v] + 1) continue;
            int pushed = dfs(to, min(flow, e[id].cap - e[id].flow));
            if (pushed) {
                e[id].flow += pushed;
                e[id ^ 1].flow -= pushed;
                return pushed;
            }
        }
        return 0;
    }
};
const int MN = 211;
const int oo = 1000111000;
struct GomoryHu {
    int ok[MN], cap[MN][MN];
    int answer[MN][MN], parent[MN];
    int n;
    MaxFlow flow;
    GomoryHu(int n) : n(n), flow(n) {
        for (int i = 0; i < n; ++i) ok[i] = parent[i] = 0;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) cap[i][j] = 0, answer[i][j] = INF;
    }
    void addEdge(int u, int v, int c) { cap[u][v] += c; }
    void calc() {
        for (int i = 0; i < n; ++i) parent[i] = 0;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) answer[i][j] = 2000111000;
        for (int i = 1; i <= n - 1; ++i) {
            flow = MaxFlow(n);
            REP(u, n) REP(v, n) if (cap[u][v]) flow.addEdge(u, v, cap[u][v]);
            int f = flow.getMaxFlow(i, parent[i]);
            bfs(i);
            for (int j = i + 1; j < n; ++j)
                if (ok[j] && parent[j] == parent[i]) parent[j] = i;
            answer[i][parent[i]] = answer[parent[i]][i] = f;
            for (int j = 0; j < i; ++j)
                answer[i][j] = answer[j][i] = min(f, answer[parent[i]][j]);
        }
    }
    void bfs(int start) {
        memset(ok, 0, sizeof ok);
        queue<int> qu;
        qu.push(start);
        while (!qu.empty()) {
            int u = qu.front();
            qu.pop();
            for (int xid = 0; xid < flow.g[u].size(); ++xid) {
                int id = flow.g[u][xid];
                int v = flow.e[id].b, fl = flow.e[id].flow,
                    cap = flow.e[id].cap;
                if (!ok[v] && fl < cap) {
                    ok[v] = 1;
                    qu.push(v);
                }
            }
        }
    }
};
int main() {
    ios::sync_with_stdio(false);
    int ntest;
    cin >> ntest;
    while (ntest--) {
        int n, m;
        cin >> n >> m;
        GomoryHu gh(n);
        while (m--) {
            int u, v, c;
            cin >> u >> v >> c;
            --u;
            --v;
            gh.addEdge(u, v, c);
            gh.addEdge(v, u, c);
        }
        gh.calc();
        int q;
        cin >> q;
        while (q--) {
            int t;
            cin >> t;
            int res = 0;
            REP(i, n) FOR(j, i + 1, n - 1) if (gh.answer[i][j] <= t) { ++res; }
            cout << res << endl;
        }
        cout << endl;
    }
}
