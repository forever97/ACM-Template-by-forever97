/*
    题目大意：
        给出一个只包含三种字母的字符串，求出三种字母数量相等的最长子串
    题解：
        我们记录三个字母的前缀和，我们发现只要一个区段左右端点前缀和之差相等，
        就说明这个区段三个字母出现次数相等，其充要条件为SJ-SO和SO-SI相等，
        所以我们在map中保存双关键字第一次出现的位置，
        每次查询当前位置和第一次出现位置的距离
        求最大值即可。
*/
#include <algorithm>
#include <cstdio>
#include <map>
#include <utility>
using namespace std;
const int N = 200010;
int n;
char s[N];
int main() {
    while (~scanf("%d", &n)) {
        map<pair<int, int>, int> M;
        scanf("%s", s + 1);
        M[make_pair(0, 0)] = 0;
        int SJ = 0, SO = 0, SI = 0, ans = 0;
        for (int i = 1; i <= n; i++) {
            SJ += (s[i] == 'J');
            SO += (s[i] == 'O');
            SI += (s[i] == 'I');
            if (M.find(make_pair(SJ - SO, SO - SI)) == M.end())
                M[make_pair(SJ - SO, SO - SI)] = i;
            else
                ans = max(ans, i - M[make_pair(SJ - SO, SO - SI)]);
        }
        printf("%d\n", ans);
    }
    return 0;
}
