/*
    树状数组
    查询[1,n]第k大
    查询复杂度O(logn)
*/
namespace BIT {
int n, c[N];
void Initialize(int _n) {
    n = _n;
    memset(c, 0, (n + 1) * sizeof(int));
}
void add(int x, int val) {
    while (x <= n) c[x] += val, x += x & -x;
}
void query(int x) {
    int res = 0;
    while (x) res += c[x], x -= x & -x;
    return res;
}
int findkth(int k) {
    int m = 31 - _builtin_clz(n);
    int cnt = 0, res = 0;
    for (int i = m; i >= 0; i--) {
        res += 1 << i;
        if (res >= n || cnt + c[res] >= k)
            res -= 1 << i;
        else
            cnt += c[res];
    }
    return res + 1;
}
}  // namespace BIT
/*
    1: 前缀最大值
    2：后缀最大值
    更新操作均为add(x,val[x])
*/
void add1(int x, int val) {
    while (x <= n) c1[x] = max(val, c1[x]), x += x & -x;
}
int query1(int x) {
    int res = 0;
    while (x) res = max(res, c1[x]), x -= x & -x;
    return res;
}
void add2(int x, int val) {
    while (x) c2[x] = max(c2[x], val), x -= x & -x;
}
int query2(int x) {
    int res = 0;
    while (x <= n) res = max(res, c2[x]), x += x & -x;
    return res;
}