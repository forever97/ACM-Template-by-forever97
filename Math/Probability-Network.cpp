/*
    概率网络
    Example:
        有一张无向图，两个人在起点s1和s2，在每个点i，一个人在下一分钟有p[i]的概率留在原地，
        (1-p[i])的概率随机到相连的点，问两个人每个点相遇的概率
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 1010;
int n, m, s1, s2, a[N], b[N], d[N];
int ed, nxt[N << 1], u[N << 1], v[N << 1], head[N];
double w[N << 1];
void add_edge(int a, int b, double c) {
    u[++ed] = a, v[ed] = b, w[ed] = c;
    nxt[ed] = head[u[ed]];
    head[u[ed]] = ed;
}
int sz;
double F[N][N], p[N];
int mp(int x, int y) { return (x - 1) * n + y; }
void BuildEquations() {
    F[mp(s1, s2)][0] = -1.0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i == j) continue;
            for (int ii = head[i]; ii; ii = nxt[ii]) {
                for (int jj = head[j]; jj; jj = nxt[jj]) {
                    F[mp(v[ii], v[jj])][mp(i, j)] += w[ii] * w[jj];
                }
            }
        }
    }
    for (int i = 1; i <= sz; i++) F[i][i] -= 1.0;
}
void Swap(int x, int y) {
    for (int i = 0; i <= sz; i++) swap(F[x][i], F[y][i]);
}
void multplus(int x, int y, double c) {
    for (int i = 0; i <= sz; i++) F[x][i] += F[y][i] * c;
}
void Gauss() {
    for (int i = 1; i <= sz; i++) {
        if (F[i][i] == 0) {
            for (int j = i + 1; j <= sz; j++) {
                if (F[j][i] != 0) {
                    Swap(i, j);
                    break;
                }
            }
        }
        for (int j = i + 1; j <= sz; j++) multplus(j, i, -F[j][i] / F[i][i]);
        for (int j = i - 1; j; j--) multplus(j, i, -F[j][i] / F[i][i]);
    }
    for (int i = 1; i <= sz; i++) F[i][0] /= F[i][i];
}
int main() {
    scanf("%d%d%d%d", &n, &m, &s1, &s2);
    for (int i = 1; i <= m; i++) {
        scanf("%d%d", &a[i], &b[i]);
        d[a[i]]++;
        d[b[i]]++;
    }
    for (int i = 1; i <= n; i++) {
        scanf("%lf", &p[i]);
        add_edge(i, i, p[i]);
    }
    for (int i = 1; i <= m; i++) {
        add_edge(a[i], b[i], (1 - p[a[i]]) / d[a[i]]);
        add_edge(b[i], a[i], (1 - p[b[i]]) / d[b[i]]);
    }
    sz = n * n;
    BuildEquations();
    Gauss();
    for (int i = 1; i <= n; i++)
        printf("%.7f%c", F[mp(i, i)][0], " \n"[i == n]);
    return 0;
}
