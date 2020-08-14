/*
    多项式
    乘法 NTT之后相乘
    开方 getroot
    求逆(倒数) getinv
    求对数函数 getln
    求指数函数 getexp
    求幂 getpow
    初始化 init
    拉格朗日反演 [x^n]F(x)=[x^(n-1)]*(1/n)*(x/G(x))^n
*/
#include <cstdio>
typedef long long ll;
const int N = 262144, K = 17;  // N = 1 << (K + 1)
int n, m, i, k;
int a[N + 10], b[N + 10], tmp[N + 10], tmp2[N + 10];
int P = 998244353, G = 3, g[K + 1], ng[K + 10], inv[N + 10], inv2;
int power(int a, int b) {
    int t = 1;
    for (; b; b >>= 1, a = (ll)a * a % P)
        if (b & 1) t = (ll)t * a % P;
    return t;
}
void NTT(int* a, int n, int t) {
    for (int i = 1, j = 0; i < n - 1; i++) {
        for (int s = n; j ^= s >>= 1, ~j & s;)
            ;
        if (i < j) {
            int k = a[i];
            a[i] = a[j];
            a[j] = k;
        }
    }
    for (int d = 0; (1 << d) < n; d++) {
        int m = 1 << d, m2 = m << 1, _w = t == 1 ? g[d] : ng[d];
        for (int i = 0; i < n; i += m2)
            for (int w = 1, j = 0; j < m; j++) {
                int &A = a[i + j + m], &B = a[i + j], t = (ll)w * A % P;
                A = B - t;
                if (A < 0) A += P;
                B = B + t;
                if (B >= P) B -= P;
                w = (ll)w * _w % P;
            }
    }
    if (t == -1)
        for (int i = 0, j = inv[n]; i < n; i++) a[i] = (ll)a[i] * j % P;
}
// 给定a, 求a的逆元b
void getinv(int* a, int* b, int n) {
    if (n == 1) {
        b[0] = power(a[0], P - 2);
        return;
    }
    getinv(a, b, n >> 1);
    int k = n << 1, i;
    for (i = 0; i < n; i++) tmp[i] = a[i];
    for (i = n; i < k; i++) tmp[i] = b[i] = 0;
    NTT(tmp, k, 1), NTT(b, k, 1);
    for (i = 0; i < k; i++) {
        b[i] = (ll)b[i] * (2 - (ll)tmp[i] * b[i] % P) % P;
        if (b[i] < 0) b[i] += P;
    }
    NTT(b, k, -1);
    for (i = n; i < k; i++) b[i] = 0;
}
// 给定a, 求a的对数函数, 且a[0] = 1
void getln(int* a, int* b, int n) {
    getinv(a, tmp2, n);
    int k = n << 1, i;
    for (i = 0; i < n - 1; i++) b[i] = (ll)a[i + 1] * (i + 1) % P;
    for (i = n - 1; i < k; i++) b[i] = 0;
    NTT(b, k, 1), NTT(tmp2, k, 1);
    for (i = 0; i < k; i++) b[i] = (ll)b[i] * tmp2[i] % P;
    NTT(b, k, -1);
    for (i = n - 1; i; i--) b[i] = (ll)b[i - 1] * inv[i] % P;
    b[0] = 0;
}
// 给定a, 求a的指数函数, 且a[0]=0
void getexp(int* a, int* b, int n) {
    if (n == 1) {
        b[0] = 1;
        return;
    }
    getexp(a, b, n >> 1);
    getln(b, tmp, n);
    int k = n << 1, i;
    for (i = 0; i < n; i++) {
        tmp[i] = a[i] - tmp[i];
        if (tmp[i] < 0) tmp[i] += P;
    }
    if ((++tmp[0]) == P) tmp[0] = 0;
    for (i = n; i < k; i++) tmp[i] = b[i] = 0;
    NTT(tmp, k, 1), NTT(b, k, 1);
    for (i = 0; i < k; i++) b[i] = (ll)b[i] * tmp[i] % P;
    NTT(b, k, -1);
    for (i = n; i < k; i++) b[i] = 0;
}
// 给定a, 求a的平方根b, 且a[0]=1
void getroot(int* a, int* b, int n) {
    if (n == 1) {
        b[0] = 1;
        return;
    }
    getroot(a, b, n >> 1);
    getinv(b, tmp2, n);
    int k = n << 1, i;
    for (i = 0; i < n; i++) tmp[i] = a[i];
    for (i = n; i < k; i++) tmp[i] = b[i] = 0;
    NTT(tmp, k, 1), NTT(b, k, 1), NTT(tmp2, k, 1);
    for (i = 0; i < k; i++)
        b[i] = ((ll)b[i] * b[i] + tmp[i]) % P * inv2 % P * tmp2[i] % P;
    NTT(b, k, -1);
    for (i = n; i < k; i++) b[i] = 0;
}
// 给定a, 求a的pw次幂, 且a[0] = 1
void getpow(int* a, int n, int pw) {
    getln(a, b, n);
    for (i = 0; i < n; i++) b[i] = (ll)b[i] * pw % P;
    getexp(b, a, n);
}
void init() {
    g[K] = power(G, (P - 1) / N), ng[K] = power(g[K], P - 2);
    for (i = K - 1; ~i; i--)
        g[i] = (ll)g[i + 1] * g[i + 1] % P,
        ng[i] = (ll)ng[i + 1] * ng[i + 1] % P;
    for (inv[1] = 1, i = 2; i <= N; i++)
        inv[i] = (ll)(P - inv[P % i]) * (P / i) % P;
    inv2 = inv[2];
}
/*
    Example1:
        给定一个包含n个数字的非重数集C和正整数m，
        现在要求构造一棵点权二叉树，要求每个节点的权值在集合C中，
        定义二叉树的权值为所有点权之和，现在希望对于s∈[1,m]，
        求出本质不同的权值为s的二叉树数量
        两棵二叉树相同当且仅当树结构相同且对应位置点权相同，
        左右子树交换当做不相同处理
    Solution:
        我们记包含n的数字的非重集合C的生成函数为G(x)
        G(x)=1+x^{C_1}+x^{C_2}+……
        我们记满足条件的二叉树的生成函数为F(x)
        x^i前的系数为权值为i的二叉树的数量
        则有方程F(x)=G(x)*F(x)^2+1
        F(x)^2表示左右子树情况，G(x)表示子树根节点选值情况，+1表示子树为空
        移项得到G(x)*F(x)^2-F(x)+1=0，方程有解为(1±\sqrt(1-4G(x))/(2G(x))
        上下同乘(1∓\sqrt(1-4G(x))得到2/(1∓\sqrt(1-4G(x))
        因为分母常数项不能为0，所以F(x)=2/(1+\sqrt(1-4G(x))
        多项式开方并求逆即可
*/
int main() {
    init();
    scanf("%d%d", &n, &m);
    for (i = 1; i <= n; i++) {
        scanf("%d", &k);
        if (k <= m) a[k]++;
    }
    int len;
    for (len = 1; len <= m; len <<= 1);
    for (i = 1; i < len; i++) if (a[i]) a[i] = P - 4;
    a[0]++;
    getroot(a, b, len);
    b[0]++;
    getinv(b, a, len);
	for (int i = 1; i <= m; i++)
        printf("%d\n", (a[i] + a[i]) % P);
    return 0;
}
/*
    Example2:
        给定一个含有m个正整数的集合D和一个正整数s，
        一棵树是合法的当且仅当每个非叶子节点的孩子数量均在集合D内
        问有多少棵不同的含有s个叶节点的树，孩子有序
    Solution:
        我们记包含n的数字的非重集合C的生成函数为G(x)
        G(x)=1+x^{C_1}+x^{C_2}+……
        我们记满足条件的树的生成函数为F(x)
        则F(x)=G(F(x))+x
        表示子树的生成函数的合法幂次之和以及没有子树的情况
        F(x)-G(F(x))=x，F(x)是x-G(x)构成复合逆(反函数)
        根据拉格朗日反演[x^n]G(x)=(1/n)[x^(n-1)](x/F(x))^n
        我们可以得到[x^n]F(x)=(1/n)[x^(n-1)](x/(x-G(x)))^n
        即[x^n]F(x)=(1/n)[x^(n-1)](1-G(x)/x)^{-n}
        计算后面这个多项式的n-1项即可
*/
int s, m;
int main() {
    init();
    scanf("%d%d", &s, &m);
    a[0] = 1;
    for (int i = 1; i <= m; i++) {
        scanf("%d", &k);
        a[k - 1] = (a[k - 1] + P - 1) % P;
    }
    for (k = 1; k <= s; k <<= 1);
    getpow(a, k, P - s);
    printf("%d", 1ll * a[s - 1] * power(s, P - 2) % P);
    return 0;
}
