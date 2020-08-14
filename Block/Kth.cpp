/*
    求静态区间第k大
*/
const int B = 1000, N = 100010, M = 5010;
int n, m, A[N], num[N], l, r, k;
vector<int> block[N / B];
int main() {
    while (~scanf("%d%d", &n, &m)) {
        memset(block, 0, sizeof(block));
        for (int i = 0; i < n; i++) {
            scanf("%d", &A[i]);
            block[i / B].push_back(A[i]);
            num[i] = A[i];
        }
        sort(num, num + n);
        for (int i = 0; i < n / B; i++) sort(block[i].begin(), block[i].end());
        for (int i = 0; i < m; i++) {
            scanf("%d%d%d", &l, &r, &k);
            l--;
            int L = 0, R = n - 1, ans = 0;
            while (L <= R) {
                int mid = (L + R) >> 1, x = num[mid];
                int tl = l, tr = r, c = 0;
                while (tl < tr && tl % B != 0)
                    if (A[tl++] <= x) c++;
                while (tl < tr && tr % B != 0)
                    if (A[--tr] <= x) c++;
                while (tl < tr) {
                    int b = tl / B;
                    tl += B;
                    c += upper_bound(block[b].begin(), block[b].end(), x) -
                         block[b].begin();
                }
                if (c >= k)
                    ans = mid, R = mid - 1;
                else
                    L = mid + 1;
            }
            printf("%d\n", num[ans]);
        }
    }
    return 0;
}