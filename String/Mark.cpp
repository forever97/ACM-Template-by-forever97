/*
    字符串的使用技巧示例：
    对于单串长度未知单总长一定的串
    我们将其保存在一个char数组中并记录每个串的起始位置
    每次保留换行符方便长度计算和输出
    示例为寻找给出字符串集合中是否存在一个串包含所有串
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;
const int N = 200010;
char s[300010];
int T, n, id, len, L[N], nl;
struct SAM {
    int p, q, np, nq, cnt, lst, a[N][26], l[N], f[N];
    int Tr(char c) { return c - 'a'; }
    SAM() {
        cnt = 0;
        lst = ++cnt;
    }
    void Initialize() {
        memset(l, 0, sizeof(int) * (cnt + 1));
        memset(f, 0, sizeof(int) * (cnt + 1));
        for (int i = 0; i <= cnt; i++)
            for (int j = 0; j < 26; j++) a[i][j] = 0;
        cnt = 0;
        lst = ++cnt;
    }
    void extend(int c) {
        p = lst;
        np = lst = ++cnt;
        l[np] = l[p] + 1;
        while (!a[p][c] && p) a[p][c] = np, p = f[p];
        if (!p) {
            f[np] = 1;
        } else {
            q = a[p][c];
            if (l[p] + 1 == l[q]) {
                f[np] = q;
            } else {
                nq = ++cnt;
                l[nq] = l[p] + 1;
                memcpy(a[nq], a[q], sizeof(a[q]));
                f[nq] = f[q];
                f[np] = f[q] = nq;
                while (a[p][c] == q) a[p][c] = nq, p = f[p];
            }
        }
    }
    void build(int x) {
        int len = strlen(s + L[x]);
        for (int i = 0; i < len; i++) extend(Tr(s[L[x] + i]));
    }
    int Match(int x) {
        int len = strlen(s + L[x]);
        int p = 1;
        for (int i = 0; i < len; i++) {
            int c = Tr(s[L[x] + i]);
            if (!a[p][c]) return 0;
            p = a[p][c];
        }
        return 1;
    }
} sam;
int main() {
    scanf("%d", &T);
    while (T--) {
        int flag = 1;
        sam.Initialize();
        id = len = nl = 0;
        L[1] = nl;
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) {
            scanf("%s", s + nl);
            int l = strlen(s + nl);
            nl += l + 1;
            L[i + 1] = nl;
            if (l > len) len = l, id = i;
        }
        sam.build(id);
        for (int i = 1; i <= n; i++) {
            if (!sam.Match(i)) {
                puts("No");
                flag = 0;
                break;
            }
        }
        if (flag) puts(s + L[id]);
    }
    return 0;
}