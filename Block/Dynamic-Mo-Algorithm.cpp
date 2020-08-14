/*
    带修改莫队
    给出一颜色序列，每次可以修改一个位置的颜色或者询问一个区间不同颜色的数目
*/
const int N = 1000010;
using namespace std;
int Ans, ans[N], pos[N], c[N], pre[N], sz, cnt, n, m, limit;
struct Q {
    int l, r, id, t;
    friend bool operator<(const Q &a, const Q &b) {
        if (pos[a.l] != pos[b.l]) return pos[a.l] < pos[b.l];
        if (a.r != b.r) return a.r < b.r;
        return a.t < b.t;
    }
} ask[N];
struct update {
    int pos, c, pre;
} w[N];
int in[N], num[N];
void cal(int x) {
    if (in[x]) {
        if (!--num[c[x]]) Ans--;
    } else {
        if (++num[c[x]] == 1) Ans++;
    }
    in[x] ^= 1;
}
void update(int x, int y) {
    if (in[x]) {
        cal(x);
        c[x] = y;
        cal(x);
    } else
        c[x] = y;
}
int main() {
    while (~scanf("%d%d", &n, &m)) {
        int limit = sqrt(n);
        for (int i = 1; i <= n; i++) {
            scanf("%d", &c[i]);
            pre[i] = c[i], pos[i] = (i - 1) / limit + 1, in[i] = 0;
        }
        cnt = sz = Ans = 0;
        for (int i = 1; i <= m; i++) {
            char op[10];
            scanf("%s", op);
            int x, y;
            scanf("%d%d", &x, &y);
            if (op[0] == 'R') {
                w[++cnt].pos = x;
                w[cnt].c = y;
                w[cnt].pre = pre[x];
                pre[x] = y;
            } else {
                ask[++sz].l = x;
                ask[sz].r = y;
                ask[sz].id = sz;
                ask[sz].t = cnt;
            }
        }
        sort(ask + 1, ask + sz + 1);
        int l, r;
        cal(l = r = 1);
        for (int i = 1; i <= sz; i++) {
            for (int j = ask[i - 1].t + 1; j <= ask[i].t; j++)
                update(w[j].pos, w[j].c);
            for (int j = ask[i - 1].t; j > ask[i].t; j--)
                update(w[j].pos, w[j].pre);
            while (l < ask[i].l) cal(l++);
            while (l > ask[i].l) cal(--l);
            while (r < ask[i].r) cal(++r);
            while (r > ask[i].r) cal(r--);
            ans[ask[i].id] = Ans;
        }
        for (int i = 1; i <= sz; i++) printf("%d\n", ans[i]);
    }
    return 0;
}