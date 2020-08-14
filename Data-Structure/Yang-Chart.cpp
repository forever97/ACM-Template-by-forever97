/*
    杨氏图表
    求k条不共享元素的单调不降子序列，使得子序列元素和最大
    对于每个前缀数组输出答案
    
    我们对于每个元素x拆分为x个x，则题目转化为
    求k条不共享元素的单调不降子序列，使得子序列长度和最大，
    等价于杨氏图表前k层长度之和
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int K = 5;
int Case, n, i, x;
ll ans;
map<int, ll> T[K];
void ins(int o, int x, ll p) {
    if (o >= K) return;
    T[o][x] += p;
    ans += p;
    while (p) {
        map<int, ll>::iterator it = T[o].lower_bound(x + 1);
        if (it == T[o].end()) return;
        ll t = min(p, it->second);
        ans -= t;
        p -= t;
        ins(o + 1, it->first, t);
        if (t == it->second)
            T[o].erase(it);
        else
            it->second -= t;
    }
}
int main() {
    scanf("%d", &Case);
    while (Case--) {
        scanf("%d", &n);
        ans = 0;
        for (i = 0; i < K; i++) T[i].clear();
        for (i = 1; i <= n; i++) {
            scanf("%d", &x);
            ins(0, x, x);
            printf("%lld%c", ans, " \n"[i==n]);
        }
    }
}