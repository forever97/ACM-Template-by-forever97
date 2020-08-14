/*
    期望-组合项精度保护
    Problem：
        有两个盒子，每个盒子里有n颗糖，每次打开第一个盒子概率为p，第二个盒子概率为1-p,
        每次打开盒子之后就拿走一颗糖，问当某次打开盒子没有糖时，另一个盒子拥有的糖数量的期望
    Solution：
        假设最后打开第1个盒子为空，第2个盒子里还有i颗糖，其概率为C(2n-1,n)*p^(n+1)*(1-p)^(n-i)
        同理最后打开第2个盒子为空，第1个盒子里还有i颗糖的概率为C(2n-1,n)(1-p)^(n+1)p^(n-i)
        所以总期望就为∑i*(C(2n-1,n)*p^(n+1)*(1-p)^(n-i)+C(2n-1,n)(1-p)^(n+1)p^(n-i))
        考虑到n接近20万时组合数极大而p的(n+1)次幂极小容易导致精度缺失，故两者均取ln后相加，
        最后求exp即可
*/
#include <bits/stdc++.h>
using namespace std;
typedef long double LD;
const int N = 400010;
LD logF[N];
void Init() {
    for (int i = 1; i < N; i++) logF[i] = logF[i - 1] + log(i);
}
LD logC(int n, int m) { return logF[n] - logF[m] - logF[n - m]; }
int n, cas = 0;
double p;
int main() {
    Init();
    while (~scanf("%d%lf", &n, &p)) {
        double ans = 0.0;
        for (int i = 1; i <= n; i++) {
            LD c = logC(2 * n - i, n);
            LD v1 = c + (n + 1) * log(p) + (n - i) * log(1 - p);
            LD v2 = c + (n + 1) * log(1 - p) + (n - i) * log(p);
            ans += i * (exp(v1) + exp(v2));
        }
        printf("Case %d: %.6f\n", ++cas, ans);
    }
    return 0;
}