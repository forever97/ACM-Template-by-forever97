/* 稳定婚姻系统
 * 给定n对夫妻关系，再给定m对情人关系。
 * 如果在第i对夫妻离婚的前提下，
 * 这些人仍然能组成n对夫妻，就称i这个婚姻是不稳定的。
 * 要求判断所有的婚姻稳不稳定
 * O(n+m)
 */
const int N = 4010, M = 20010;
bool v[N];
int idx, cnt, dfn[N], low[N], c[N], s[N], sz[N], h[N], p[M], nxt[M], n, m, i, x;
map<string, int> g, b;
string name;
inline void _min(int& x, int y) {
    if (y < x) x = y;
}
void tarjan(int x) {
    dfn[x] = low[x] = ++idx;
    v[s[++s[0]] = x] = 1;
    for (int k = h[x]; k; k = nxt[k]) {
        if (!dfn[p[k]])
            tarjan(p[k]), _min(low[x], low[p[k]]);
        else if (v[p[k]])
            _min(low[x], dfn[p[k]]);
    }
    if (dfn[x] == low[x])
        for (++sz[c[s[s[0]]] = ++cnt], v[s[s[0]]] = 0; s[s[0]--] != x;
             ++sz[c[s[s[0]]] = cnt], v[s[s[0]]] = 0)
            ;
}
int main() {
    cin >> n;
    for (i = 1; i <= n; i++) {
        cin >> name;
        g[name] = i;
        cin >> name;
        b[name] = i;
    }
    cin >> m;
    for (i = 1; i <= m; i++) {
        cin >> name;
        x = g[name];
        cin >> name;
        p[i] = b[name];
        nxt[i] = h[x];
        h[x] = i;
    }
    for (i = 1; i <= n; i++)
        if (!dfn[i]) tarjan(i);
    for (i = 1; i <= n; i++) puts(sz[c[i]] == 1 ? "Safe" : "Unsafe");
    return 0;
}