/* 差分约束:
 *   按照A-B<=C建图，或者转化B-A>C为A-B<=-C建图
 *   用最短路来松弛条件，固定起点值可从无穷向下约束得到其余点最大值，
 *   按照A-B>=C建图，用最长路来松弛条件，从负无穷往上约束可得最小值
 *   若图存在负环则表明无解，d为正负INF表明解无穷大或者无穷小
 *   对于非连通图的可行性判断利用加入超级源点的方法可以实现
 *   特别注意dijkstra算法不能处理有负权边的情况
 * Example
 *   题意：求是否存在一个数列，满足给定m个区间和的取值范围，
 *   题解：对于区间和，我们可以转化为前缀和的点差关系，
 *   根据小于等于建图，若图中不存在负环，说明在该约束条件下有解，
 *   由于图未必连通，所以需要建立超级源点，存在负环的情况下，
 *   用stack代替queue在实际运作中会快很多
*/
using namespace std;
const int N = 210;
int ed, d[N], head[N], u[N], v[N], w[N], nxt[N], cnt[N];
const int INF = ~0U >> 2;
void init() {
    ed = 0;
    memset(head, -1, sizeof(head));
}
void add(int a, int b, int c) {
    u[++ed] = a, v[ed] = b, w[ed] = c;
    nxt[ed] = head[u[ed]];
    head[u[ed]] = ed;
}
bool SPFA(int s, int n) {
    for (int i = 0; i <= n; i++) d[i] = INF;
    d[s] = 0;
    memset(cnt, 0, sizeof(cnt));
    bool inq[N] = {0};
    stack<int> Q;
    Q.push(s);
    while (!Q.empty()) {
        int x = Q.top();
        Q.pop();
        inq[x] = 0;
        for (int i = head[x]; i != -1; i = nxt[i]) {
            if (d[v[i]] <= d[x] + w[i]) continue;
            d[v[i]] = d[x] + w[i];
            if (!inq[v[i]]) {
                inq[v[i]] = 1;
                Q.push(v[i]);
                if (++cnt[v[i]] > n) return 0;
            }
        }
    }
    return 1;
}
int n, m;
int main() {
    while (scanf("%d", &n) && n) {
        scanf("%d", &m);
        init();
        char op[10];
        int x, y, z;
        while (m--) {
            scanf("%d %d %s %d", &x, &y, op, &z);
            if (op[0] == 'g')
                add(x + y + 1, x, -z - 1);
            else
                add(x, x + y + 1, z - 1);
        }
        for (int i = 1; i <= n + 1; i++) add(0, i, 0);
        if (SPFA(0, n + 1))
            puts("lamentable kingdom");
        else
            puts("successful conspiracy");
    }
    return 0;
}
