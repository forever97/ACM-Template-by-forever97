// 按字典序生成长度为n以内的Lyndonword
void lyndon_generate(int n, int m) {
    char z = 'a' + m - 1, s[1000];
    s[0] = 'a' - 1;
    for (int i = 1, x = 1;; ++i) {
        s[x - 1]++;
        s[x] = 0;
        puts(s);
        for (int j = x; j < n; ++j) s[j] = s[j - x];
        for (x = n; s[x - 1] == z; --x);
    }
}