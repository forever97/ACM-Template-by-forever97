/*
    Linear_Rec
    n为矩阵的阶,k为需要计算的数列第k项
    比如An=2*An-1+An-2阶为2
    参数：
        以An=2*An-1+An-2为例
        基础数列first{1,1},转移数列trans{2,1}
    测试正确性: Calc(3)=3,Calc(10007)=71480733 (P=1e9+7)
    O(n*n*log2(k))
*/
struct LinearRec {
    int n;
    const int LOG = 31, P = 1000000007;
    vector<int> first, trans;
    vector<vector<int> > bin;
    vector<int> Add(vector<int> &a, vector<int> &b) {
        vector<int> result(n * 2 + 1, 0);
        for (int i = 0; i <= n; i++) {
            for (int j = 0; j <= n; j++) {
                if ((result[i + j] += 1LL * a[i] * b[j] % P) >= P)
                    result[i + j] -= P;
            }
        }
        for (int i = 2 * n; i > n; i--) {
            for (int j = 0; j < n; j++) {
                if ((result[i - 1 - j] += 1LL * result[i] * trans[j] % P) >= P)
                    result[i - 1 - j] -= P;
            }
            result[i] = 0;
        }
        result.erase(result.begin() + n + 1, result.end());
        return result;
    }
    LinearRec(vector<int> &first, vector<int> &trans)
        : first(first), trans(trans) {
        n = first.size();
        vector<int> a(n + 1, 0);
        a[1] = 1;
        bin.push_back(a);
        for (int i = 1; i < LOG; i++)
            bin.push_back(Add(bin[i - 1], bin[i - 1]));
    }
    int Calc(int k) {
        vector<int> a(n + 1, 0);
        a[0] = 1;
        for (int i = 0; i < LOG; i++) {
            if (k >> i & 1) a = Add(a, bin[i]);
        }
        int ret = 0;
        for (int i = 0; i < n; i++) {
            if ((ret += 1LL * a[i + 1] * first[i] % P) >= P) ret -= P;
        }
        return ret;
    }
};
/*
    Example：
        a[1]=a[2]=…=a[n]=1
        a[i]=a[i-1]+...+a[i-n](i>n)
        求第k项
*/
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    vector<int> a(n, 1);
    LinearRec Rec(a, a);
    printf("%d\n", Rec.Calc(k));
    return 0;
}
