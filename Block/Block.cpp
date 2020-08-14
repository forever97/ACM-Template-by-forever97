/*
    分块
    区间修改+区间查询大于等于c的数字个数
    O(qsqrt(n)log(sqrt(n))
*/
int n, m, q, block;
const int N = 1000010;
int a[N], b[N], pos[N], add[N];
void Sortblock(int x) {
    int l = (x - 1) * block + 1, r = min(x * block, n);
    for (int i = l; i <= r; i++) b[i] = a[i];
    sort(b + l, b + r + 1);
}
int find(int x, int v) {
    int l = (x - 1) * block + 1, r = min(x * block, n);
    int R = r;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (b[mid] < v)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return R - l + 1;
}
void update(int x, int y, int v) {
    if (pos[x] == pos[y]) {
        for (int i = x; i <= y; i++) a[i] += v;
    } else {
        for (int i = x; i <= pos[x] * block; i++) a[i] += v;
        for (int i = (pos[y] - 1) * block + 1; i <= y; i++) a[i] += v;
    }
    Sortblock(pos[x]);
    Sortblock(pos[y]);
    for (int i = pos[x] + 1; i < pos[y]; i++) add[i] += v;
}
int query(int x, int y, int v) {
    int res = 0;
    if (pos[x] == pos[y]) {
        for (int i = x; i <= y; i++)
            if (a[i] + add[pos[i]] >= v) res++;
    } else {
        for (int i = x; i <= pos[x] * block; i++)
            if (a[i] + add[pos[i]] >= v) res++;
        for (int i = (pos[y] - 1) * block + 1; i <= y; i++)
            if (a[i] + add[pos[i]] >= v) res++;
    }
    for (int i = pos[x] + 1; i < pos[y]; i++) res += find(i, v - add[i]);
    return res;
}
int main() {
    while (~scanf("%d%d", &n, &q)) {
        block = int(sqrt(n));
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            pos[i] = (i - 1) / block + 1;
            b[i] = a[i];
        }
        if (n % block)
            m = n / block + 1;
        else
            m = n / block;
        for (int i = 1; i <= m; i++) Sortblock(i);
        for (int i = 1; i <= q; i++) {
            char op[5];
            int x, y, v;
            scanf("%s%d%d%d", op, &x, &y, &v);
            if (op[0] == 'M')
                update(x, y, v);
            else
                printf("%d\n", query(x, y, v));
        }
    }
    return 0;
}