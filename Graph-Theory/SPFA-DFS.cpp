/*
    SPFA-DFS
    题目大意：给出n个字符串，当两个字符串首尾有两个字母相同的时候就能可以相接，
    问能否接成一个串环，使得这个环上串的平均长度最长
    题解：我们将串的连接转化为首尾点的连接，边长为串长，那么问题转化为
    求一个平均长度最长的环，我们二分这个平均长度，用所有的边减去这个二分值，
    那么只要完成是否存在正环的判定即可，环的判定用SPFA-DFS效率比较高
*/
using namespace std;
typedef long long LL;
const double eps = 1e-3;
const int N = 100010;
int ed, head[N], u[N], v[N], nxt[N], cnt[N];
double d[N], w[N];
void init() {
    ed = 0;
    memset(head, -1, sizeof(head));

    void add(int a, int b, double c) {
        u[++ed] = a, v[ed] = b, w[ed] = c;
        nxt[ed] = head[u[ed]];
        head[u[ed]] = ed;
    }
    char s[N];
    const int M = 25 * 26 + 25 + 1;
    double mx;
    int f[M], tot, n;
    int vis[N], PC;
    void SPFA(int x, double ave) {
        if (PC) return;
        vis[x] = 1;
        for (int i = head[x]; i != -1; i = nxt[i]) {
            if (d[x] + w[i] - ave > d[v[i]] + eps) {
                d[v[i]] = d[x] + w[i] - ave;
                if (!vis[v[i]])
                    SPFA(v[i], ave);
                else {
                    PC = 1;
                    return;
                }
            }
        }
        vis[x] = 0;
    }
    bool Check(double x) {
        PC = 0;
        memset(vis, 0, sizeof(vis));
        memset(d, 0, sizeof(d));
        for (int i = 1; i <= tot; i++) {
            SPFA(i, x);
            if (PC) break;
        }
        return PC;
    }
    int main() {
        while (~scanf("%d", &n) && n) {
            init();
            tot = 0;
            memset(f, 0, sizeof(f));
            for (int i = 1; i <= n; i++) {
                scanf("%s", s + 1);
                int l = strlen(s + 1);
                int a = (s[1] - 'a') * 26 + s[2] - 'a';
                int b = (s[l - 1] - 'a') * 26 + s[l] - 'a';
                if (!f[a]) f[a] = ++tot;
                if (!f[b]) f[b] = ++tot;
                if (l > 1) add(f[a], f[b], l);
            }
            mx *= n;
            double ans = -1, l = 0, r = 1000;
            while (l + eps < r) {
                double mid = (l + r) * 0.5;
                if (Check(mid))
                    l = mid, ans = mid;
                else
                    r = mid;
            }
            if (ans == -1)
                puts("No solution.");
            else
                printf("%.3f\n", ans);
        }
        return 0;
    }