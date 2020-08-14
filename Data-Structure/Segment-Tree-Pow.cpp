/*
    线段树区间求幂
    操作
        1. 查询区间乘积
        2. 区间每个数变为原来的k次
        3. 区间每个数乘k
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int mod = 1e9 + 7;
const int M = 400010;
void read(int &u) {
    int x = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    u = x * f;
}
int tot;
struct node {
    int l, r, val, a, b, ta, tb, L;
} T[M];
void build(int, int);
inline void Initialize(int n) {
    tot = 0;
    build(1, n);
}
inline int pow(int a, int b, int mod) {
    int res = 1;
    while (b) {
        if (b & 1) res = 1LL * res * a % mod;
        b >>= 1;
        a = 1LL * a * a % mod;
    }
    return res;
}
inline void addtag(int x, int a, int b) {
    T[x].val = 1LL * pow(T[x].val, b, mod) * pow(a, T[x].L, mod) % mod;
    T[x].ta = 1LL * pow(T[x].ta, b, mod) * a % mod;
    T[x].tb = 1LL * T[x].tb * b % (mod - 1);
}
inline void pb(int x) {
    if (T[x].l) {
        addtag(T[x].l, T[x].ta, T[x].tb);
        addtag(T[x].r, T[x].ta, T[x].tb);
    }
    T[x].ta = T[x].tb = 1;
}
inline void up(int x) {
    T[x].val = (1LL * T[T[x].l].val * T[T[x].r].val) % mod;
}
inline void build(int l, int r) {
    int x = ++tot;
    T[x].a = l;
    T[x].b = r;
    T[x].ta = T[x].tb = 1;
    T[x].l = T[x].r = 0;
    T[x].L = T[x].b - T[x].a + 1;
    if (l == r) {
        scanf("%d", &T[x].val);
        return;
    }
    int mid = (l + r) >> 1;
    T[x].l = tot + 1;
    build(l, mid);
    T[x].r = tot + 1;
    build(mid + 1, r);
    up(x);
}
inline void change(int x, int a, int b, int m, int k) {
    if (T[x].a >= a && T[x].b <= b) {
        addtag(x, m, k);
        return;
    }
    if (T[x].ta != 1 || T[x].tb != 1) pb(x);
    int mid = (T[x].a + T[x].b) >> 1;
    if (mid >= a && T[x].l) change(T[x].l, a, b, m, k);
    if (mid < b && T[x].r) change(T[x].r, a, b, m, k);
    up(x);
}
inline int query(int x, int a, int b) {
    if (T[x].a >= a && T[x].b <= b) return T[x].val;
    if (T[x].ta != 1 || T[x].tb != 1) pb(x);
    int mid = (T[x].a + T[x].b) >> 1;
    int res = 1;
    if (mid >= a && T[x].l) res = 1LL * res * query(T[x].l, a, b) % mod;
    if (mid < b && T[x].r) res = 1LL * res * query(T[x].r, a, b) % mod;
    return res;
}
int cas, n, q, op, x, y, z;
int main() {
    read(cas);
    while (cas--) {
        read(n);
        read(q);
        Initialize(n);
        while (q--) {
            read(op);
            if (op == 1) {
                read(x);
                read(y);
                read(z);
                change(1, x, y, z, 1);
            } else if (op == 2) {
                read(x);
                read(y);
                read(z);
                change(1, x, y, 1, z);
            } else {
                read(x);
                read(y);
                printf("%d\n", query(1, x, y));
            }
        }
    }
    return 0;
}
/*
    指标法+线段树
    操作：
        1. 查询区间乘积
        2. 区间每个数变为原来的k次，k<=1e9
        3. 区间每个数乘k，k<=1e3
    所有初始数据小于等于1e3
    题解：我们对1-1000预处理指标I(x)，
    有I(x*y%P)=(I(x)+I(y))%(P-1)，I((x^k)%P)=k*I(x)%(P-1)
    所以只要用线段树维护乘法和加法即可，
    求指标的时候，BSGS的步长=询问*P/(步长)，所以我们求得步长为1000000为佳，
    考虑常数情况，调整至300000。
*/
#include <algorithm>
#include <cstdio>
#include <tr1/unordered_map>
using namespace std;
using namespace std::tr1;
const int P = 1000000007, Q = P - 1;
const int M = 400010;
void read(int &u) {
    int x = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    u = x * f;
}
inline int pow(int a, int b, int mod) {
    int res = 1;
    while (b) {
        if (b & 1) res = 1LL * res * a % mod;
        b >>= 1;
        a = 1LL * a * a % mod;
    }
    return res;
}
namespace NT {
unordered_map<int, int> T;
typedef pair<int, int> PI;
const int K = 300000, G = 5;  // 1e9+7的原根为5
int ind[1010], base, i;
inline int ask(int x) {
    if (x == 1) return 0;
    int t = pow(x, P - 2, P);
    for (int i = K;; i += K) {
        t = 1LL * t * base % P;
        unordered_map<int, int>::iterator it = T.find(t);
        if (it != T.end()) return i - it->second;
    }
}
void init() {
    for (base = 1, i = 0; i < K; i++) {
        T.insert(PI(base, i));
        base = 1LL * base * G % P;
    }
    for (i = 1; i <= 1000; i++) ind[i] = ask(i);
}
}  // namespace NT
int tot;
struct node {
    int l, r, val, a, b, ta, tb, L;
} T[M];
void build(int, int);
inline void Initialize(int n) {
    tot = 0;
    build(1, n);
}
inline void addtag(int x, int a, int b) {
    T[x].val = (1LL * T[x].val * b + 1LL * a * T[x].L) % Q;
    T[x].ta = (1LL * T[x].ta * b + a) % Q;
    T[x].tb = 1LL * T[x].tb * b % Q;
}
inline void pb(int x) {
    if (T[x].l) {
        addtag(T[x].l, T[x].ta, T[x].tb);
        addtag(T[x].r, T[x].ta, T[x].tb);
    }
    T[x].ta = 0;
    T[x].tb = 1;
}
inline void up(int x) { T[x].val = (T[T[x].l].val + T[T[x].r].val) % Q; }
inline void build(int l, int r) {
    int x = ++tot;
    T[x].a = l;
    T[x].b = r;
    T[x].ta = 0;
    T[x].tb = 1;
    T[x].l = T[x].r = 0;
    T[x].L = T[x].b - T[x].a + 1;
    if (l == r) {
        read(T[x].val);
        T[x].val = NT::ind[T[x].val];
        return;
    }
    int mid = (l + r) >> 1;
    T[x].l = tot + 1;
    build(l, mid);
    T[x].r = tot + 1;
    build(mid + 1, r);
    up(x);
}
inline void change(int x, int a, int b, int m, int k) {
    if (T[x].a >= a && T[x].b <= b) {
        addtag(x, m, k);
        return;
    }
    if (T[x].ta != 0 || T[x].tb != 1) pb(x);
    int mid = (T[x].a + T[x].b) >> 1;
    if (mid >= a && T[x].l) change(T[x].l, a, b, m, k);
    if (mid < b && T[x].r) change(T[x].r, a, b, m, k);
    up(x);
}
inline int query(int x, int a, int b) {
    if (T[x].a >= a && T[x].b <= b) return T[x].val;
    if (T[x].ta != 0 || T[x].tb != 1) pb(x);
    int mid = (T[x].a + T[x].b) >> 1;
    int res = 0;
    if (mid >= a && T[x].l) res = (res + query(T[x].l, a, b)) % Q;
    if (mid < b && T[x].r) res = (res + query(T[x].r, a, b)) % Q;
    return res;
}
int cas, n, q, op, x, y, z;
int main() {
    NT::init();
    read(cas);
    while (cas--) {
        read(n);
        read(q);
        Initialize(n);
        while (q--) {
            read(op);
            if (op == 1) {
                read(x);
                read(y);
                read(z);
                change(1, x, y, NT::ind[z], 1);
            } else if (op == 2) {
                read(x);
                read(y);
                read(z);
                change(1, x, y, 0, z);
            } else {
                read(x);
                read(y);
                printf("%d\n", pow(NT::G, query(1, x, y), P));
            }
        }
    }
    return 0;
}
