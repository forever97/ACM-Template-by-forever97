/*
    循环链表
    示例：在圈上选择m个不相邻的点使得其和最大
*/
const int N = 200010;
namespace Circular_List {
bool del[N];
int n, nxt[N], pre[N], val[N];
typedef pair<int, int> P;
priority_queue<P, vector<P> > Q;  // 大根堆
void Initialize() {
    while (Q.size()) Q.pop();
    for (int i = 1; i <= n; i++) pre[i] = i - 1;
    pre[1] = n;
    for (int i = 1; i <= n; i++) nxt[i] = i + 1;
    nxt[n] = 1;
    for (int i = 1; i <= n; i++) Q.push(make_pair(val[i], i)), del[i] = 0;
}
void Del(int x) {
    del[x] = 1;
    nxt[pre[x]] = nxt[x];
    pre[nxt[x]] = pre[x];
    nxt[x] = pre[x] = 0;
}
int Get() {
    while (del[Q.top().second]) Q.pop();
    int res = Q.top().second;
    Q.pop();
    return res;
}
}  // namespace Circular_List
int m;
int main() {
    using namespace Circular_List;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", &val[i]);
    if (m > n / 2) {
        puts("Error!");
        return 0;
    }
    Initialize();
    int ans = 0;
    for (int i = 1; i <= m; i++) {
        int x = Get();
        ans += val[x];
        val[x] = val[pre[x]] + val[nxt[x]] - val[x];
        Del(pre[x]);
        Del(nxt[x]);
        Q.push(make_pair(val[x], x));
    }
    printf("%d\n", ans);
    return 0;
}