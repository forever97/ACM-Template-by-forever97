/*
    Problem:
        游戏开始有四座塔，每座均由正方体叠成，所有正方体是黑色或者白色
        玩家L和R轮流操作，每次选定一个正方体，将正方体及其以上正方体全部拿走
        L玩家只能选择白色正方体，R玩家只能选择黑色正方体，不能操作者输
        如果L玩家无论先手或者后手都能赢，则称局面为W-configuration，
        定义子局面为三塔局面即为C，对于完整局面(C,T),
        如果对于任意塔T，(C2,T)为W-configuration时，(C1,T)均为W-configuration
        则称C1不劣于C2，给定C1和C2，判断C1是否不劣于C2
    Solution:
        考虑一座塔的SN值，当塔为空时SN={|}=0
        如果塔包含一个白色正方体，则玩家L拥有可转移到0的决策，SN={0|}=1
        如果塔包含n个白色正方体，则SN={0,1,…,n-1|}=n
        同理塔包含n个黑色正方体时SN=-n
        当塔包含n个白色正方体和顶端一个黑色正方体时，SN={0,1,…,n-1|n}={n-1|n}=n-1/2
        如果包含n个白色正方体和顶端两个黑色正方体时，SN={n-1|n-1/2}=n-1/2-1/4
        在以上情况下在顶端再堆叠一个白色正方体，SN={n-1/2-1/4|n-1/2}=n-1/2-1/4+1/8
        结论就比较显然了，除去最底端的连续块，黑色方块-1/2^i，白色方块+1/2^i
*/
#include <cstdio>
#include <cstring>
using namespace std;
double getSN(int T[], int n) {
    double SN = 0;
    int i = 0;
    for (; i < n && T[i] == T[0]; i++) SN += T[i];
    for (double k = 2; i < n; i++, k = k * 2) SN += T[i] / k;
    return SN;
}
const int N = 60;
char s[N];
int T[N], d[3], Cas;
int main() {
    scanf("%d", &Cas);
    for (int cas = 1; cas <= Cas; cas++) {
        scanf("%s%s", s, s);
        for (int i = 0; i < 3; i++) scanf("%d", &d[i]);
        double SN1 = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < d[i]; j++) {
                scanf("%s", s);
                T[j] = 2 * (s[0] == 'W') - 1;
            }
            SN1 += getSN(T, d[i]);
        }
        for (int i = 0; i < 3; i++) scanf("%d", &d[i]);
        double SN2 = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < d[i]; j++) {
                scanf("%s", s);
                T[j] = 2 * (s[0] == 'W') - 1;
            }
            SN2 += getSN(T, d[i]);
        }
        if (SN1 >= SN2)
            printf("Test %d: Yes\n", cas);
        else
            printf("Test %d: No\n", cas);
    }
    return 0;
}
/*
    Problem:
        给定一个多个3*3的棋盘格，包含#,.,O和X
        Alice和Bob轮流对这些棋盘格做操作，当一方不能操作了算输
        Alice每次可以选择一个O，将其变成#
        同时，对以下三种事件挑选一种发生：[不能不选]
            1.选择的格子上下格子均变成#
            2.选择的格子左右格子均变成#
            3.上述两种事件同时发生
        Bob每次可以选择一个X，将其变成#，没有任何伴随事件
        如果Alice或者Bob始终能赢则对应输出Alice和Bob
        否则如果先手或者后手始终能赢则对应输出First和Second
        否则直接输出Others
    Solution:
        SN={Alice can get|Bob can get}
        非平等博弈只存在三种情况：
            1.SN>0：Alice总能够取胜
            2.SN<0：Bob总能取胜
            3.SN==0：后手总能取胜
        我们对$3*3$棋盘进行三进制状压
        暴力预处理出每种状态下的SN，Multi-SN情况将SN相加判断即可
*/
#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)
#define red(i, n) for (int i = n - 1; ~i; i--)
const int N = 27 * 27 * 27;
using ll = long long;
struct S {
    int x, y;
    S() {}
    S(int x, int y) : x(x), y(y) {}
    bool operator==(const S &b) const { return x == b.x && y == b.y; }
    bool operator<(const S &b) const { return x * b.y < y * b.x; }
    bool operator<=(const S &b) const { return *this < b || *this == b; }
};
S INF(0, -1), SN[N];
int st[9], nst[9];
auto msk = [&]() {
    int t = 0;
    red(i, 9) t = t * 3 + nst[i];
    return t;
};
auto getmid = [&](S &a, S &b) {
    ll down = max(a.y, b.y);
    ll up = a.x * (down / a.y) + b.x * (down / b.y);
    down <<= 1;
    while (up % 2 == 0) up /= 2, down /= 2;
    return S(up, down);
};
int Decode(int x) { rep(i, 9) st[i] = x % 3, x /= 3; }
void Init() {
    rep(i, N) {
        S L = INF, R = INF;
        Decode(i);
        auto updL = [&]() {
            S s = SN[msk()];
            if (L == INF || L < s) L = s;
        };
        auto updR = [&]() {
            S s = SN[msk()];
            if (R == INF || s < R) R = s;
        };
        rep(j, 9) if (st[j] == 1) {
            int x = j / 3, y = j % 3;
            rep(k, 9) nst[k] = st[k];
            nst[j] = 0;
            if (x) nst[j - 3] = 0;
            if (x != 2) nst[j + 3] = 0;
            updL();
            if (y) nst[j - 1] = 0;
            if (y != 2) nst[j + 1] = 0;
            updL();
            if (x) nst[j - 3] = st[j - 3];
            if (x != 2) nst[j + 3] = st[j + 3];
            updL();
        }
        rep(j, 9) if (st[j] == 2) {
            int x = j / 3, y = j % 3;
            rep(k, 9) nst[k] = st[k];
            nst[j] = 0;
            updR();
        }
        if (L == INF && R == INF)
            SN[i] = S(0, 1);
        else if (L == INF)
            SN[i] = S(R.x - 1, 1);
        else if (R == INF)
            SN[i] = S(L.x + 1, 1);
        else {
            assert(L < R);
            S l = INF, r = INF, x(0, 1);
            while (R <= x || x <= L) {
                if (R <= x) {
                    r = x;
                    if (l == INF)
                        x.x--;
                    else
                        x = getmid(l, r);
                } else {
                    l = x;
                    if (r == INF)
                        x.x++;
                    else
                        x = getmid(l, r);
                }
            }
            SN[i] = x;
        }
    }
}
int Tr(char c) {
    if (c == 'O') return 1;
    if (c == 'X') return 2;
    return 0;
}
int T, n;
char s[10];
int main() {
    Init();
    scanf("%d", &T);
    while (T--) {
        int sn = 0;
        scanf("%d", &n);
        while (n--) {
            string b;
            rep(i, 3) {
                scanf("%s", s);
                rep(j, 3) b += s[j << 1];
            }
            int msk = 0;
            red(i, 9) msk = msk * 3 + Tr(b[i]);
            sn += SN[msk].x * (64 / SN[msk].y);
        }
        if (sn == 0)
            puts("Second");
        else if (sn > 0)
            puts("Alice");
        else
            puts("Bob");
    }
    return 0;
}