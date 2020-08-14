/*
    双端回文树
    Operation
        1. 在串首插入字符
        2. 在串尾插入字符
        3. 查询非重回文串数量
        4. 查询回文串数量
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long LL;
const int N = 200010, S = 26;
struct Palindromic_Tree {
    int son[N][S], fail[N], len[N];
    int text[N], L, R, num[N], lst[2], p;
    LL tot;
    int newnode(int l) {
        for (int i = 0; i < S; i++) son[p][i] = 0;
        num[p] = 0;
        len[p] = l;
        return p++;
    }
    void init(int n) {
        p = 0;
        newnode(0);
        newnode(-1);
        memset(text, -1, sizeof(text));
        L = n;
        R = n - 1;
        lst[0] = lst[1] = 1;
        fail[0] = 1;
        tot = 0;
    }
    int get_fail(int v, bool d) {
        if (d)
            while (text[R - len[v] - 1] != text[R]) v = fail[v];
        else
            while (text[L + len[v] + 1] != text[L]) v = fail[v];
        return v;
    }
    void add(int c, bool d) {
        if (d)
            text[++R] = c;
        else
            text[--L] = c;
        int x = get_fail(lst[d], d);
        if (!son[x][c]) {
            int y = newnode(len[x] + 2);
            fail[y] = son[get_fail(fail[x], d)][c];
            son[x][c] = y;
            num[y] = num[fail[y]] + 1;
        }
        lst[d] = son[x][c];
        if (len[lst[d]] == R - L + 1) lst[d ^ 1] = lst[d];
        tot += num[lst[d]];
    }
};
Palindromic_Tree T;
int n;
void solve() {
    int op;
    char c;
    T.init(n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &op);
        if (op <= 2) {
            scanf(" %c", &c);
            T.add(c - 'a', op - 1);
        }
        if (op == 3) printf("%d\n", T.p - 2);
        if (op == 4) printf("%lld\n", T.tot);
    }
}
int main() {
    while (~scanf("%d", &n)) solve();
    return 0;
}
