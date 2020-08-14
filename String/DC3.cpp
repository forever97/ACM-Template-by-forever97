/*
    DC3
    复杂度O(n)
    rk：该位置[0~len-1]的后缀排名
    sa：对应排名在原串[0~len-1]中的位置
    ht：排名相邻的后缀最长公共前缀[2~len]
*/
#define F(x) ((x) / 3 + ((x) % 3 == 1 ? 0 : ty))
#define G(x) ((x) < ty ? (x)*3 + 1 : ((x)-ty) * 3 + 2)
using namespace std;
const int N = 20100;
template <typename T = int>  // 应用时直接将T赋值为需要的类型速度比较快
struct SA {
    int str[N * 3], sa[N * 3], rk[N], ht[N], sz;
    int wa[N], wb[N], wv[N], ws[N];
    int &operator[](int k) { return sa[k]; }
    int size() const { return sz; }
    bool eq(const int *r, int a, int b) const {
        return r[a] == r[b] && r[a + 1] == r[b + 1] && r[a + 2] == r[b + 2];
    }
    bool cmp(const int *r, int a, int b, int d) const {
        if (d == 1)
            return (r[a] < r[b]) || (r[a] == r[b] && wv[a + 1] < wv[b + 1]);
        return (r[a] < r[b]) || (r[a] == r[b] && cmp(r, a + 1, b + 1, 1));
    }
    void rsort(const int *r, const int *a, int *b, int n, int m) {
        int i;
        fill(ws, ws + m, 0);
        for (i = 0; i < n; i++) ++ws[wv[i] = r[a[i]]];
        for (i = 1; i < m; i++) ws[i] += ws[i - 1];
        for (i = n - 1; ~i; i--) b[--ws[wv[i]]] = a[i];
    }
    void dc3(int *r, int *sa, int n, int m) {
        int i, j, k, *rn = r + n, *san = sa + n, tx = 0, ty = (n + 1) / 3,
                     tz = 0;
        r[n] = r[n + 1] = 0;
        for (i = 0; i < n; i++) {
            if (i % 3) wa[tz++] = i;
        }
        rsort(r + 2, wa, wb, tz, m);
        rsort(r + 1, wb, wa, tz, m);
        rsort(r, wa, wb, tz, m);
        for (rn[F(wb[0])] = 0, k = i = 1; i < tz; i++) {
            rn[F(wb[i])] = eq(r, wb[i - 1], wb[i]) ? k - 1 : k++;
        }
        if (k < tz)
            dc3(rn, san, tz, k);
        else {
            for (i = 0; i < tz; i++) san[rn[i]] = i;
        }
        for (i = 0; i < tz; i++) {
            if (san[i] < ty) wb[tx++] = san[i] * 3;
        }
        if (n % 3 == 1) wb[tx++] = n - 1;
        rsort(r, wb, wa, tx, m);
        for (i = 0; i < tz; i++) wv[wb[i] = G(san[i])] = i;
        for (i = j = k = 0; i < tx && j < tz; k++) {
            sa[k] = cmp(r, wa[i], wb[j], wb[j] % 3) ? wa[i++] : wb[j++];
        }
        for (; i < tx; i++) sa[k++] = wa[i];
        for (; j < tz; j++) sa[k++] = wb[j];
    }
    void build(const T *s, int n, int m = 128) {
        int i;
        for (i = 0; i < n; i++) str[i] = (int)s[i];
        str[n] = 0;
        sz = n + 1;
        dc3(str, sa, sz, m);
    }
    void CalHeight() {
        int i, j, k = 0;
        for (i = 0; i < sz; i++) rk[sa[i]] = i;
        for (i = 0; i < sz; ht[rk[i++]] = k) {
            for (k ? --k : 0, j = sa[rk[i] - 1]; str[i + k] == str[j + k]; k++)
                ;
        }
    }
};
/*
    Example
    求最长可允许重叠的出现次数不小于k的子串
*/
SA<int> Sa;
int n, s[N];
int k;
bool check(int x) {
    int num = 0;
    for (int i = 1; i <= n; i++) {
        if (Sa.ht[i] >= x)
            num++;
        else
            num = 1;
        if (num >= k) return 1;
    }
    return 0;
}
int main() {
    while (~scanf("%d%d", &n, &k)) {
        for (int i = 0; i < n; i++) scanf("%d", &s[i]);
        s[n] = 0;
        Sa.build(s, n, 128);
        Sa.CalHeight();
        int l = 0, r = n, ans = 0;
        while (l <= r) {
            int mid = (l + r) >> 1;
            if (check(mid))
                ans = mid, l = mid + 1;
            else
                r = mid - 1;
        }
        printf("%d\n", ans);
    }
    return 0;
}
