/*
    可持久化Trie
    支持查询区间内与某个数异或最大值
    trick：当求区间和异或最大的时候，我们可以求前缀异或和，
    这样子又转化回选择一个已插入的数使得其与当前值异或值最大
    Presum[R]^Presum[L-1]=Xorsum[L~R]
    那么插入R的同时查询与之前的Xor最大值即可，复杂度O(len*n)
*/
const int N = 600010;
int bin[32], root[N];
bin[0] = 1;
struct trie {
    int cnt, ch[N * 32][2], sum[N * 32];
    void Initialize() {
        memset(ch, 0, sizeof(ch));
        memset(sum, 0, sizeof(sum));
        cnt = 0;
    }
    int insert(int x, int val) {
        int tmp, y;
        tmp = y = ++cnt;
        for (int i = 31; i >= 0; i--) {
            int t = val & bin[i];
            t >>= i;
            ch[y][0] = ch[x][0];
            ch[y][1] = ch[x][1];
            x = ch[x][t];
            y = ch[y][t] = ++cnt;
            sum[y] = sum[x] + 1;
        }
        return tmp;
    }
    int query(int l, int r, int val) {
        int tmp = 0;
        for (int i = 31; i >= 0; i--) {
            int t = val & bin[i];
            t >>= i;
            if (sum[ch[r][t ^ 1]] - sum[ch[l][t ^ 1]])
                tmp += bin[i], r = ch[r][t ^ 1], l = ch[l][t ^ 1];
            else
                r = ch[r][t], l = ch[l][t];
        }
        return tmp;
    }
} Trie;
// Test
int main() {
    for (int i = 1; i < 32; i++) bin[i] = bin[i - 1] << 1;
    for (int Cas = 1; Cas <= T; Cas++) {
        Trie.Initialize();
        // 插入数据
        for (int i = 1; i <= n; i++) root[i] = trie.insert(root[i - 1], a[i]);
        // 查询 [L,R] 中与x异或值最大的数
        int ans=trie.query(root[L-1],root[R],x));
    }
}