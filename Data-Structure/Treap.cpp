/*
    Treap
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
}  // namespace Treap