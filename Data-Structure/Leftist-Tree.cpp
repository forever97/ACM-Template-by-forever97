/*
    左偏树
*/
namespace Leftist_Tree {
typedef pair<int, int> P;
struct Node {
    int l, r, d;
    P v;
    Node() {}
    Node(int _l, int _r, int _d, P _v) { l = _l, r = _r, d = _d, v = _v; }
} T[N];
// 并查集判断两个点是否在同一棵左偏树上
int f[N];
int sf(int x) { return x == f[x] ? x : f[x] = sf(f[x]); }
// 返回值为合并后的根节点
int Merge(int a, int b) {
    if (!a) return b;
    if (!b) return a;
    if (T[a].v < T[b].v) swap(a, b);
    T[a].r = Merge(T[a].r, b);
    if (T[T[a].l].d < T[T[a].r].d) swap(T[a].l, T[a].r);
    T[a].d = T[a].r ? T[T[a].r].d + 1 : 0;
    return a;
}
// 返回值为删去了堆顶元素的新左偏树根节点编号
int Pop(int a) {
    int l = T[a].l, r = T[a].r;
    T[a].l = T[a].r = T[a].d = 0;
    return Merge(l, r);
}
// 新建一棵单节点的树，权值为y，下标为x
int Newnode(int x, int y) {
    T[x] = Node(0, 0, 0, y);
    f[x] = x;
}
}  // namespace Leftist_Tree
