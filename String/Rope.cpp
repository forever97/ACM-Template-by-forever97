/*
    rope
    append(x):字符串末尾添加x
    insert(pos,sub):在pos位置插入字符串sub
    erase(pos,x):从pos位置开始删除x个字符
    replace(pos,x):将pos位置的字符替换成x串
    substr(pos,x):提取pos开始的x个字符，返回rope类型
    s.copy(pos,len,t):将从s第6位开始的len个字符插入到t开头
    at(x)/[x]:访问第x个元素
    翻转操作实现方法:同时维护一正一反两个rope,反转即交换两个子串
    区间循环位移:拆为多个子串拼接即可
    

*/
#include <bits/stdc++.h>
#include <ext/rope>
using namespace std;
using namespace __gnu_cxx;
int n, m;
inline char nc() {
    static char buf[100000], *p1 = buf, *p2 = buf;
    if (p1 == p2) {
        p2 = (p1 = buf) + fread(buf, 1, 100000, stdin);
        if (p1 == p2) return EOF;
    }
    return *p1++;
}
inline void read(int &x) {
    char c = nc(), b = 1;
    for (; !(c >= '0' && c <= '9'); c = nc())
        if (c == '-') b = -1;
    for (x = 0; c >= '0' && c <= '9'; x = x * 10 + c - '0', c = nc())
        ;
    x *= b;
}
int main() {
    read(n);
    rope<int> s;
    for (int i = 1; i <= n; i++) s.append(i);
    read(m);
    while (m--) {
        int st, len;
        read(st);
        read(len);
        auto sub = s.substr(st - 1, len);
        s.erase(st - 1, len);
        s.insert(0, sub);
    }
    for (int i = 0; i < n; i++) printf("%d%c", s[i], " \n"[i == n - 1]);
    return 0;
}
