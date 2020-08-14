/*
    启发式合并Treap
*/
namespace Treap {
struct node {
    int val, cnt, size, p, id;
    node *l, *r;
    node() {}
    node(int val, int id) : val(val), id(id) {
        p = rand();
        cnt = size = 1;
        l = r = NULL;
    }
    void up() {
        size = cnt;
        if (l) size += l->size;
        if (r) size += r->size;
    }
} * root[N], *pool[N];
int top;
node* new_node() { return pool[top++]; }
void Initialize() {
    top = 0;
    for (int i = 0; i < N; i++) pool[i] = new node();
}
void Rotatel(node*& x) {
    node* y = x->r;
    x->r = y->l;
    x->up();
    y->l = x;
    y->up();
    x = y;
}
void Rotater(node*& x) {
    node* y = x->l;
    x->l = y->r;
    x->up();
    y->r = x;
    y->up();
    x = y;
}
//往treap上插入点
void Insert(node*& x, node y) {
    if (!x) {
        x = new_node();
        (*x) = y;
        return;
    }
    x->size++;
    if (y.val == x->val) {
        x->cnt++;
        return;
    }
    if (y.val < x->val) {
        Insert(x->l, y);
        if (x->l->p > x->p) Rotater(x);
    } else {
        Insert(x->r, y);
        if (x->r->p > x->p) Rotatel(x);
    }
}
// 查找第k小的元素
int kth(node* x, int rnk) {
    while (x) {
        int d = x->l ? x->l->size : 0;
        if (rnk <= d)
            x = x->l;
        else if (rnk > d + x->cnt)
            rnk -= d + x->cnt, x = x->r;
        else
            return x->id;
    }
    return -1;
}
// 删除一个p
void Delete(node*& x, int p) {
    x->size--;
    if (p == x->val) {
        x->cnt--;
        return;
    }
    if (p < x->val)
        Delete(x->l, p);
    else
        Delete(x->r, p);
}
// 查询大于p的数字的个数
int Ask(node* x, int p) {
    if (!x) return 0;
    if (p == x->val) return x->r->size;
    if (p < x->val) return x->cnt + x->r->size + Ask(x->l, p);
    return Ask(x->r, p);
}
// 查询在[c,d]范围内的数字的个数
int Ask(node* x, int a, int b, int c, int d) {
    if (!x) return 0;
    if (c <= a && b <= d) return x->size;
    int t = c <= x->val && x->val <= d ? x->cnt : 0;
    if (c < x->val) t += Ask(x->l, a, x->val - 1, c, d);
    if (d > x->val) t += Ask(x->r, x->val + 1, b, c, d);
}
void Del_node(node*& u) {
    pool[--top] = u;
    u = NULL;
}
// 将B树并入A树
void merge(node*& A, node*& B) {
    if (!B) return;
    if (B->l) merge(A, B->l);
    if (B->r) merge(A, B->r);
    Insert(A, *B);
    Del_node(B);
}
}  // namespace Treap
int n, m, k, u, v, q;
namespace Union_Find_Set {
int f[M], size[M], block;
void Initialize() {
    memset(f, 0, sizeof(f));
    block = n;
}
int Find(int x) {
    if (!f[x]) f[x] = x, size[x] = 1;
    if (f[x] == x) return x;
    return f[x] = Find(f[x]);
}
void Union(int x, int y) {
    x = Find(x);
    y = Find(y);
    if (x == y) return;
    if (size[x] > size[y]) swap(x, y);
    f[x] = y;
    size[y] += size[x];
    block--;
}
}  // namespace Union_Find_Set
void Heuristic_merge(int x, int y) {
    int fx = Union_Find_Set::Find(x);
    int fy = Union_Find_Set::Find(y);
    if (fx == fy) return;
    if (Treap::root[fx]->size < Treap::root[fy]->size) swap(x, y), swap(fx, fy);
    Treap::merge(Treap::root[fx], Treap::root[fy]);
    Union_Find_Set::f[fy] = fx;
}
/*
    Test
    有连边操作和查询操作，每个点有其重要度
    查询要求找出连通块中重要度第k大的点的id
*/
int main() {
    using namespace Treap;
    Initialize();
    Union_Find_Set::Initialize();
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &k);
        Union_Find_Set::f[i] = i;
        Insert(root[i], node(k, i));
    }
    while (m--) {
        scanf("%d%d", &u, &v);
        Heuristic_merge(u, v);
    }
    scanf("%d", &q);
    char op[5];
    while (q--) {
        scanf("%s", op);
        if (op[0] == 'B') {
            scanf("%d%d", &u, &v);
            Heuristic_merge(u, v);
        } else {
            scanf("%d%d", &u, &v);
            printf("%d\n", kth(root[Union_Find_Set::Find(u)], v));
        }
    }
    return 0;
}