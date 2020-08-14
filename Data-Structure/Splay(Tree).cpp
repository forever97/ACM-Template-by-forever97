/*
    双旋Splay
*/
const int N = 200010, inf = ~0U >> 1;
namespace Splay {
int sum[N], key[N], son[N][2], f[N], root, tot, sumx;
void Initialize(int n) {
    tot = 0;
    root = 0;
}
int up(int x) { sum[x] = sum[son[x][0]] + sum[son[x][1]] + 1; }
int rotate(int x) {
    int y = f[x];
    int w = (son[y][0] == x);  // w为x与其父亲关系的相反值
    son[y][!w] = son[x][w];
    if (son[x][w]) f[son[x][w]] = y;
    if (f[y]) {
        int z = f[y];
        son[z][son[z][1] == y] = x;
    }
    f[x] = f[y];
    f[y] = x;
    son[x][w] = y;
    up(y);
    up(x);
}
int splay(int x) {
    while (f[x]) {
        int y = f[x];
        if (f[y]) {
            if ((son[f[y]][1] == y) ^ (son[y][1] == x))
                rotate(x);
            else
                rotate(y);
        }
        rotate(x);
    }
    root = x;
}
int newnode(int k) {
    sum[++tot] = 1;
    key[tot] = k;
    return tot;
}
int add(int x, int k) {
    if (!x) return splay(newnode(k));
    int w = (key[x] < k);
    if (!son[x][w])
        son[x][w] = newnode(k), f[son[x][w]] = x, splay(son[x][w]);
    else
        add(son[x][w], k);
    up(x);
}
int find(int x, int k) {
    int ans = 0;
    sumx = 0;
    while (x) {
        if (key[x] == k) ans = x;
        if (key[x] >= k)
            x = son[x][0];
        else
            sumx += sum[son[x][0]] + 1, x = son[x][1];
    }
    return ans;
}
int del(int x) {
    splay(x);
    int w = son[x][0];
    f[son[x][0]] = f[son[x][1]] = 0;
    if (!w) return root = son[x][1];
    while (son[w][1]) w = son[w][1];
    splay(w);
    son[w][1] = son[x][1];
    if (son[x][1]) f[son[x][1]] = w;
}
int rank(int x, int k) {
    if (k == sum[son[x][0]] + 1) return key[x];
    if (k > sum[son[x][0]] + 1)
        return rank(son[x][1], k - sum[son[x][0]] - 1);
    else
        return rank(son[x][0], k);
}
int pred(int x, int k) {
    int ans = 0;
    while (x) {
        if (key[x] >= k)
            x = son[x][0];
        else
            ans = max(ans, key[x]), x = son[x][1];
    }
    return ans;
}
int succ(int x, int k) {
    int ans = inf;
    while (x) {
        if (key[x] <= k)
            x = son[x][1];
        else
            ans = min(ans, key[x]), x = son[x][0];
    }
    return ans;
}
}  // namespace Splay
// Test
int main() {
    using namespace Splay;
    int n, x, y;
    scanf("%d", &n);
    while (n--) {
        scanf("%d%d", &x, &y);
        // 插入数字y
        if (x == 1) add(root, y);
        // 删除y数(若有多个相同的数，因只删除一个)
        if (x == 2 && find(root, y)) del(find(root, y));
        // 查询x数的排名(若有多个相同的数，输出最小的排名)
        if (x == 3) find(root, y), printf("%d\n", sumx + 1);
        // 查询排名为x的数
        if (x == 4) printf("%d\n", rank(root, y));
        // 求x的前驱(前驱定义为小于x，且最大的数)
        if (x == 5) printf("%d\n", pred(root, y));
        // 求x的后继(后继定义为大于x，且最小的数)
        if (x == 6) printf("%d\n", succ(root, y));
    }
    return 0;
}