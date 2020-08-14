/*
    单调队列
    求[i-k+1,i]中的最小最大值
*/
namespace Monotone_Queue {
int n, k;
int a[N], maxv[N], minv[N];
int deq[N];
void getMax() {
    int l = 0, r = 0;
    for (int i = 1; i <= n; i++) {
        while (l < r && a[deq[r - 1]] <= a[i]) r--;
        dep[r++] = i;
        if (i >= k) {
            maxv[i - k + 1] = a[deq[l]];
            if (deq[l] == i - k + 1) l++;
        }
    }
}
void getMin() {
    int l = 0, r = 0;
    for (int i = 1; i <= n; i++) {
        while (l < r && a[deq[r - 1]] >= a[i]) r--;
        deq[r++] = i;
        if (i >= k) {
            minv[i - k + 1] = a[deq[l]];
            if (deq[l] == i - k + 1) l++;
        }
    }
}
}  // namespace Monotone_Queue