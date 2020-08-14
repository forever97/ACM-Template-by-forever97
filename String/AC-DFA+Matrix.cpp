/*
    求长度为n的不包含任何给出字符串的串数量
*/
#include <cstdio>
#include <cstring>
using namespace std;
const int N = 110;
typedef long long LL;
LL P = 100000LL;
struct mat {
    int n;
    LL num[110][110];
    void init0(int t) {
        n = t;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) num[i][j] = 0;
    }
    void init1(int t) {
        n = t;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (i != j)
                    num[i][j] = 0;
                else
                    num[i][j] = 1;
    }
    mat operator=(const struct mat p) {
        n = p.n;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) num[i][j] = p.num[i][j];
    }
    mat operator*(const struct mat p) const {
        struct mat ans;
        ans.init0(n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    ans.num[i][j] =
                        (ans.num[i][j] + num[i][k] * p.num[k][j]) % P;
        return ans;
    }
    mat operator^(int t) const {
        struct mat ans, now;
        ans.init1(n);
        now.n = n;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) now.num[i][j] = num[i][j];
        while (t > 0) {
            if (t & 1) ans = ans * now;
            now = now * now;
            t >>= 1;
        }
        return ans;
    }
} mat;
namespace AC_DFA {
const int Csize = 4;
int tot, son[N][Csize], sum[N], fail[N], q[N], ans[N], match[N];
void Initialize() {
    memset(sum, 0, sizeof(int) * (tot + 1));
    memset(ans, 0, sizeof(int) * (tot + 1));
    memset(match, 0, sizeof(int) * (tot + 1));
    memset(fail, 0, sizeof(int) * (tot + 1));
    for (int i = 0; i <= tot; i++)
        for (int j = 0; j < Csize; j++) son[i][j] = 0;
    tot = 0;
    fail[0] = -1;
}
inline int Tr(char ch) {
    if (ch == 'A') return 0;
    if (ch == 'T') return 1;
    if (ch == 'C') return 2;
    if (ch == 'G') return 3;
}
int Insert(char *s) {
    int x = 0;
    for (int l = strlen(s), i = 0, w; i < l; i++) {
        if (!son[x][w = Tr(s[i])]) {
            son[x][w] = ++tot;
        }
        x = son[x][w];
    }
    sum[x]++;
    return x;
}
void MakeFail() {
    int h = 1, t = 0, i, j, x = 0;
    for (i = 0; i < Csize; i++)
        if (son[0][i]) {
            q[++t] = son[0][i];
            match[son[0][i]] =
                sum[son[0][i]] ? son[0][i] : match[fail[son[0][i]]];
        }
    while (h <= t)
        for (x = q[h++], i = 0; i < Csize; i++)
            if (son[x][i]) {
                fail[son[x][i]] = son[fail[x]][i], q[++t] = son[x][i];
                match[son[x][i]] =
                    sum[son[x][i]] ? son[x][i] : match[fail[son[x][i]]];
            } else
                son[x][i] = son[fail[x]][i];
}
}  // namespace AC_DFA
using namespace AC_DFA;
char s[20];
void BuildMat() {
    mat.init0(tot + 1);
    for (int i = 0; i <= tot; i++) {
        if (match[i]) continue;
        for (int j = 0; j < Csize; j++) {
            if (!match[son[i][j]]) mat.num[i][son[i][j]]++;
        }
    }
}
int n, m;
int main() {
    while (~scanf("%d%d", &m, &n)) {
        Initialize();
        while (m--) {
            scanf("%s", s);
            Insert(s);
        }
        MakeFail();
        BuildMat();
        mat = mat ^ n;
        int ans = 0;
        for (int i = 0; i < mat.n; i++) ans = (ans + mat.num[0][i]) % P;
        printf("%d\n", ans);
    }
    return 0;
}
