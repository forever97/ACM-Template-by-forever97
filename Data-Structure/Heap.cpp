/*
    二叉堆 (大根堆)
*/
namespace Heap {
int h[N], l;
void down(int x) {
    int ls = x << 1, rs = l | 1, idx = x;
    if (ls <= l && h[ls] > h[idx]) idx = ls;
    if (rs <= l && h[rs] > h[idx]) idx = rs;
    if (idx != x) {
        swap(h[idx], h[x]);
        down(idx);
    }
}
void build() { l = 0; }
/*
    离线O(n)建立二叉堆
*/
void build(int a[], int n) {
    l = n;
    for (int i = 1; i <= n; i++) h[i] = a[i];
    for (int i = n >> 1; i; i--) down(i);
}
void push(int x) {
    h[++l] = x;
    for (int i = l; i > 1 && h[i] > h[i >> 1]; i >>= 1) swap(h[i], h[i >> 1]);
}
int top() { return h[1]; }
void pop() {
    h[1] = h[l--];
    for (int i = 1;;) {
        int tmp = h[i];
        int j = 0;
        if ((i << 1) <= l && h[i << 1] > tmp) tmp = h[j = i << 1];
        if ((i << 1 | 1) <= l && h[i << 1 | 1] > tmp) j = i << 1 | 1;
        if (j)
            swap(h[i], h[j]), i = j;
        else
            return;
    }
}
}  // namespace Heap
