/*
    高维前缀和
    Example：
        给出n个数字c，求非负整数序列a，满足a<2^m
        并且有a[i]&a[i+1]=0，对于每个a[i]，要保证a[i]不是c[i]的倍数，
        求这样的a[i]序列的个数
    Solution：
        我们用dp[i]表示以i为结尾的方案数，
        我们发现要满足a[i]&a[i+1]=0,
        则dp[i]是从上一次结果中所有满足i&j=0的地方转移过来的
        i&j=0即i&(~j)=i，即i为~j的子集，那么我们每次对上一次的结果进行下标取反操作，
        那么求当前dp[i]，就是求出以i为子集的上一次计算出的dp值的高维前缀和。
        对于c[i]这个条件，我们每轮计算后将c[i]倍数为下标的dp值置0即可。
    
*/
int T, n, m, c[100];
const int mod = 1000000000;
struct data {
    int val;
    data operator+(const data &rhs) const {
        int t_val = val + rhs.val;
        if (t_val >= mod) t_val -= mod;
        if (t_val < 0) t_val += mod;
        return data{t_val};
    }
} dp[(1 << 15) + 10], res;
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++) scanf("%d", &c[i]);
        int all = 1 << m;
        for (int j = 0; j < all; j++) dp[j].val = (j == 0);
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < all; j += 2) swap(dp[j], dp[j ^ (all - 1)]);
            for (int j = 0; j < m; j++)
                for (int k = 0; k < all; k++) {
                    if (~k & (1 << j)) dp[k] = dp[k] + dp[k | (1 << j)];
                }
            for (int j = 0; j < all; j += c[i]) dp[j].val = 0;
        }
        res.val = 0;
        for (int j = 0; j < all; j++) res = res + dp[j];
        printf("%d\n", res.val);
    }
    return 0;
}
/*
    高维前缀最大次大
    Example：
        给定N个数A1, A2, A3, ... AN,
        从中找到两个数Ai和Aj(i≠j)使得乘积Ai*Aj*(Ai&Aj)最大
    Solution：
        我们可以枚举x&y的结果z，找出两个数x&y==z使得x*y最大，更新答案即可，
        条件可以被削弱为z为x&y的子集，这种条件放缩不会导致最优解的丢失，
        z为x&y的子集等价于z为x的子集并且z为y的子集。
        那么我们只要找出以z为子集的最大值和次大值，然后枚举z即可计算出答案。
        复杂度O(k*2^k).
*/
struct data {
    int val[2];
    data operator+(const data &rhs) const {
        int t_val[2] = {val[0], val[1]};
        for (int i = 0; i < 2; i++) {
            if (rhs.val[i] > t_val[0]) {
                t_val[1] = t_val[0];
                t_val[0] = rhs.val[i];
            } else if (rhs.val[i] > t_val[1])
                t_val[1] = rhs.val[i];
        }
        return data{t_val[0], t_val[1]};
    }
} dp[(1 << 20) + 10];
int T, n;
int main() {
    scanf("%d", &T);
    int all = 1 << 20;
    while (T--) {
        for (int i = 0; i < all; i++) dp[i] = data{0, 0};
        scanf("%d", &n);
        for (int i = 1, x; i <= n; i++) {
            scanf("%d", &x);
            dp[x] = dp[x] + data{x, 0};
        }
        for (int i = 0; i < 20; i++)
            for (int j = 0; j < all; j++)
                if (~j & (1 << i)) dp[j] = dp[j] + dp[j | (1 << i)];
        long long ans = 0;
        for (int i = 0; i < all; i++)
            ans = max(ans, 1LL * i * dp[i].val[0] * dp[i].val[1]);
        printf("%lld\n", ans);
    }
    return 0;
}
/*
    高维前缀差分
    Example:
        给出一些数字，问其选出一些数字作and为0的方案数有多少
    Solution：
        题目等价于给出一些集合，问其交集为空集的方案数，
        我们先求交集为S的方案数，记为dp[S]，发现处理起来还是比较麻烦，
        我们放缩一下条件，求出交集包含S的方案数，记为dp[S]，
        我们发现dp[S]，是以其为子集的方案的高维前缀和，
        我们逆序求高维前缀和即可，之后考虑容斥，求出交集为0的情况，
        我们发现这个容斥实质上等价于高维的前缀差分，
        那么我们利用之前的代码，修改一下参数就能得到答案。
    

*/
const int mod = 1000000007;
typedef long long LL;
int all, n, m, x;
struct data {
    int val;
    data operator+(const data &rhs) const {
        int t_val = val + rhs.val;
        if (t_val >= mod) t_val -= mod;
        if (t_val < 0) t_val += mod;
        return data{t_val};
    }
    data operator*(const int &rhs) const {
        int t_val = val * rhs;
        return data{t_val};
    }
} dp[(1 << 20) + 10];
LL pow(LL a, LL b, LL p) {
    LL t = 1;
    for (a %= p; b; b >>= 1LL, a = a * a % p)
        if (b & 1LL) t = t * a % p;
    return t;
}
void doit(data dp[], int n, int f) {
    for (int i = 0; i < n; i++) {
        for (int j = all - 1; j >= 0; j--) {
            if (~j & (1 << i)) dp[j] = dp[j] + dp[j | (1 << i)] * f;
        }
    }
}
int main() {
    scanf("%d", &n);
    all = (1 << 20);
    for (int i = 0; i < n; i++) {
        scanf("%d", &x);
        dp[x].val++;
    }
    doit(dp, 20, 1);
    for (int i = 0; i < all; i++) dp[i].val = pow(2, dp[i].val, mod);
    doit(dp, 20, -1);
    printf("%d\n", dp[0].val);
    return 0;
}