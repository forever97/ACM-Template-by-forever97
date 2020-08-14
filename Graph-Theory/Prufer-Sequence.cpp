/*
    任意一棵n节点的树都可唯一的用长度为n-2的prufer编码表示，
    度数为m的节点的序号在prufer编码中出现的次数为m-1
    因此无限制生成树的种类有n^(n-2)种，
    Prufer数列构造方法：
        选取编号最小的叶子节点删掉
        并将它的父亲加入到prufer数列中
        直到树上还有两个节点。
    Prufer数列转化成树方法：
        设{a1,a2,..an-2}为一棵有n个节点的树的Prufer序列
        另建一个集合G含有元素{1..n}，
        找出集合中最小的未在Prufer序列中出现过的数，
        将该点与Prufer序列中首项连一条边，
        并将该点和Prufer序列首项删除，重复操作n-2次，
        将集合中剩余的两个点之间连边即可。
    示例：给出树上n个点的度数，求满足条件的树的数目
*/
#include <algorithm>
#include <cstdio>
using namespace std;
const int M = 200;
int p[200], pNum;
bool f[M];
void Prime() {
    int i, j;
    for (i = 2; i < M; i++) {
        if (!f[i]) {
            p[pNum++] = i;
        }
        for (j = 0; j < pNum && p[j] * i < M; j++) {
            f[p[j] * i] = 1;
            if (!(i % p[j])) break;
        }
    }
}
int n, d[200], u[200];
void Calc(int x, int r) {
    if (!x) return;
    for (int i = 0; i < pNum; i++)
        for (int j = p[i]; x / j != 0; j *= p[i]) u[i] += x / j * r;
}
void Solve() {
    scanf("%d", &n);
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &d[i]), sum += d[i];
        if (d[i] <= 0 && n > 1) {
            puts("0");
            return;
        }
    }
    if (sum != (n - 1) * 2) {
        puts("0");
        return;
    }
    if (n == 1) {
        puts("1");
        return;
    }
    Calc(n - 2, 1);
    for (int i = 1; i <= n; i++) Calc(d[i] - 1, -1);
    long long ans = 1;
    for (int i = 0; i < pNum; i++)
        for (int j = 1; j <= u[i]; j++) ans *= p[i];
    printf("%lld\n", ans);
}
int main() {
    Prime();
    Solve();
    return 0;
}