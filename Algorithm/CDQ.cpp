/*
    CDQ分治
    三维数点
    将区间整体按x排序，
    在每个区间完成操作将区间按照y排序，
    那么两个合并的区间可以保证每个区间y是单调递增的
    而显然右区间x要大于左区间，
    所以只要将y小于右区间当前统计点的左区间的点的z坐标全部加入数据结构
    然后在数据结构上进行相应的统计就可以了。
*/
const int N = 100005, M = 100001;
struct node {
    int x, y, z, t;
} q[N];
bool cmpx(node a, node b) { return a.x < b.x; }
bool cmpy(node a, node b) { return a.y < b.y; }
void read(int& a) {
    char ch;
    while (!((ch = getchar()) >= '0') && (ch <= '9'))
        ;
    a = ch - '0';
    while (((ch = getchar()) >= '0') && (ch <= '9')) a *= 10, a += ch - '0';
}
int n, ans[N], T, s[N], c[N];
int add(int x) {
    while (x < M) {
        if (c[x] != T) c[x] = T, s[x] = 0;
        s[x]++, x += x & -x;
    }
}
int query(int x) {
    int sum = 0;
    while (x) {
        if (c[x] != T) c[x] = T, s[x] = 0;
        sum += s[x], x -= x & -x;
    }
    return sum;
}
void CDQ(int l, int r) {
    if (l == r) return;
    int mid = (l + r) >> 1, tmp = q[mid].x;
    CDQ(l, mid);
    CDQ(mid + 1, r);
    T++;
    int L = l, R = mid + 1;
    while (L <= mid || R <= r) {
        if (L > mid)
            ans[q[R].t] += query(q[R].z), R++;
        else if (R > r)
            add(q[L].z), L++;
        else if (q[L].y <= q[R].y)
            add(q[L].z), L++;
        else
            ans[q[R].t] += query(q[R].z), R++;
    }
    T++;
    L = l, R = mid + 1;
    while (L <= mid || R <= r) {
        if (L > mid) {
            if (q[R].x == tmp) add(q[R].z);
            R++;
        } else if (R > r) {
            if (q[L].x == tmp) ans[q[L].t] += query(q[L].z);
            L++;
        } else if (q[L].y >= q[R].y) {
            if (q[R].x == tmp) add(q[R].z);
            R++;
        } else {
            if (q[L].x == tmp) ans[q[L].t] += query(q[L].z);
            L++;
        }
    }
    sort(q + l, q + r + 1, cmpy);
}
void init() {
    T = 0;
    memset(c, 0, sizeof(c));
    memset(ans, 0, sizeof(ans));
}
int main(int cas) {
    read(cas);
    while (cas--) {
        read(n);
        init();
        for (int i = 1; i <= n; i++) {
            read(q[i].x), read(q[i].y), read(q[i].z);
            q[i].t = i;
        }
        sort(q + 1, q + n + 1, cmpx);
        CDQ(1, n);
        for (int i = 1; i <= n; i++) printf("%d\n", ans[i]);
    }
    return 0;
}