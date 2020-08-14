/*
    连续随机均匀分布联合概率
    S=x1+x2+x3+……
    已知x[i]的取值范围xl[i]~xr[i]，求S取值范围为[l,r]的概率
    一般做法，将x的取值范围变为左端点为0，xr[i]=xr[i]-xl[i]，同时l,r也减去xl[i]，
    转化为x取值范围为[0.X]来计算
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 20;
typedef long double LD;
namespace Continuous_Random_Uniform_Distribution {
int n;
double x[N];
LD cal(double x) {
    if (x < 0) return 0;
    LD res = 1;
    for (int i = 1; i <= n; i++) res = res * x / i;
    return res;
}
LD solve(double d) {
    LD res = 0;
    for (int i = 0; i < 1 << n; i++) {
        int cnt = 1;
        double cur = d;
        for (int j = 0; j < n; j++) {
            if ((1 << j) & i) {
                cnt *= -1;
                cur -= x[j];
            }
        }
        res += cnt * cal(cur);
    }
    return res;
}
LD Cal(double l, double r) {
    LD base = 1;
    for (int i = 0; i < n; i++) base *= x[i];
    return fabs(solve(r) - solve(l)) / base;
}
}  // namespace Continuous_Random_Uniform_Distribution
/*
    Example：Xi的取值范围为[-xi,xi]，S=X1+X2+X3+……，求S取值在[l,r]范围的概率
*/
using namespace Continuous_Random_Uniform_Distribution;
int main() {
    int T;
    double l, r;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%lf%lf", &n, &l, &r);
        for (int i = 0; i < n; i++) {
            scanf("%lf", &x[i]);
            l += x[i];
            r += x[i];
            x[i] *= 2;
        }
        printf("%.9f\n", (double)Cal(l, r));
    }
    return 0;
}
