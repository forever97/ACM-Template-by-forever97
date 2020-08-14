/*
    TD树
    划分为四个平面维护
    Example: 给出一个颜色方格图，每个格子都有一个颜色，
    现在往一些矩形内的方格涂新的颜色，当一个格子涂色和原来不一样时就不再是一个纯色格子，
    问有多少个格子不再是纯色格子
*/
#include <bits/stdc++.h>
#define rep(i, n) for (int i = 1; i <= n; i++)
typedef long long ll;
using namespace std;
inline char nc() {
    static char buf[100000], *p1 = buf, *p2 = buf;
    if (p1 == p2) {
        p2 = (p1 = buf) + fread(buf, 1, 100000, stdin);
        if (p1 == p2) return EOF;
    }
    return *p1++;
}
inline void read(int &x) {
    char c = nc(), b = 1;
    for (; !(c >= '0' && c <= '9'); c = nc())
        if (c == '-') b = -1;
    for (x = 0; c >= '0' && c <= '9'; x = x * 10 + c - '0', c = nc())
        ;
    x *= b;
}
const int N = 1000010;
int tot = 0, T[N << 2][4], col[N << 2];
int a[N];
int *mp[N];
void build(int &x, int l1, int r1, int l2, int r2) {
    if (l1 > r1 || l2 > r2) return;  // Notice
    x = ++tot;
    if (l1 == r1 && l2 == r2) {
        col[x] = mp[l1 - 1][l2 - 1];
        return;
    }
    int mid1 = (l1 + r1) >> 1, mid2 = (l2 + r2) >> 1;
    build(T[x][0], l1, mid1, l2, mid2);
    build(T[x][1], l1, mid1, mid2 + 1, r2);
    build(T[x][2], mid1 + 1, r1, l2, mid2);
    build(T[x][3], mid1 + 1, r1, mid2 + 1, r2);
}
void pb(int x) {
    if (!col[x]) return;
    for (int i = 0; i < 4; i++)
        if (T[x][i]) {
            if (!col[T[x][i]] || col[T[x][i]] == col[x])
                col[T[x][i]] = col[x];
            else
                col[T[x][i]] = -1;
        }
    col[x] = 0;
}
int cx1, cx2, cy1, cy2, cc;
void update(int x, int l1, int r1, int l2, int r2) {
    if (cx1 <= l1 && r1 <= cx2 && cy1 <= l2 && r2 <= cy2) {
        if (!col[x] || col[x] == cc)
            col[x] = cc;
        else
            col[x] = -1;
        return;
    }
    if (col[x] < 0) return;
    pb(x);
    int mid1 = (l1 + r1) >> 1, mid2 = (l2 + r2) >> 1;
    if (cx1 <= mid1 && cy1 <= mid2) update(T[x][0], l1, mid1, l2, mid2);
    if (cx1 <= mid1 && cy2 > mid2) update(T[x][1], l1, mid1, mid2 + 1, r2);
    if (cx2 > mid1 && cy1 <= mid2) update(T[x][2], mid1 + 1, r1, l2, mid2);
    if (cx2 > mid1 && cy2 > mid2) update(T[x][3], mid1 + 1, r1, mid2 + 1, r2);
}
int qx, qy;
int query(int x, int l1, int r1, int l2, int r2) {
    if (l1 == r1 && l2 == r2 || col[x] < 0) return col[x];
    pb(x);
    int mid1 = (l1 + r1) >> 1, mid2 = (l2 + r2) >> 1;
    if (qx <= mid1 && qy <= mid2) return query(T[x][0], l1, mid1, l2, mid2);
    if (qx <= mid1 && qy > mid2) return query(T[x][1], l1, mid1, mid2 + 1, r2);
    if (qx > mid1 && qy <= mid2) return query(T[x][2], mid1 + 1, r1, l2, mid2);
    return query(T[x][3], mid1 + 1, r1, mid2 + 1, r2);
}
int main() {
    int n, m, k, root, lft = 0;
    read(n);
    read(m);
    read(k);
    for (int i = 0; i < n * m; i++) read(a[i]);
    for (int i = 0; i < n; i++) mp[i] = a + i * m;
    build(root, 1, n, 1, m);
    for (int i = 1; i <= k; i++) {
        read(cx1);
        read(cy1);
        read(cx2);
        read(cy2);
        read(cc);
        update(root, 1, n, 1, m);
    }
    for (qx = 1; qx <= n; qx++) {
        for (qy = 1; qy <= m; qy++) lft += query(root, 1, n, 1, m) >= 0;
    }
    printf("%d\n", n * m - lft);
    return 0;
}
