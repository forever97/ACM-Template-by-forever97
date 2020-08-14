/*
    动态逆序对
    分块+权值树状数组
    一个1到n顺序排列的数列，每次选择两个位置的数进行交换，
    求交换后的数列的逆序对数
*/
const int N = 200010, M = 650;
int n, q, l, r, x, y, arr[N], st[M], en[M], c[M][N], block[N];
void update(int k, int x, int val) {
    while (x < N) c[k][x] += val, x += x & -x;
}
int query(int k, int x) {
    int s = 0;
    while (x) s += c[k][x], x -= x & -x;
    return s;
}
long long ans = 0;
int query(int l, int r, int x) {
    int res = 0;
    for (int i = block[l]; i <= block[r]; i++) {
        if (st[i] >= l && en[i] <= r)
            res += query(i, x);
        else
            for (int j = max(st[i], l); j <= min(en[i], r); j++)
                res += (arr[j] <= x);
    }
    return res;
}
void cal(int x, int y) {
    ans -= x - 1 - query(1, x - 1, arr[x]);
    ans -= query(x + 1, n, arr[x] - 1);
    update(block[x], y, 1);
    update(block[x], arr[x], -1);
    arr[x] = y;
    ans += x - 1 - query(1, x - 1, arr[x]);
    ans += query(x + 1, n, arr[x] - 1);
}
int main() {
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++) arr[i] = i;
    for (int i = 1, cur = 0; i <= n;) {
        int j = i;
        st[++cur] = i;
        while (j <= n && j < i + M) {
            block[j] = cur;
            update(cur, arr[j++], 1);
        }
        en[cur] = j - 1;
        i = j;
    }
    while (q--) {
        scanf("%d%d", &l, &r);
        if (l != r) {
            int t = arr[l];
            x = l;
            y = arr[r];
            cal(x, y);
            x = r;
            y = t;
            cal(x, y);
        }
        printf("%lld\n", ans);
    }
    return 0;
}