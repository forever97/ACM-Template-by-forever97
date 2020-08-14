/*
    题目大意：给出一个数列，有三种区间查询，
    分别查询区间是否存在两个数乘积为x，是否存在两个数和为x，以及是否存在两个数差为x，
    题解：我们对于询问进行莫队处理，保存当前区间的权值数组，记为F，
    同时保存权值数组的反向数组G
    那么存在差为x的情况只要存在一组F[i]&F[i-x]=1即可
    存在和为x的情况只要存在一组F[i]&G[M-x+i]即可。
    对于乘积为x的情况，我们枚举x的约数，判断F[i]&F[x/i]是否存在。
    复杂度O(nsqrt(n)+nm/w)
*/
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
using namespace std;
const int N = 100010, M = 100000;
int limit, n, m, pos[N], a[N], ans[N], cnt[N];
bitset<N> F, G;
struct Q {
    int l, r, x, id, op;
    friend bool operator<(const Q &a, const Q &b) {
        return pos[a.l] < pos[b.l] || (pos[a.l] == pos[b.l] && a.r < b.r);
    }
} ask[M];
int read(int &x) {
    int f = 1;
    char ch = getchar();
    x = 0;
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    x *= f;
}
int main() {
    read(n);
    read(m);
    limit = (int)sqrt(n + 0.5);
    for (int i = 1; i <= n; i++) read(a[i]), pos[i] = (i - 1) / limit + 1;
    for (int i = 1; i <= m; i++)
        read(ask[i].op), read(ask[i].l), read(ask[i].r), read(ask[i].x),
            ask[i].id = i;
    sort(ask + 1, ask + m + 1);
    for (int i = 1, l = 1, r = 0; i <= m; i++) {
        for (; r < ask[i].r; r++)
            cnt[a[r + 1]]++, F.set(a[r + 1]), G.set(M - a[r + 1]);
        for (; l > ask[i].l; l--)
            cnt[a[l - 1]]++, F.set(a[l - 1]), G.set(M - a[l - 1]);
        for (; l < ask[i].l; l++) {
            cnt[a[l]]--;
            if (!cnt[a[l]]) F.reset(a[l]), G.reset(M - a[l]);
        }
        for (; r > ask[i].r; r--) {
            cnt[a[r]]--;
            if (!cnt[a[r]]) F.reset(a[r]), G.reset(M - a[r]);
        }
        if (ask[i].op == 1) {
            if ((F & (F >> ask[i].x)).any()) ans[ask[i].id] = 1;
        } else if (ask[i].op == 2) {
            if ((F & (G >> (M - ask[i].x))).any()) ans[ask[i].id] = 1;
        } else {
            for (int j = 1; j * j <= ask[i].x; j++)
                if (ask[i].x % j == 0) {
                    if (F[j] & F[ask[i].x / j]) {
                        ans[ask[i].id] = 1;
                        break;
                    }
                }
        }
    }
    for (int i = 1; i <= m; i++) puts(ans[i] ? "yuno" : "yumi");
    return 0;
}
/*
    题目大意：给出一个数组a一个数组b，以及询问数组c，
    问对于每个c有多少对a%b=c，答案对2取模
    题解：考虑对2取模我们发现答案等价于数字的xor，01状态可以用bitset保存，
    在bitset上存a的权值数组，那么每次只要将b*i~b*(i+1)-1的数值xor到答案数组的0~b-1上去即可，
    鉴于bitset没有截取区间的功能，我们手写压位，
    考虑压32位之后不是32倍数的部分截取起来非常尴尬，
    因此我们保存其偏移量为0~31的数组，这样取区间就比较方便了。
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 10010;
struct Bitset {
    unsigned int u[N];
    void reset() { memset(u, 0, sizeof(u)); }
    void set(int x) { u[x >> 5] |= 1 << (x & 31); }
    void flip(int x) { u[x >> 5] ^= 1 << (x & 31); }
    bool test(int x) { return u[x >> 5] & (1 << (x & 31)); }
    void reset(int x) {
        if (test(x)) flip(x);
    }
} a[32], ans;
void Solve(int l, int r) {
    while ((r - l) & 31) {
        r--;
        if (a[0].test(r)) ans.flip(r - l);
    }
    int m = 0;
    while (l & 31) l++, r++, m++;
    l >>= 5;
    r >>= 5;
    for (int i = l; i < r; i++) ans.u[i - l] ^= a[m].u[i];
}
int T, n, m, q, x, mx;
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d%d", &n, &m, &q);
        for (int i = 0; i < 32; i++) a[i].reset();
        ans.reset();
        mx = 0;
        for (int i = 1; i <= n; i++) {
            scanf("%d", &x);
            mx = max(x, mx);
            for (int j = 0; j < 32; j++) a[j].set(x + j);
        }
        for (int i = 1; i <= m; i++) {
            scanf("%d", &x);
            for (int j = 0; j <= mx; j += x) Solve(j, min(mx + 1, j + x));
        }
        while (q--) {
            scanf("%d", &x);
            if (ans.test(x))
                puts("1");
            else
                puts("0");
        }
    }
    return 0;
}
/*
    题目大意：
        给出一个数列，每个询问给出三个区间，问除去三个区间共有的数字外，
        还剩下几个数字，注意删去的是共有的数字个数，不是数字种类，统计时候也一样
    题解：
        首先，答案为区间长度和减去区间并数字个数和的三倍。
        所以题目转化为求区间并。很显然在开始对数据可以进行离散化。
        考虑每个数字只出现一次的情况，我们可以用bitset来统计区间某个数字是否存在，
        莫队处理查询每个区间，保存其bitset的值，最后求交即可，
        现在考虑每个数字出现多次的情况，
        我们发现经过离散的数据之间空位数量恰好可以用来标出现多次的数据，
        比如1 4 4 9 9，离散后为 1 2 2 4 4，
        我们可以将多出来的2标在3位置，4标在5位置，那么就可以用bitset统计了。
        -  Me : 询问区间存不下怎么办？
        -  Claris ：将询问分批进行处理，单次处理25000个询问
        -  Me : 超时了欸……
        -  Claris : 这题卡常数，要手写bitset.
        -  Me : 你的代码为什么有6.7k？
        -  Claris ：我分出现一次，两次和跟多次讨论
        -  Me : 我……还是咸鱼吧……
*/
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
using namespace std;
typedef unsigned long long ULL;
const int N = 100010, M = N << 2;
int limit, n, m, pos[N], a[N], cnt[N], Ans[N], mark[N];
struct Q {
    int l, r, id;
    friend bool operator<(const Q &a, const Q &b) {
        return pos[a.l] < pos[b.l] || (pos[a.l] == pos[b.l] && a.r < b.r);
    }
} ask[M];
int read(int &x) {
    int f = 1;
    char ch = getchar();
    x = 0;
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    x *= f;
}
int disc[N];
int remark(int x) {
    int l = 1, r = n, res = 0;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (disc[mid] < x)
            l = mid + 1;
        else
            res = mid, r = mid - 1;
    }
    return res;
}
const int B = 1567, K = 25000;
ULL v[B], f[K + 3][B];
int u[65537], tmp, U;
void flip(int x) { v[x >> 6] ^= 1ULL << (x & 63); }
void Copy(ULL *a) {
    int i = 0;
    for (; i + 13 <= U; i += 14) {
        for (int j = 0; j < 14; j++) a[i + j] = v[i + j];
    }
    for (; i <= U; i++) a[i] = v[i];
}
void And(ULL *a) {
    int i = 0;
    for (; i + 13 <= U; i += 14) {
        for (int j = 0; j < 14; j++) a[i + j] &= v[i + j];
    }
    for (; i <= U; i++) a[i] &= v[i];
}
void popcount(ULL x) {
    tmp += u[x & 65535] + u[x >> 16 & 65535] + u[x >> 32 & 65535] + u[x >> 48];
}
int count(ULL *a) {
    int i = tmp = 0;
    for (; i + 13 <= U; i += 14) {
        for (int j = 0; j < 14; j++) popcount(a[i + j]);
    }
    for (; i <= U; i++) popcount(a[i]);
    return tmp;
}
void init() {
    for (int i = 1; i < 65536; i++) u[i] = u[i >> 1] + (i & 1);
}
int main() {
    read(n);
    read(m);
    U = n >> 6;
    init();
    limit = (int)sqrt(n + 0.5);
    for (int i = 1; i <= n; i++)
        read(a[i]), disc[i] = a[i], pos[i] = (i - 1) / limit + 1;
    sort(disc + 1, disc + n + 1);
    for (int i = 1; i <= n; i++) a[i] = remark(a[i]);
    int pos = 0, l = 1, r = 0;
    while (pos < m) {
        int tot = 0;
        for (int i = 1; i <= 25000 && i + pos <= m; i++) {
            tot += 3;
            Ans[i] = 0;
            mark[i] = 0;
            read(ask[i * 3 - 2].l);
            read(ask[i * 3 - 2].r);
            ask[i * 3 - 2].id = i;
            read(ask[i * 3 - 1].l);
            read(ask[i * 3 - 1].r);
            ask[i * 3 - 1].id = i;
            read(ask[i * 3].l);
            read(ask[i * 3].r);
            ask[i * 3].id = i;
            Ans[i] += ask[i * 3 - 2].r - ask[i * 3 - 2].l + 1;
            Ans[i] += ask[i * 3 - 1].r - ask[i * 3 - 1].l + 1;
            Ans[i] += ask[i * 3].r - ask[i * 3].l + 1;
        }
        sort(ask + 1, ask + tot + 1);
        for (int i = 1; i <= tot; i++) {
            for (; r < ask[i].r; r++) {
                flip(a[r + 1] + cnt[a[r + 1]]);
                cnt[a[r + 1]]++;
            }
            for (; l > ask[i].l; l--) {
                flip(a[l - 1] + cnt[a[l - 1]]);
                cnt[a[l - 1]]++;
            }
            for (; l < ask[i].l; l++) {
                cnt[a[l]]--;
                flip(a[l] + cnt[a[l]]);
            }
            for (; r > ask[i].r; r--) {
                cnt[a[r]]--;
                flip(a[r] + cnt[a[r]]);
            }
            if (mark[ask[i].id])
                And(f[ask[i].id]);
            else
                Copy(f[ask[i].id]), mark[ask[i].id] = 1;
        }
        tot /= 3;
        for (int i = 1; i <= tot; i++) printf("%d\n", Ans[i] - 3 * count(f[i]));
        pos += tot;
    }
    return 0;
}
