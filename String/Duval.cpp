/*
    Duval算法
        求出一个字符串的Lyndon分解
    Lyndon串
        串本身小于所有的循环位移结果
*/
#include <bits/stdc++.h>
using namespace std;
std::vector<int> duval(char s[]) {
    std::vector<int> ret;
    int n = strlen(s) + 1;  // zero used here
    int start = 0, mid = 1, cur = 0;
    ret.push_back(0);
    for (int i = 0; i < n; ++i) {
        if (s[i] == s[cur]) {
            if (++cur == mid) cur = start;
        } else if (s[i] > s[cur]) {
            mid = i + 1;
            cur = start;
        } else if (s[i] < s[cur]) {
            int temp = mid - start;
            while (start + temp <= i) {
                start += temp;
                ret.push_back(start);
                // 如果输出分界点直接在里面输出比较快
                // printf("%d ",start);
            }
            i = cur = start;
            mid = start + 1;
        }
    }
    return ret;
}
char s[2000100]; 
int main() {
    scanf("%s", s);
    auto res = duval(s);
//	for (auto v : res) {
//		if (v) printf("%d%c", v, " \n"[v==strlen(s)]);
//	}
    return 0;
}
