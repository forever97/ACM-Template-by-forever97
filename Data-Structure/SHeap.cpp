/*
    斜堆
    可并堆
*/
struct SHeap {
    int v[N], l[N], r[N];
    int merge(int x, int y) {
        if (x == 0 || y == 0) return x + y;
        if (v[x] < v[y]) swap(x, y);
        r[x] = merge(r[x], y);
        swap(l[x], r[x]);
        return x;
    }
    void pop(int &x) { x = merge(l[x], r[x]); }
    int top(int x) { return v[x]; }
} heap;