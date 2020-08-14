/*
    笛卡尔树
    任意节点值比子树的节点值要小
    中序遍历可得原序列
    构造O(n)
*/
#define CartesianTree_size 500001;
struct Cartesian_Tree {
    int num[CartesianTree_size], size[CartesianTree_size];
    int l[CartesianTree_size], r[CartesianTree_size], fa[CartesianTree_size];
    int st[CartesianTree_size];
    int S, root;
    void get_size(int now) {
        if (l[now]) get_size(l[now]);
        if (r[now]) get_size(r[now]);
        size[now] = size[l[now]] + size[r[now]] + 1;
    }
    inline void build() {
        int i, top = 0;
        bool dec;
        for (i = 1; i <= S; ++i) {
            dec = false;
            while (top && num[i] < num[st[top]]) {
                dec = true;
                --top;
            }
            r[st[top]] = i;
            fa[i] = st[top];
            if (dec) l[i] = st[top + 1], fa[st[top + 1]] = i;
            ++top;
            st[top] = i;
        }
        root = st[1];
        get_size(root);
    }
} tree;
void dfs(int now) {
    int L = tree.l[now], R = tree.r[now];
    if (L) dfs(L);
    printf("%d", tree.num[now]);
    if (R) dfs(R);
}
int n;
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &tree.num[i]);
    tree.build();
    dfs(tree.root);
    return 0;
}