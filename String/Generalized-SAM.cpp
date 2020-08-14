/*
    广义SAM
    题目大意：给出多个主串，和多个子串，问每个子串在多少个主串中出现过
    题解：对Trie建广义SAM，从根dfs保存lst，
    用SAM跑每个主串，状态维护cnt和cur分别为出现次数及上一次出现是哪个串，
    出现次数向父亲传递，所以要沿着Parent向上跑更新，遇到cur=当前串时停止
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>
using namespace std;
const int N = 200010;
typedef long long ll;
int n, Q;
string ss[N];
char s[N << 1];
struct node {
    int ch[130], par, val;
    int cnt, cur;
} t[N];
int sz = 1, root = 1, lst = 1;
void extend(int c) {
    int p = lst, np = ++sz;
    t[np].val = t[p].val + 1;
    for (; p && !t[p].ch[c]; p = t[p].par) t[p].ch[c] = np;
    if (!p)
        t[np].par = root;
    else {
        int q = t[p].ch[c];
        if (t[q].val == t[p].val + 1)
            t[np].par = q;
        else {
            int nq = ++sz;
            t[nq] = t[q];
            t[nq].val = t[p].val + 1;
            t[q].par = t[np].par = nq;
            for (; p && t[p].ch[c] == q; p = t[p].par) t[p].ch[c] = nq;
        }
    }
    lst = np;
}
void solve() {
    int u;
    for (int i = 1; i <= n; i++) {
        u = root;
        string &s = ss[i];
        for (int j = 0; j < s.size(); j++) {
            u = t[u].ch[s[j]];
            int p = u;
            for (; p && t[p].cur != i; p = t[p].par) t[p].cnt++, t[p].cur = i;
        }
    }
    while (Q--) {
        scanf("%s", s);
        int n = strlen(s), u = root, flag = 0;
        for (int i = 0; i < n; i++) {
            int c = s[i];
            if (t[u].ch[c])
                u = t[u].ch[c];
            else {
                flag = 1;
                break;
            }
        }
        if (flag)
            puts("0");
        else
            printf("%d\n", t[u].cnt);
    }
}
int main() {
    scanf("%d%d", &n, &Q);
    for (int i = 1; i <= n; i++) {
        scanf("%s", s);
        ss[i] = string(s);
        lst = root;
        int len = ss[i].size();
        for (int j = 0; j < len; j++) extend(s[j]);
    }
    solve();
    return 0;
}