/*
    GarsiaWachs算法
    石子归并问题O(nlogn)解法
    N堆石子摆成一条线。现要将石子有次序地合并成一堆。
    规定每次只能选相邻的2堆石子合并成新的一堆，并将新的一堆石子数记为该次合并的代价。
    计算将N堆石子合并成一堆的最小代价。
*/
int a[N], n, i, t;
long long ans;
void combine(int k) {
    int tmp = a[k] + a[k - 1], i, j;
    ans += tmp;
    for (i = k; i < t - 1; i++) a[i] = a[i + 1];
    for (t--, j = k - 1; j > 0 && a[j - 1] < tmp; j--) a[j] = a[j - 1];
    a[j] = tmp;
    while (j >= 2 && a[j] >= a[j - 2]) i = t - j, combine(j - 1), j = t - i;
}
int main() {
    for (scanf("%d", &n), i = 0; i < n; i++) scanf("%d", a + i);
    for (t = i = 1; i < n; i++) {
        a[t++] = a[i];
        while (t >= 3 && a[t - 3] <= a[t - 1]) combine(t - 2);
    }
    while (t > 1) combine(t - 1);
    printf("%lld", ans);
    return 0;
}