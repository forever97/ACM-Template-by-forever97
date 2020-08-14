/*
    启发式分解
    题目大意：
        一个序列被称为是不无聊的，仅当它的每个连续子序列存在一个独一无二的数字，
        即每个子序列里至少存在一个数字只出现一次。
        给定一个整数序列，请你判断它是不是不无聊的。
    题解：
        预处理每个元素上一次出现位置和下一个出现位置，
        我们发现对于一个子序列[L,R]来说，
        如果存在pre[i]<L&&nxt[i]>R那么这个子序列一定是满足条件的，
        否则就不满足，那么我们分治处理这个问题，
        从两边往中间寻找这个i，那么每次拆开的复杂度就是拆成的两个序列中较小的一个，
        所以这是一个逆启发式合并的过程，复杂度O(nlogn)
*/
#include <algorithm>
#include <cstdio>
#include <map>
using namespace std;
const int N = 200010;
int n, T, a[N], pre[N], nxt[N];
bool check(int L, int R) {
    if (L >= R) return 1;
    int l = L, r = R;
    for (int i = L; i <= R; i++) {
        if (i & 1) {
            if (pre[l] < L && nxt[l] > R)
                return (check(L, l - 1) && check(l + 1, R));
            l++;
        } else {
            if (pre[r] < L && nxt[r] > R)
                return (check(L, r - 1) && check(r + 1, R));
            r--;
        }
    }
    return 0;
}
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        map<int, int> M;
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            nxt[M[a[i]]] = i;
            pre[i] = M[a[i]];
            M[a[i]] = i;
        }
        for (int i = 1; i <= n; i++) nxt[M[a[i]]] = n + 1;
        if (check(1, n))
            puts("non-boring");
        else
            puts("boring");
    }
    return 0;
}
