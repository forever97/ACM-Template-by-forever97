/*
    Example：
        n个牧场排成一行，需要在某些牧场上面建立控制站，
        每个牧场上只能建立一个控制站，每个控制站控制的牧场
        是它所在的牧场一直到它西边第一个控制站的所有牧场
        它西边第一个控制站所在的牧场不被控制，如果它西边不存在控制站，
        那么它控制西边所有的牧场，每个牧场被控制都需要一定的花费，
        而且该花费等于它到控制它的控制站之间的牧场数目乘上该牧场的放养量，
        在第i个牧场建立控制站的花费是ai，每个牧场i的放养量是bi，
        求最小总花费。
    Solution：
        考虑只在n建立控制站的情况，答案为∑b[i]*(n-i)
        记dp[i]为考虑了i到n的情况，并且在i点建立了控制站的最优情况，
        有dp[i]=max{dp[j]+sum[i]*(j-i)}-a[i]
        =-a[i]-sum[i]*i+max(dp[j]+sum[i]*j)
        为关于sum[i]的线性函数，那么对f(y)=x*y+dp[x]进行斜率优化
*/
using namespace std;
typedef long long LL;
const int N = 1000010;
int deq[N], n;
LL tot, ans, dp[N], S[N], a[N], b[N];
LL f(int x, LL y) { return y * x + dp[x]; }
bool check(int f1, int f2, int f3) {
    LL a1 = f1, b1 = dp[f1];
    LL a2 = f2, b2 = dp[f2];
    LL a3 = f3, b3 = dp[f3];
    return (a2 - a1) * (b3 - b2) <= (b2 - b1) * (a3 - a2);
}
void solve() {
    for (int i = 1; i <= n; i++) S[i] = S[i - 1] + b[i];
    int s = 0, t = 1;
    deq[0] = n;
    for (int i = n - 1; i; i--) {
        while (s + 1 < t && f(deq[s], S[i]) <= f(deq[s + 1], S[i])) s++;
        dp[i] = -a[i] - S[i] * i + f(deq[s], S[i]);
        ans = max(ans, dp[i]);
        while (s + 1 < t && check(deq[t - 2], deq[t - 1], i)) t--;
        deq[t++] = i;
    }
    printf("%lld\n", tot - ans);
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%lld", &a[i]);
    for (int i = 1; i <= n; i++) scanf("%lld", &b[i]);
    for (int i = 1; i < n; i++) tot += b[i] * (n - i);
    tot += a[n];
    solve();
    return 0;
}
/*
    Example：
        N个任务排成一个序列在一台机器上等待完成（顺序不得改变），
        这N个任务被分成若干批，每批包含相邻的若干任务。
        从时刻0开始，这些任务被分批加工，第i个任务单独完成所需的时间是Ti。
        在每批任务开始前，机器需要启动时间S，
        而完成这批任务所需的时间是各个任务需要时间的总和（同一批任务将在同一时刻完成）。
        每个任务的费用是它的完成时刻乘以一个费用系数Fi。
        请确定一个分组方案，使得总费用最小
    Solution：
        我们可以得到dp方程dp[i]=min{dp[j]+(sumT[i]-sumT[j]+S)*(sumF[n]-sumF[j])}
        dp[i]=(sumF[n]-sumF[j])*sumT[i]+dp[j]+(S-sumT[j])*(sumF[n]-sumF[j])
        是关于sumT[i]的一元一次方程，我们对此做斜率优化。
*/
typedef long long LL;
const int MAX_N = 10010;
int n;
LL k, a[MAX_N], b[MAX_N], dp[MAX_N], S[MAX_N], F[MAX_N], deq[MAX_N];
LL f(int x, int y) {
    return (F[n] - F[x]) * S[y] + dp[x] + (k - S[x]) * (F[n] - F[x]);
}
bool check(int f1, int f2, int f3) {
    LL a1 = F[n] - F[f1], b1 = dp[f1] + (k - S[f1]) * (F[n] - F[f1]);
    LL a2 = F[n] - F[f2], b2 = dp[f2] + (k - S[f2]) * (F[n] - F[f2]);
    LL a3 = F[n] - F[f3], b3 = dp[f3] + (k - S[f3]) * (F[n] - F[f3]);
    return (a2 - a1) * (b3 - b2) >= (b2 - b1) * (a3 - a2);
}
void solve() {
    for (int i = 0; i < n; i++) S[i + 1] = S[i] + a[i], F[i + 1] = F[i] + b[i];
    int s = 0, t = 1;
    deq[0] = 0;
    dp[0] = 0;
    for (int i = 1; i <= n; i++) {
        while (s + 1 < t && f(deq[s], i) >= f(deq[s + 1], i)) s++;
        dp[i] = f(deq[s], i);
        while (s + 1 < t && check(deq[t - 2], deq[t - 1], i)) t--;
        deq[t++] = i;
    }
    printf("%lld\n", dp[n]);
}
int main() {
    while (~scanf("%d%lld", &n, &k)) {
        for (int i = 0; i < n; i++) scanf("%lld%lld", &a[i], &b[i]);
        solve();
    }
    return 0;
}
/*
    Example：
        给出一个长度为n个非严格单调递增数列，每次操作可以使得其中任意一项减一，
        问现在使得数列中每项数相同的数的数量都大于等于k-1，问最少进行几次操作
    Solution：
        我们设dp[i]为前i项答案，得到方程dp[i]=min(dp[j]+S[i]-S[j]-aj*(i-j)),
        dp[i]=S[i]+min(dp[j]-S[j]+aj*j-aj*i)，为关于i的线性函数，
        所以我们对f(y)=-ax*y+dp[x]-S[x]+ax*x进行斜率优化。
*/
typedef long long LL;
const int MAX_N = 500010;
int n, k;
LL a[MAX_N], dp[MAX_N], S[MAX_N], deq[MAX_N];
LL f(int x, int y) { return -a[x] * y + dp[x] - S[x] + a[x] * x; }
bool check(int f1, int f2, int f3) {
    LL a1 = -a[f1], b1 = dp[f1] - S[f1] + a[f1] * f1;
    LL a2 = -a[f2], b2 = dp[f2] - S[f2] + a[f2] * f2;
    LL a3 = -a[f3], b3 = dp[f3] - S[f3] + a[f3] * f3;
    return (a2 - a1) * (b3 - b2) >= (b2 - b1) * (a3 - a2);
}
void solve() {
    for (int i = 0; i < n; i++) S[i + 1] = S[i] + a[i];
    int s = 0, t = 1;
    deq[0] = 0;
    dp[0] = 0;
    for (int i = k; i <= n; i++) {
        if (i - k >= k) {
            while (s + 1 < t && check(deq[t - 2], deq[t - 1], i - k)) t--;
            deq[t++] = i - k;
        }
        while (s + 1 < t && f(deq[s], i) >= f(deq[s + 1], i)) s++;
        dp[i] = S[i] + f(deq[s], i);
    }
    printf("%lld\n", dp[n]);
}
int T;
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &k);
        for (int i = 0; i < n; i++) scanf("%lld", &a[i]);
        solve();
    }
    return 0;
}
