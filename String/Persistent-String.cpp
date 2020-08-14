/*
    可持久化字符串
    his[i]=new rope<char>(*his[i-1]) 即可O(1)拷贝历史版本
*/
/*
    要求支持操作
    1.在pos位置插入一串字符
    2.从pos位置开始删除长度为l的字符串
    3.查询第v个历史版本中从pos位置开始的长度为l的字符串
    pos,l,v均需要减去d，d的定义：所有询问中输出的字符‘c’的数量和
*/
#include <bits/stdc++.h>
#include <ext/rope>
using namespace std;
using namespace __gnu_cxx;
const int N = 50010;
rope<char> *his[N];
char ch[210];
int n, d, op, cur, x, y, v;
int main() {
    scanf("%d", &n);
    his[0] = new rope<char>();
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &op);
        if (op == 1) {
            scanf("%d%s", &x, ch);
            x -= d;
            cur++;
            his[cur] = new rope<char>(*his[cur - 1]);
            his[cur]->insert(x, ch);
        } else if (op == 2) {
            scanf("%d%d", &x, &y);
            x -= d;
            y -= d;
            cur++;
            his[cur] = new rope<char>(*his[cur - 1]);
            his[cur]->erase(x - 1, y);
        } else {
            scanf("%d%d%d", &v, &x, &y);
            v -= d;
            x -= d;
            y -= d;
            auto res = his[v]->substr(x - 1, y);
            d += count(res.begin(), res.end(), 'c');
            cout << res << endl;
        }
    }
    return 0;
}
/*
    要求支持操作
    T 字符串末尾加字符x
    U 撤销最后x次操作(不包含查询操作)
    Q 查询字符串的第x个字母并输出
*/
#include <bits/stdc++.h>
#include <ext/rope>
using namespace std;
using namespace __gnu_cxx;
const int N = 1e5 + 10;
rope<char> *his[N];
int n, d[N];
inline int lowbit(int x) { return x & -x; }
inline void update(int x) {
    while (x <= n) {
        d[x]++;
        x += lowbit(x);
    }
}
inline int get(int x) {
    int res = 0;
    while (x) {
        res += d[x];
        x -= lowbit(x);
    }
    return res;
}
inline char getC() {
    char ch = getchar();
    while (!isalpha(ch)) ch = getchar();
    return ch;
}
inline int getint() {
    int res = 0;
    char ch, ok = 0;
    while (ch = getchar()) {
        if (isdigit(ch)) {
            res *= 10;
            res += ch - '0';
            ok = 1;
        } else if (ok)
            break;
    }
    return res;
}
// debug
void deb(rope<char> s) {
    for (int i = 0; i < s.length(); i++) cout << s[i];
    puts("");
}
int main() {
    n = getint();
    his[0] = new rope<char>();
    for (int i = 1; i <= n; i++) {
        his[i] = new rope<char>(*his[i - 1]);
        char opt = getC();
        if (opt == 'T') {
            // 字符串末尾加字符x
            char x = getC();
            his[i]->push_back(x);
            update(i);
        } else if (opt == 'U') {
            // 撤销最后x次操作(不包含查询操作)
            update(i);
            int x = getint();
            int l = 1, r = i, mid, now = get(i);
            while (l < r) {
                mid = (l + r) >> 1;
                if (now - get(mid) > x)
                    l = mid + 1;
                else
                    r = mid;
            }
            his[i] = his[l - 1];

        } else if (opt == 'Q') {
            // 查询字符串的第x个字母并输出
            int x = getint() - 1;
            putchar(his[i]->at(x));
            putchar('\n');
        }
    }
    return 0;
}
