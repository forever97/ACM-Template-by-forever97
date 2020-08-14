/*
    可持久化并查集
    1.当前版本集合合并
    2.回退版本y
    3.当前版本集合判定
*/
#include <cstdio>
const int N = 2000005;
int n, m, i, x, y, z, ans;
int l[N << 2], r[N << 2], v[N << 2], tot, root[N];
int build(int a, int b) {
    int x = ++tot;
    if (a == b) return v[x] = a, x;
    int mid = (a + b) >> 1;
    return l[x] = build(a, mid), r[x] = build(mid + 1, b), x;
}
int change(int x, int a, int b, int c, int p) {
    int y = ++tot;
    if (a == b) return v[y] = p, y;
    int mid = (a + b) >> 1;
    if (c <= mid)
        l[y] = change(l[x], a, mid, c, p), r[y] = r[x];
    else
        l[y] = l[x], r[y] = change(r[x], mid + 1, b, c, p);
    return y;
}
int ask(int x, int a, int b, int c) {
    if (a == b) return v[x];
    int mid = (a + b) >> 1;
    return c <= mid ? ask(l[x], a, mid, c) : ask(r[x], mid + 1, b, c);
}
int f(int x) {
    int t = ask(root[i], 1, n, x);
    if (t == x) return x;
    return root[i] = change(root[i], 1, n, x, t = f(t)), t;
}
int main() {
    scanf("%d%d", &n, &m);
    for (root[0] = build(i = 1, n); i <= m; i++) {
        root[i] = root[i - 1];
        scanf("%d%d", &x, &y);
        if (x != 2) scanf("%d", &z);
        if (x == 1) root[i] = change(root[i], 1, n, f(y), f(z));
        if (x == 2) root[i] = root[y];
        if (x == 3) printf("%d\n", f(y) == f(z));
    }
    return 0;
}