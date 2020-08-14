/*
    矩阵乘法
*/
typedef long long LL;
LL P = 100000LL;
struct mat {
    int n;
    LL num[110][110];
    void init0(int t) {
        n = t;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) num[i][j] = 0;
    }
    void init1(int t) {
        n = t;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (i != j)
                    num[i][j] = 0;
                else
                    num[i][j] = 1;
    }
    mat operator=(const struct mat p) {
        n = p.n;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) num[i][j] = p.num[i][j];
    }
    mat operator*(const struct mat p) const {
        struct mat ans;
        ans.init0(n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    ans.num[i][j] =
                        (ans.num[i][j] + num[i][k] * p.num[k][j]) % P;
        return ans;
    }
    mat operator^(int t) const {
        struct mat ans, now;
        ans.init1(n);
        now.n = n;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) now.num[i][j] = num[i][j];
        while (t > 0) {
            if (t & 1) ans = ans * now;
            now = now * now;
            t >>= 1;
        }
        return ans;
    }
}