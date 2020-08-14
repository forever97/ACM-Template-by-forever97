/*
    约瑟夫问题
*/
int Josephus(int n, int k) {  // Number(0-n-1)
    if (k == 1) return n - 1;
    if (n == 1) return 0;
    int ret;
    if (n < k) {
        ret = 0;
        for (int i = 2; i <= n; i++) ret = (ret + k) % i;
        return ret;
    }
    ret = Josephus(n - n / k, k);
    if (ret < n % k)
        ret = ret - n % k + n;
    else
        ret = ret - n % k + (ret - n % k) / (k - 1);
    return ret;
}