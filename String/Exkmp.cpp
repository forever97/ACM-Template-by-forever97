/*
    扩展kmp
    LCP表示T第i位往后和T原串的最长公共前缀
    extend表示S的第i位往后和T的最长公共前缀
*/
namespace Exkmp {
const int N = 500010;
int len, LCP[N], ex1[N], ex2[N], a[N], s[N];
char S[N], T[N];
void getLCP(char *T) {
    int i, len = strlen(T);
    LCP[0] = len;
    for (i = 0; i < len - 1 && T[i] == T[i + 1]; i++)
        ;
    LCP[1] = i;
    int a = 1;
    for (int k = 2; k < len; k++) {
        int p = a + LCP[a] - 1, L = LCP[k - a];
        if ((k - 1) + L >= p) {
            int j = (p - k + 1) > 0 ? (p - k + 1) : 0;
            while (k + j < len && T[k + j] == T[j]) j++;
            LCP[k] = j, a = k;
        } else
            LCP[k] = L;
    }
}
void exkmp(char *S, char *T, int *extend) {
    memset(LCP, 0, sizeof(LCP));
    getLCP(T);
    int Slen = strlen(S), Tlen = strlen(T), a = 0;
    int MinLen = Slen > Tlen ? Tlen : Slen;
    while (a < MinLen && S[a] == T[a]) a++;
    extend[0] = a, a = 0;
    for (int k = 1; k < Slen; k++) {
        int p = a + extend[a] - 1, L = LCP[k - a];
        if ((k - 1) + L >= p) {
            int j = (p - k + 1) > 0 ? (p - k + 1) : 0;
            while (k + j < Slen && j < Tlen && S[k + j] == T[j]) j++;
            extend[k] = j;
            a = k;
        } else
            extend[k] = L;
    }
}
}  // namespace Exkmp
/*
    Example1
    一个字符串的价值定义为，当它是一个回文串的时候，
    价值为每个字符的价值的和，如果不是回文串，价值为0，现在给出每种字符的价值。
    给出一个字符串，要求将其划分为两个子串，要求两个子串的价值和最大。
*/
void revcpy(char *S, char *T, int len) {
    memset(T, 0, sizeof(T));
    for (int i = 0, k = len - 1; i < len; ++i, --k) T[i] = S[k];
}
int Cas;
int main() {
    using namespace Exkmp;
    scanf("%d", &Cas);
    while (Cas--) {
        for (int i = 0; i < 26; i++) scanf("%d", &a[i]);
        scanf("%s", S);
        len = strlen(S);
        for (int i = 0; S[i]; i++) s[i + 1] = s[i] + a[S[i] - 'a'];
        revcpy(S, T, len);
        exkmp(S, T, ex2);
        exkmp(T, S, ex1);
        int ans = -1e9;
        for (int i = 0; i < len; i++) {
            if (i && ex1[i] + i == len) {  // S前缀是回文串
                int j = ex1[i], tmp = s[j];
                if (ex2[j] + j == len) tmp += s[len] - s[j];  // S后缀是回文串
                if (tmp > ans) ans = tmp;
            } else {
                int j = i + 1, tmp = 0;
                if (ex2[j] + j == len) tmp += s[len] - s[j];
                if (tmp > ans) ans = tmp;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
/*
    Example2
    给出一个数字，每次将其最后一位提到最前面来，问产生的所有数字中，
    有多少比原数大，有多少比原数小，有多少和原数相等
*/
int main() {
    int Cas;
    scanf("%d", &Cas);
    for (int cas = 1; cas <= Cas; cas++) {
        int L = 0, E = 0, G = 0;
        scanf("%s", S);
        int len = strlen(S);
        for (int i = 0; i < len; i++) S[len + i] = S[i];
        S[len * 2] = '\0';
        getLCP(S);
        for (int i = 0; i < len; i++) {
            if (LCP[i] >= len)
                E++;
            else if (S[LCP[i]] < S[LCP[i] + i])
                G++;
            else
                L++;
        }
        printf("Case %d: %d %d %d\n", cas, L / E, E / E, G / E);
    }
    return 0;
}