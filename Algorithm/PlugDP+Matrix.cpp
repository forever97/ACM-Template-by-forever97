/*
    插头DP+矩阵快速幂
    求一个n*m的格子图中，从左上角到右下角经过所有格子的方案数
    2<=n<=7
    1<=m<=10^9
*/
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long LL;
const int mod = 7777777;
struct mat {
    int n, m;
    int v[200][200];
};
mat mul(mat a, mat b) {
    mat ret;
    ret.n = a.n;
    ret.m = b.m;
    for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < b.m; j++) {
            LL sum = 0;
            for (int k = 0; k < a.m; k++) sum += 1LL * a.v[i][k] * b.v[k][j];
            ret.v[i][j] = sum % mod;
        }
    }
    return ret;
}
mat pow(mat a, int n) {
    mat ret = a;
    memset(ret.v, 0, sizeof(ret.v));
    for (int i = 0; i < a.n; i++) ret.v[i][i] = 1;
    mat tmp = a;
    while (n) {
        if (n & 1) ret = mul(ret, tmp);
        tmp = mul(tmp, tmp);
        n >>= 1;
    }
    return ret;
}
const int bit = 3, dig = 2;
const int N = 200010;
const int SIZE = 4001;
struct Hash_Table {
    int size, g[SIZE], nxt[N];
    LL state[N];
    void init() {
        size = 0;
        memset(g, -1, sizeof(g));
    }
    int push(LL S) {
        int id = S % SIZE;
        for (int i = g[id]; ~i; i = nxt[i]) {
            if (state[i] == S) {
                return i;
            }
        }
        state[size] = S;
        nxt[size] = g[id], g[id] = size++;
        return size - 1;
    }
} dp;
int n, m;
int code[20], vis[20];
int cur, pre;
char mp[20][20];
int edx, edy;
inline void Decode(LL S, int m) {
    for (int i = m - 1; i >= 0; i--) code[i] = S & bit, S >>= dig;
}
inline LL Encode(int m) {
    LL res = 0;
    int x = 1;
    memset(vis, -1, sizeof(vis));
    vis[0] = 0;
    for (int i = 0; i < m; i++) {
        if (-1 == vis[code[i]]) vis[code[i]] = x++;
        code[i] = vis[code[i]];
        res <<= dig;
        res |= code[i];
    }
    return res;
}
bool check(int st, int nst) {
    Decode(st, n);
    int flag = 0, cnt = 0, k;
    for (int i = 0; i < n; i++) {
        if (flag == 0) {
            if (code[i] == 0 && (nst & (1 << i)) == 0) return 0;
            if (code[i] && (nst & (1 << i))) continue;
            if (code[i])
                flag = code[i];
            else
                flag = -1;
            k = i;
        } else {
            if (code[i] && (nst & (1 << i))) return 0;
            if (code[i] == 0 && (nst & (1 << i)) == 0) continue;
            if (code[i]) {
                if (code[i] == flag && ((nst != 0) || i != n - 1)) return 0;
                if (flag > 0) {
                    for (int j = 0; j < N; j++)
                        if (code[j] == code[i] && j != i) code[j] = code[k];
                    code[i] = code[k] = 0;
                } else {
                    code[k] = code[i];
                    code[i] = 0;
                }
            } else {
                if (flag > 0)
                    code[i] = code[k], code[k] = 0;
                else
                    code[i] = code[k] = n + (cnt++);
            }
            flag = 0;
        }
    }
    if (flag != 0) return 0;
    return 1;
}
int D, g[200][200];
struct Node {
    int g[200][200];
    int D;
} node[20];
void init() {
    if (node[n].D != 0) {
        memcpy(g, node[n].g, sizeof(node[n].g));
        D = node[n].D;
        return;
    }
    int st, nst;
    dp.init();
    memset(code, 0, sizeof(code));
    code[0] = code[n - 1] = 1;
    dp.push(0);
    dp.push(Encode(n));
    memset(g, 0, sizeof(g));
    for (int i = 1; i < dp.size; i++) {
        st = dp.state[i];
        for (nst = 0; nst < (1 << n); nst++)
            if (check(st, nst)) {
                int j = dp.push(Encode(n));
                g[i][j] = 1;
            }
    }
    D = dp.size;
    memcpy(node[n].g, g, sizeof(g));
    node[n].D = D;
}
int main() {
    for (int i = 0; i < 20; i++) node[i].D = 0;
    while (~scanf("%d%d", &n, &m)) {
        init();
        mat tmp;
        tmp.n = tmp.m = D;
        memcpy(tmp.v, g, sizeof(g));
        mat ans = pow(tmp, m);
        if (ans.v[1][0] == 0)
            printf("Impossible\n");
        else
            printf("%d\n", ans.v[1][0] % mod);
    }
    return 0;
}
