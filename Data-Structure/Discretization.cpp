/*
    离散化
*/
void Discretization() {
    sort(b + 1, b + n + 1);
    int siz = unique(b + 1, b + n + 1) - (b + 1);
    for (int i = 1; i <= n; i++)
        w[i] = lower_bound(b + 1, b + siz + 1, w[i]) - b;
}