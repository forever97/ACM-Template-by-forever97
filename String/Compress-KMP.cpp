/*
    压缩串KMP
    求最长匹配
*/
pair<char, long long> s[200005], t[200005], tmp1, tmp2;
int nxt[200005], n, m, cnt1, cnt2;
long long ans, num;
char c;
void kmp(int n, pair<char, long long>* a, int m, pair<char, long long>* b) {
    int i, j;
    for (nxt[0] = j = -1, i = 1; i < n; nxt[i++] = j) {
        while (~j && a[j + 1] != a[i]) j = nxt[j];
        if (a[j + 1] == a[i]) j++;
    }
    for (j = -1, i = 0; i < m; i++) {
        while (~j && a[j + 1] != b[i]) j = nxt[j];
        if (a[j + 1] == b[i]) j++;
        if (j == n - 1) {
            if (i >= n && b[i + 1].first == tmp2.first &&
                b[i + 1].second >= tmp2.second &&
                b[i - n].first == tmp1.first && b[i - n].second >= tmp1.second)
                ans++;
            j = nxt[j];
        }
    }
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf(" %lld-%c", &num, &c);
        if (s[cnt1].first == c)
            s[cnt1].second += num;
        else {
            s[++cnt1].first = c;
            s[cnt1].second = num;
        }
    }
    for (int i = 1; i <= m; i++) {
        scanf(" %lld-%c", &num, &c);
        if (t[cnt2].first == c)
            t[cnt2].second += num;
        else {
            t[++cnt2].first = c;
            t[cnt2].second = num;
        }
    }
    if (cnt2 >= 3) {
        tmp1 = t[1];
        tmp2 = t[cnt2];
        kmp(cnt2 - 2, t + 2, cnt1, s + 1);
    } else if (cnt2 == 1) {
        for (int i = 1; i <= cnt1; i++)
            if (t[cnt2].first == s[i].first && s[i].second >= t[cnt2].second) {
                ans += (long long)(s[i].second - t[cnt2].second + 1);
            }
    } else if (cnt2 == 2) {
        for (int i = 1; i < cnt1; i++)
            if (t[1].first == s[i].first && t[2].first == s[i + 1].first &&
                t[1].second <= s[i].second && t[2].second <= s[i + 1].second)
                ans++;
    }
    return printf("%lld\n", ans), 0;
}