/*
    单调栈
*/
namespace Monotone_Stack {
int n, a[N], l[N], r[N];
LL LargestArea() {
    stack<int> s;
    for (int i = 1; i <= n; i++) {
        while (s.size() && a[s.top()] >= a[i]) s.pop();
        l[i] = s.size() ? s.top() : 0;
        s.push(i);
    }
    while (s.size()) s.pop();
    for (int i = n; i >= 1; i--) {
        while (s.size() && a[s.top()] >= a[i]) s.pop();
        r[i] = s.size() ? s.top() : n + 1;
        s.push(i);
    }
    LL ans = 0;
    for (int i = 1; i <= n; i++) ans = max(ans, 1LL * a[i] * (r[i] - l[i] - 1));
    return ans;
}
}  // namespace Monotone_Stack