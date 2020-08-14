/*
    分数规划
    Example
        有一个数列，要求选出M个区间，每个区间长度在[L,R]内，
        使得选出的那些数字的平均值尽可能大
*/
#include <deque>
typedef long long LL;
const LL INF = 1000000000000000007ll;
typedef pair<int, LL> PIL;
int n, m, l, r, a[100005];
LL dp[15][100005], sum[100005], pfx[100005];
PIL pt[15][100005];
bool check(LL& S, LL& N) {
    for (int i = 0; i < n; i++) sum[i + 1] = sum[i] + a[i] * N - S;
    for (int x = 1; x <= m; x++) {
        dp[x][0] = -INF;
        deque<PIL> u;
        for (int i = 1; i <= n; i++) {
            while (u.size() && i - u.front().first > r) u.pop_front();
            int k = i - l;
            if (k >= 0 && dp[x - 1][k] != -INF) {
                LL tmp = dp[x - 1][k] - sum[k];
                while (u.size() && tmp > u.back().second) u.pop_back();
                u.push_back(PIL(k, tmp));
            }
            dp[x][i] = dp[x][i - 1], pt[x][i] = pt[x][i - 1];
            if (u.size()) {
                int pos = u.front().first;
                LL tmp = u.front().second + sum[i];
                if (tmp > dp[x][i]) {
                    dp[x][i] = tmp;
                    pt[x][i] = pt[x - 1][pos];
                    pt[x][i].first += i - pos;
                    pt[x][i].second += pfx[i] - pfx[pos];
                }
            }
        }
    }
    if (!dp[m][n]) return 1;
    return N = pt[m][n].first, S = pt[m][n].second, 0;
}
LL GetAns(LL S, LL N) {
    if (S >= 0) return S / N;
    return -((-S + N - 1) / N);
}
int main() {
    while (~scanf("%d%d%d%d", &n, &m, &l, &r)) {
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
            pfx[i + 1] = pfx[i] + a[i];
        }
        LL S = 0, N = 1;
        while (N && !check(S, N))
            ;
        if (!N) {
            puts("-1");
        } else {
            LL ans = GetAns(S * 100, N);
            printf("%lld.%02lld\n", ans / 100, abs(ans) % 100);
        }
    }
    return 0;
}