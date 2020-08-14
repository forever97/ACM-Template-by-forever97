/*
    数位DP
    Problem:
        一个数被称之为平衡数当且仅当其为奇数的数位每个数字出现次数为偶数，
        偶数的数位每个数字出现次数为奇数，询问[L,R]之间有多少平衡数
    Solution:
        用三进制表示每个数位的状态，0为该数字没有出现过，1为出现奇数次，2为出现偶数次，
        用dp[i][j]保存剩下未填充的位为i位，当前得到状态为j且接下来没有数位限制时的方案数，
        记忆化数位dp即可
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long unsigned LL;
LL dp[20][60000], L, R;
int n, bit[20], pw[20], b[60000];
bool Check(int s) {
    for (int i = 0; i <= 9; i++, s /= 3) {
        if (i & 1 && s % 3 == 1)
            return 0;
        else if (!(i & 1) && s % 3 == 2)
            return 0;
    }
    return 1;
}
int Cal(int i, int s) {
    int p = s / pw[i] % 3;
    if (p < 2)
        s += pw[i];
    else
        s -= pw[i];
    return s;
}
LL DP(int i, int j, int lim, int z) {  // lim: 数位大小限制  |  z: 是否有前导0
    if (!i) return b[j];
    if (!lim && ~dp[i][j]) return dp[i][j];
    LL ans = 0;
    int ed = lim ? bit[i] : 9;
    for (int k = 0; k <= ed; k++)
        ans += DP(i - 1, (z && !k) ? 0 : Cal(k, j), lim && k == ed, z && !k);
    if (!lim) dp[i][j] = ans;
    return ans;
}
LL Solve(LL s, int len = 0) {
    for (; s; s /= 10) bit[++len] = s % 10;
    return DP(len, 0, 1, 1);
}
int main() {
    memset(dp, -1, sizeof(dp));
    pw[0] = 1;
    for (int i = 1; i <= 10; i++) pw[i] = pw[i - 1] * 3;
    for (int i = 0; i < pw[10]; i++) b[i] = Check(i);
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%llu%llu", &L, &R);
        printf("%llu\n", Solve(R) - Solve(L - 1));
    }
    return 0;
}
