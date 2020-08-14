/*
    ask(x): 查询与集合标识的距离奇偶性
    合并操作: Union(fx,fy,ask(x)^ask(y)^1)
    撤回操作：back(Tim)，回退到时间Tim
    查询复杂度 O(logn)
*/
namespace Union_Find_Set {
int st[N], top, f[N], val[N], d[N];
void Initialize(int n) {
    for (int i = 1; i <= n; i++) f[i] = i, val[i] = 0, d[i] = 1;
    top = 0;
}
int sf(int x) { return f[x] == x ? x : sf(f[x]); }
int ask(int x) {
    int res = 0;
    for (; x != f[x]; x = f[x]) res ^= val[x];
    return res;
}
void back(int tag) {
    for (; top != tag; top--) {
        if (st[top] < 0)
            d[-st[top]]--;
        else {
            f[st[top]] = st[top];
            val[st[top]] = 0;
        }
    }
}
void Union(int x, int y, int _val) {
    if (d[x] > d[y]) swap(x, y);
    if (d[x] == d[y]) d[y]++, st[++top] = -y;
    f[x] = y;
    val[x] = _val;
    st[++top] = x;
}
}  // namespace Union_Find_Set
/*
    Test
    给出一张图，有些边只存在一段时间，
    问在一个每个时间段，这张图是否是二分图
*/
using namespace Union_Find_Set;
struct data {
    int x, y, l, r;
} E[N];
void dfs(int l, int r, int pos) {
    int t = top;
    for (int i = 1; i <= pos; i++) {
        int x = E[i].x, y = E[i].y;
        if (E[i].l <= l && E[i].r >= r) {
            int fx = sf(x), fy = sf(y);
            int val = ask(x) ^ ask(y) ^ 1;
            if (fx == fy) {
                if (val) {
                    for (int j = l; j <= r; j++) puts("No");
                    back(t);
                    return;
                }
            }
            Union(fx, fy, val);
            swap(E[i--], E[pos--]);
        }
    }
    if (l == r) {
        puts("Yes");
        back(t);
        return;
    }
    int mid = (l + r) >> 1, ppos = pos;
    for (int i = 1; i <= ppos; i++) {
        if (E[i].l > mid) swap(E[i--], E[ppos--]);
    }
    dfs(l, mid, ppos);
    ppos = pos;
    for (int i = 1; i <= ppos; i++) {
        if (E[i].r <= mid) swap(E[i--], E[ppos--]);
    }
    dfs(mid + 1, r, ppos);
    back(t);
}
/*
    对时间进行分治，在操作树上加删边，
    保留涵盖时间区间的有效操作，将剩余操作按时间划分到两端的子树，
    退出子树的时候撤销加边操作。
    对于判断奇环，我们用并查集维护每个点与标兵的相对距离的奇偶性即可
*/
int n, m, op, x, y, l, r, T;
int main() {
    scanf("%d%d%d", &n, &m, &T);
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d%d", &x, &y, &l, &r);
        E[i] = (data){x, y, ++l, r};
    }
    Initialize(n);
    for (int i = 1; i <= m; i++)
        if (E[i].l > E[i].r) swap(E[i], E[m--]);
    dfs(1, T, m);
    return 0;
}