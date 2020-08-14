/*
    区间查询区间求和
    用栈存储修改操作
    当数量大于限定时重构前缀和
    复杂度O(nsqrt(n))
*/
typedef long long LL;
char op[3];
struct data {
    int x, y;
    LL c;
} st[100005];
int N, Q, top = 0, size, x, y;
LL s[100010], a[100010];
int main() {
    scanf("%d%d", &N, &Q);
    size = (int)sqrt(N * 1.0);
    for (int i = 1; i <= N; i++) {
        scanf("%lld", &s[i]);
        s[i] += s[i - 1];
    }
    while (Q--) {
        scanf(" %s", op);
        if (op[0] == 'C') {
            ++top;
            scanf("%d%d%lld", &st[top].x, &st[top].y, &st[top].c);
            a[st[top].x] += st[top].c;
            a[st[top].y + 1] -= st[top].c;
            if (top > size) {
                LL tmp = 0;
                for (int i = 1; i <= N; i++) {
                    a[i] += a[i - 1];
                    tmp += a[i];
                    s[i] += tmp;
                }
                for (int i = 1; i <= N; i++) a[i] = 0;
                top = 0;
            }
        } else {
            scanf("%d%d", &x, &y);
            LL ans = s[y] - s[x - 1];
            for (int i = 1; i <= top; i++) {
                if (y < st[i].x || x > st[i].y) continue;
                ans +=
                    st[i].c * (LL)(st[i].y - st[i].x + 1 - max(st[i].y - y, 0) -
                                   max(x - st[i].x, 0));
            }
            printf("%lld\n", ans);
        }
    }
    return 0;
}