/*
    后缀自动机
    时间复杂度O(n)
    cnt：节点数量
    l[x]：节点x的匹配长度
    f[x]：失配指针，f[x]表示的状态为x表示的最小状态的后缀
        l[x]-l[f[x]]即x到f[x]之间非重子串的数量
        比如x:abc f[x]:c，则l[x]-l[f[x]]就得到了串bc和串abc
    r[x]：right集合，x状态的串数量
    数组开串长两倍大小
*/
char s[N];
struct SAM {
    int p, q, np, nq, cnt, lst, a[N][26], l[N], f[N], tot;
    int Tr(char c) { return c - 'a'; }
    SAM() {
        cnt = 0;
        lst = ++cnt;
    }
    // 匹配长度与失配位置的匹配长度之差就是他们之间的子串数量
    int val(int c) { return l[c] - l[f[c]]; }
    void Initialize() {
        memset(l, 0, sizeof(int) * (cnt + 1));
        memset(f, 0, sizeof(int) * (cnt + 1));
        for (int i = 0; i <= cnt; i++)
            for (int j = 0; j < 26; j++) a[i][j] = 0;
        cnt = 0;
        lst = ++cnt;
    }
    /*
        每次extend之后tot表示非重子串的数量
        Trick：
            求解串区间非重子串数量的时候可考虑建立长度个数SAM
            用ans[l][r]保存l为起点extend到r位置时候的tot
    */
    void extend(int c) {
        p = lst;
        np = lst = ++cnt;
        l[np] = l[p] + 1;
        while (!a[p][c] && p) a[p][c] = np, p = f[p];
        if (!p) {
            f[np] = 1;
            tot += val(np);
        } else {
            q = a[p][c];
            if (l[p] + 1 == l[q]) {
                f[np] = q;
                tot += val(np);
            } else {
                nq = ++cnt;
                l[nq] = l[p] + 1;
                memcpy(a[nq], a[q], sizeof(a[q]));
                tot -= val(p) + val(q);
                f[nq] = f[q];
                f[np] = f[q] = nq;
                tot += val(p) + val(q) + val(np) + val(nq);
                while (a[p][c] == q) a[p][c] = nq, p = f[p];
            }
        }
    }
    int b[N], x[N], r[N];
    void build() {
        scanf("%s", s + 1);
        int len = strlen(s + 1);
        for (int i = 1; i <= len; i++) extend(Tr(s[i]));
        /*
            以下部分为基数排序求right集合r[]
            r[i]表示i状态的串数量
            x[i]表示节点编号
        */
        memset(r, 0, sizeof(int) * (cnt + 1));
        memset(b, 0, sizeof(int) * (cnt + 1));
        for (int i = 1; i <= cnt; i++) b[l[i]]++;
        for (int i = 1; i <= len; i++) b[i] += b[i - 1];
        for (int i = 1; i <= cnt; i++) x[b[l[i]]--] = i;
        for (int i = p = 1; i <= len; i++) {
            p = a[p][Tr(s[i])];
            r[p]++;
        }
        for (int i = cnt; i; i--) r[f[x[i]]] += r[x[i]];
    }
    /*
        计算母串中恰好出现k次的子串的数量
    */
    void solve() {
        int ans = 0, k;
        scanf("%d", &k);
        build();
        for (int i = 1; i <= cnt; i++)
            if (r[x[i]] == k) ans += val(x[i]);
        printf("%d\n", ans);
    }
    /*
        求子串在母串中的最长匹配
    */
    void LongestMatch() {
        scanf("%s", s + 1);
        int len = strlen(s + 1);
        int p = 1, ans = 0;
        for (int i = 1; i <= len; i++) {
            int c = Tr(s[i]);
            if (a[p][c])
                p = a[p][c], ans++;
            else
                break;
        }
        printf("%d\n", ans);
    }
    /*
        求子串在母串中圆环匹配成功次数
        (将该子串拆成两段再首尾交换相接的串和母串匹配）
    */
    vector<int> ans;
    bool flag[N];
    void CircleMatch() {
        long long tot = 0;
        scanf("%s", s + 1);
        int len = strlen(s + 1), p = 1, tmp = 0;
        for (int i = 1; i < len; i++) s[i + len] = s[i];
        for (int i = 1; i <= len * 2 - 1; i++) {
            int c = Tr(s[i]);
            if (a[p][c])
                p = a[p][c], tmp++;
            else {
                while (p && !a[p][c]) p = f[p];
                if (!p)
                    p = 1, tmp = 0;
                else
                    tmp = l[p] + 1, p = a[p][c];
            }
            while (l[f[p]] >= len) p = f[p], tmp = l[p];
            if (!flag[p] && tmp >= len) ans.push_back(p), flag[p] = 1;
        }
        for (int i = 0; i < ans.size(); i++) flag[ans[i]] = 0;
        for (int i = 0; i < ans.size(); i++) tot += r[ans[i]];
        ans.clear();
        printf("%I64d\n", tot);
    }
    /*
        F函数：S的所有长度为x的子串中，出现次数的最大值。
    */
    int F[N];
    void CalF() {
        int len = strlen(s + 1);
        for (int i = 1; i <= cnt; i++) F[l[i]] = max(F[l[i]], r[i]);
        for (int i = 1; i <= len; i++) printf("%d\n", F[i]);
    }
    /*
        最小表示法
        [不需要build]
    */
    void MinExp() {
        Initialize();
        scanf("%s", s);
        int n = strlen(s), len = n;
        for (int k = 0; k < 2; k++)
            for (int i = 0; i < n; i++) extend(s[i] - 'a');
        int p = 1;
        while (n--) {
            for (int i = 0; i < 26; i++)
                if (a[p][i]) {
                    p = a[p][i];
                    break;
                }
        }
        printf("%d\n", l[p] - len + 1);
    }
    /*
        计算在母串中除给定字符串之外的非重子串数量
        注意多组数据时u[]的初始化
        u表示每个位置的最长匹配
        AddString：往字符集中加入字符串
        Calu：计算最终答案
    */
    int u[N];
    void AddString() {
        scanf("%s", s + 1);
        int p = 1, len = strlen(s + 1), tmp = 0;
        for (int i = 1; i <= len; i++) {
            int c = s[i] - 'a';
            if (a[p][c])
                p = a[p][c], tmp++, u[p] = max(u[p], tmp);
            else {
                while (p && !a[p][c]) p = f[p];
                if (!p)
                    p = 1, tmp = 0;
                else
                    tmp = l[p] + 1, p = a[p][c], u[p] = max(u[p], tmp);
            }
        }
    }
    void Calu() {
        long long ans = 0;
        for (int i = cnt; i; i--) {
            if (u[x[i]]) {
                u[f[x[i]]] = max(u[x[i]], u[f[x[i]]]);
                if (u[x[i]] < l[x[i]]) ans += l[x[i]] - u[x[i]];
            } else
                ans += val(x[i]);
        }
        printf("%lld\n", ans);
    }
    /*
        逆串SAM构建后缀树
        两点的LCA为两个后缀的最长前缀
        BuildTree+ShowResult可计算出sum(len(T[i])+len(T[j])-2*lcp(T[i],T[j]))
        T表示母串的后缀
    */
    vector<int> v[N];
    void BuildTree() {
        scanf("%s", s + 1);
        int len = strlen(s + 1);
        for (int i = len; i; i--) extend(Tr(s[i]));
        for (int i = 2; i <= cnt; i++) v[f[i]].push_back(i);
    }
    long long res;
    void Dfs(int x, int fx) {
        for (int i = 0; i < v[x].size(); i++) {
            int y = v[x][i];
            Dfs(y, x);
            size[x] += size[y];
        }
        l[x] -= l[fx];
        res = res - (long long)size[x] * (size[x] - 1) * l[x];
    }
    void ShowResult() {
        int len = strlen(s + 1);
        res = (long long)(len - 1) * len * (len + 1) / 2;
        for (int i = 0; i < v[1].size(); i++) Dfs(v[1][i], 1);
        printf("%lld\n", res);
    }
    /*
        多串最长公共子串
        母串build
        mx数组初始化for(int i=1;i<=cnt;i++)mx[i]=l[i];
    */
    void doit() {
        int len = strlen(s + 1), tmp = 0, p = 1;
        static int arr[N];
        for (int i = 1; i <= len; i++) {
            int c = s[i] - 'a';
            if (a[p][c])
                p = a[p][c], tmp++;
            else {
                while (p && !a[p][c]) p = f[p];
                if (!p)
                    p = 1, tmp = 0;
                else
                    tmp = l[p] + 1, p = a[p][c];
            }
            arr[p] = max(arr[p], tmp);
        }
        for (int i = cnt; i; i--) {
            int t = x[i];
            mx[t] = min(mx[t], arr[t]);
            if (arr[t] && f[t]) arr[f[t]] = l[f[t]];
            arr[t] = 0;
        }
    }
    void getans() {
        int ans = 0;
        for (int i = 1; i <= cnt; i++) ans = max(ans, mx[i]);
        printf("%d\n", ans);
    }
    /*
        查询字典序排名第k的子串
    */
    void query(int k) {
        int p = 1;
        while (k) {
            for (int i = 0; i < 26; i++)
                if (a[p][i]) {
                    if (r[a[p][i]] >= k) {
                        putchar('a' + i);
                        p = a[p][i];
                        --k;
                        break;
                    } else
                        k -= r[a[p][i]];
                }
        }
        puts("");
    }
    /*
        两串最长公共子串
    */
    int u[N];
    void solve() {
        scanf("%s", s + 1);
        int p = 1, len = strlen(s + 1), now = 1, tmp = 0, ans = len + 1;
        for (int i = 1; i <= len; i++) {
            int c = s[i] - 'a';
            if (a[p][c])
                p = a[p][c], u[p]++;
            else {
                while (p && !a[p][c]) p = f[p];
                if (!p)
                    p = 1;
                else
                    p = a[p][c], u[p]++;
            }
        }
        for (int i = cnt; i; i--) u[f[x[i]]] += u[x[i]];
        for (int i = 1; i <= cnt; i++)
            if (u[i] == 1 && r[i] == 1) ans = min(ans, l[f[i]] + 1);
        if (ans == len + 1)
            puts("-1");
        else
            printf("%d\n", ans);
    }
}