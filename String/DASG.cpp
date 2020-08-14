/*
    子序列自动机
    复杂度O(n*SIGMA)
*/
#include <bits/stdc++.h>
using namespace std;
struct DASG {
    static const int SIZE = 100000 + 10, SIGMA = 26;
    struct Node {
        Node *ch[SIGMA];
        Node() { memset(ch, 0, sizeof(ch)); }
    } pool[SIZE], *sz, *rt, *last[SIGMA];
    void init() {
        sz = pool;
        rt = sz++;
        std::fill(last, last + SIGMA, rt);
    }
    void add(int x) {
        Node *np = sz++;
        for (Node *p = last[x]; p < np; ++p) {
            if (!p->ch[x]) p->ch[x] = np;
        }
        last[x] = np;
    }
} dasg;
char s[100010];
int m;
int main() {
    scanf("%s", s);
    int n = strlen(s);
    dasg.init();
    for (int i = 0; i < n; i++) dasg.add(s[i] - 'a');
    scanf("%d", &m);
    while (m--) {
        scanf("%s", s);
        int len = strlen(s);
        DASG::Node *x = dasg.rt;
        for (int i = 0; i < len; i++) {
            if (!x->ch[s[i] - 'a']) {
                puts("No");
                break;
            }
            x = x->ch[s[i] - 'a'];
            if (i == len - 1) puts("Yes");
        }
    }
    return 0;
}
// 求串s的最长的子序列使得其字典序大于串t
#include <bits/stdc++.h>
using namespace std;
const int N = 1000010;
int ps[N];
int ch[N][26];
int sz, rt, lst[26];
void init() {
    sz = 0;
    rt = sz++;
}
void add(int x, int pos) {
    ps[sz] = pos;
    int np = sz++;
    for (int p = lst[x]; p < np; ++p) {
        if (!ch[p][x]) ch[p][x] = np;
    }
    lst[x] = np;
}
char s[N], t[N];
int n, m;
int main() {
    scanf("%d%d", &n, &m);
    scanf("%s", s);
    scanf("%s", t);
    int ans = -1;
    init();
    for (int i = 0; i < n; i++) add(s[i] - 'a', i);
    int x = rt;
    for (int i = 0; i < m; i++) {
        for (int j = t[i] - 'a' + 1; j < 26; j++) {
            if (!ch[x][j]) continue;
            ans = max(ans, n - ps[ch[x][j]] + i);
        }
        if (!ch[x][t[i] - 'a']) break;
        x = ch[x][t[i] - 'a'];
        if (i == m - 1 && ps[x] != n - 1) ans = max(ans, n - ps[x] + i);
    }
    printf("%d\n", ans);
    return 0;
}
