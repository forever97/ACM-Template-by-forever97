/*
    构造
*/
/*
    题目大意：
        构造出2^n个由1和-1组成的串使得其两两点积为0
    题解：
        我们可以构造这样一个矩阵，右下角四分之一和其余三个分块相反，
        不断扩展这个矩阵即可。
*/
#include <cstdio>
#define rep(i, n) for (int i = 1; i <= n; i++)
using namespace std;
const int N = 1000;
int k, p[N][N];
int main() {
    for (int n = 1; n <= 9; n++) {
        int m = 1 << (n - 1);
        rep(i, m) rep(j, m) p[i + m][j] = p[i][j + m] = p[i][j],
                                  p[i + m][j + m] = !p[i][j];
    }
    while (~scanf("%d", &k)) {
        rep(i, 1 << k) {
            rep(j, 1 << k) putchar(p[i][j] ? '+' : '*');
            puts("");
        }
    }
    return 0;
}
/*
    题目大意：
        给出一个1到n的排列，问每两个位置都进行一次交换最终排列不变是否可能，
        如果可能输出交换顺序。
    题解：
        我们发现对于四个一组组内进行六次交换之后可以保证四个数的位置不变，
        而对于每组相互之间可以一共进行十六次交换使得两组均不会发生变化
        所以如果n能被4整除，那么我们可以4个分组达到目的，
        当不能被4整除的时候，我们发现余数为2和3的时候都不能构造出可行解。
        在余数为1的时候有一种特殊的基于4分组的构造法，
        我们在相邻两个数字交换的时候，我们将多出来的那个数字作为第三参数，
        这样多出来那个数字就能够和剩余所有位置交换过一次而不改变位置。
*/
#include <algorithm>
#include <cstdio>
#include <utility>
#include <vector>
using namespace std;
#define rep(i, n) for (int i = 0; i < n; i++)
typedef pair<int, int> P;
typedef vector<P> V;
void add(V &ans, int pos1, int pos2) {
    pos1 <<= 2;
    pos2 <<= 2;
    rep(k, 4) rep(i, 4) ans.push_back(P(pos1 + i, pos2 + (i ^ k)));
}
int dx[] = {0, 0, 1, 0, 1, 2}, dy[] = {1, 2, 3, 3, 2, 3};
void add4(V &ans, int pos) {
    pos <<= 2;
    rep(i, 6) ans.push_back(P(pos + dx[i], pos + dy[i]));
}
V make4(int n) {
    V ans;
    rep(i, n / 4) add4(ans, i);
    rep(i, n / 4) rep(j, i) add(ans, j, i);
    return ans;
}
int n;
void solve() {
    V ans;
    if (n % 4 == 0)
        ans = make4(n);
    else if (n % 4 == 1) {
        V tmp = make4(n - 1);
        for (int i = 0; i < tmp.size(); i++) {
            P p = tmp[i];
            int x = p.first, y = p.second;
            if (x > y) swap(x, y);
            if (y == x + 1 && y % 2) {
                ans.push_back(P(n - 1, x));
                ans.push_back(P(x, y));
                ans.push_back(P(n - 1, y));
            } else
                ans.push_back(p);
        }
    } else {
        puts("NO");
        return;
    }
    puts("YES");
    for (int i = 0; i < ans.size(); i++) {
        P p = ans[i];
        if (p.first > p.second) swap(p.first, p.second);
        printf("%d %d\n", p.first + 1, p.second + 1);
    }
}
int main() {
    while (~scanf("%d", &n)) solve();
    return 0;
}
