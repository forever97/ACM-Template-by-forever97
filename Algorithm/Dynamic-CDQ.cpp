/*
    动态CDQ分治
    Example：
        给一张二维表，要求支持两种操作，查询矩阵和以及增加单个格子的数值
    Solution：
        对于区间询问拆点
        1.读入询问，按x排序
        2.将[L,R]中的数分为前部分操作，后部分操作（各部分仍保持X升序）
        3.将前面对后面的影响记录ans
        4.复原影响
        5.递归[L,M],[M+1,R]
*/
int m, x1, x2, y1, y2, u, opt, ans[200010], c[500010], M;
struct data {
    int x, y, val, pos, id, opt;
} q[800010], tmp[800010];
bool operator<(data a, data b) {
    if (a.x == b.x && a.y == b.y)
        return a.opt < b.opt;
    else
        return (a.x == b.x ? a.y < b.y : a.x < b.x);
}
void addquery() {
    scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
    int pos = ++ans[0];
    ans[pos] = (x2 - x1 + 1) * (y2 - y1 + 1) * u;
    q[++m].pos = pos;
    q[m].id = m;
    q[m].x = x1 - 1;
    q[m].y = y1 - 1;
    q[m].val = 1;
    q[m].opt = 1;
    q[++m].pos = pos;
    q[m].id = m;
    q[m].x = x2;
    q[m].y = y2;
    q[m].val = 1;
    q[m].opt = 1;
    q[++m].pos = pos;
    q[m].id = m;
    q[m].x = x1 - 1;
    q[m].y = y2;
    q[m].val = -1;
    q[m].opt = 1;
    q[++m].pos = pos;
    q[m].id = m;
    q[m].x = x2;
    q[m].y = y1 - 1;
    q[m].val = -1;
    q[m].opt = 1;
}
void addmodify() {
    ++m;
    scanf("%d%d%d", &q[m].x, &q[m].y, &q[m].val);
    q[m].id = m;
}
void update(int x, int val) {
    while (x <= M) c[x] += val, x += x & -x;
}
int query(int x) {
    int s = 0;
    while (x) s += c[x], x -= x & -x;
    return s;
}
void CDQ(int l, int r) {
    if (l == r) return;
    int mid = (l + r) >> 1, l1 = l, l2 = mid + 1;
    for (int i = l; i <= r; i++) {
        if (q[i].id <= mid && !q[i].opt) update(q[i].y, q[i].val);
        if (q[i].id > mid && q[i].opt)
            ans[q[i].pos] += q[i].val * query(q[i].y);
    }
    for (int i = l; i <= r; i++)
        if (q[i].id <= mid && !q[i].opt) update(q[i].y, -q[i].val);
    for (int i = l; i <= r; i++)
        if (q[i].id <= mid)
            tmp[l1++] = q[i];
        else
            tmp[l2++] = q[i];
    for (int i = l; i <= r; i++) q[i] = tmp[i];
    CDQ(l, mid);
    CDQ(mid + 1, r);
}
int main() {
    scanf("%d", &M);
    while (1) {
        scanf("%d", &opt);
        if (opt == 1)
            addmodify();
        else if (opt == 2)
            addquery();
        else
            break;
    }
    sort(q + 1, q + m + 1);
    CDQ(1, m);
    for (int i = 1; i <= ans[0]; i++) printf("%d\n", ans[i]);
    return 0;
}