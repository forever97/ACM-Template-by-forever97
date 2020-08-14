/*
    楼层扔鸡蛋问题
    G[i][j]表示用i个鸡蛋尝试j次所能测出的楼层最高高度
*/
typedef long long LL;
LL G[200][100];
void init() {
    G[0][0] = G[1][0] = 0;
    for (int i = 1; i <= 100; i++) {
        for (int j = 1; j <= 63; j++) {
            G[i][j] = G[i - 1][j - 1] + G[i][j - 1] + 1;
        }
    }
}
LL k, n;
void solve() {
    for (int i = 1; i <= 63; i++)
        if (G[k][i] >= n) {
            printf("%d\n", i);
            return;
        }
    puts("More than 63 trials needed.");
}
int main() {
    init();
    while (scanf("%lld%lld", &k, &n), k) solve();
    return 0;
}
