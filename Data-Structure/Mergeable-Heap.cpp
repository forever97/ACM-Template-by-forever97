/*
   Mergeable_Heap
   可并随机堆+set
*/
#define N 301000
#define inf 0x3f3f3f3f
#define ls son[x][0]
#define rs son[x][1]
#define LS son[y][0]
#define RS son[y][1]
using namespace std;
multiset<int> H;
struct Mergeable_Heap {
    int son[N][2], val[N], flag[N], FLAG;
    int root[N], fa[N];
    int find(int x) {
        if (root[x] == x) return x;
        return root[x] = find(root[x]);
    }
    int stk[N], top;
    void init(int n) {
        val[0] = -inf;
        for (int i = 1; i <= n; i++) {
            scanf("%d", &val[i]);
            H.insert(val[i]);
            root[i] = i;
        }
    }
    void pushdown(int x) {
        val[x] += flag[x];
        if (ls) flag[ls] += flag[x];
        if (rs) flag[rs] += flag[x];
        flag[x] = 0;
    }
    int merge(int x, int y) {
        pushdown(x), pushdown(y);
        if (val[x] < val[y]) swap(x, y);
        if ((x && y) == 0) return x;
        int d = rand() & 1;
        son[x][d] = merge(son[x][d], y);
        if (son[x][d]) fa[son[x][d]] = x;
        return x;
    }
    void U() {
        int x, y;
        scanf("%d%d", &x, &y);
        int fx = find(x), fy = find(y);
        if (fx == fy) return;
        root[fx] = root[fy] = merge(fx, fy);
        if (root[fx] == fx)
            H.erase(H.find(val[fy]));
        else
            H.erase(H.find(val[fx]));
    }
    void A1() {
        int x, y, A, B;
        scanf("%d%d", &x, &A);
        for (stk[top = 1] = x, y = fa[x]; y; y = fa[y]) stk[++top] = y;
        int head = stk[top];
        H.erase(H.find(val[head]));
        while (top) pushdown(stk[top--]);
        val[x] += A;
        y = fa[x];
        int i = (x == son[y][1]);
        son[y][i] = fa[x] = 0;
        A = ls, B = rs;
        fa[A] = fa[B] = 0;
        ls = rs = 0;
        A = merge(A, B);
        if (A) fa[A] = y;
        if (y) son[y][i] = A, A = head;
        int last = head;
        root[A] = root[x] = root[head] = merge(x, A);
        root[0] = 0;
        pushdown(root[x]);
        H.insert(val[root[x]]);
    }
    void A2() {
        int x, y;
        scanf("%d%d", &x, &y);
        H.erase(H.find(val[find(x)]));
        flag[find(x)] += y;
        pushdown(root[x]);
        H.insert(val[root[x]]);
    }
    void A3() {
        int x;
        scanf("%d", &x);
        FLAG += x;
    }
    void F1() {
        int x, y;
        scanf("%d", &x);
        pushdown(x);
        if (x == root[x]) {
            printf("%d\n", val[x] + FLAG);
            return;
        }
        for (stk[top = 1] = x, y = fa[x]; y; y = fa[y]) stk[++top] = y;
        while (top) pushdown(stk[top--]);
        printf("%d\n", val[x] + FLAG);
    }
    void F2() {
        int x;
        scanf("%d", &x);
        pushdown(find(x));
        printf("%d\n", val[root[x]] + FLAG);
    }
    void F3() {
        multiset<int>::iterator it = H.end();
        printf("%d\n", (*(--it)) + FLAG);
    }
} heap;
int n, m;
char s[5];
int main() {
    srand(252503);
    int i, j, k;
    scanf("%d", &n);
    heap.init(n);
    for (scanf("%d", &m); m--;) {
        scanf("%s", s);
        // 加一条边，连接第x个节点和第y个节点
        if (s[0] == 'U') heap.U();
        // 将第x个节点的权值增加v
        else if (s[0] == 'A' && s[1] == '1')
            heap.A1();
        // 将第x个节点所在的连通块的所有节点的权值都增加v
        else if (s[0] == 'A' && s[1] == '2')
            heap.A2();
        // 将所有节点的权值增加v
        else if (s[0] == 'A' && s[1] == '3')
            heap.A3();
        // 输出第x个节点当前的权值
        else if (s[0] == 'F' && s[1] == '1')
            heap.F1();
        // 输出第x个节点所在的连通块中，权值最大的节点的权值
        else if (s[0] == 'F' && s[1] == '2')
            heap.F2();
        // 输出所有节点中，权值最大的节点的权值
        else if (s[0] == 'F' && s[1] == '3')
            heap.F3();
    }
    return 0;
}