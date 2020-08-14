/*
    Trie
    Trick: dfs序为符合前缀的所有串或者符合后缀的所有串
*/
struct Trie {
    int tot, dfn, st[S], en[S], son[S][26], ansL, ansR;
    int Tr(char c) { return c - 'a'; }
    void Init() {
        for (int i = 0; i <= tot; i++)
            for (int j = 0; j < 26; j++) son[i][j] = 0;
        dfn = tot = 0;
    }
    int insert(char *s) {
        int x = 0;
        for (int l = strlen(s), i = 0; i < l; i++) {
            if (!son[x][Tr(s[i])]) son[x][Tr(s[i])] = ++tot;
            x = son[x][Tr(s[i])];
        }
        return x;
    }
    void dfs(int x) {
        st[x] = ++dfn;
        for (int i = 0; i < 26; i++)
            if (son[x][i]) dfs(son[x][i]);
        en[x] = dfn;
    }
    void ask(char *s) {
        int x = 0;
        for (int l = strlen(s), i = 0; i < l; i++) {
            if (!son[x][Tr(s[i])]) {
                ansL = ansR = 0;
                return;
            }
            x = son[x][Tr(s[i])];
        }
        ansL = st[x], ansR = en[x];
    }
};
struct RevTrie {
    int tot, dfn, st[S], en[S], son[S][26], ansL, ansR;
    int Tr(char c) { return c - 'a'; }
    void Init() {
        for (int i = 0; i <= tot; i++)
            for (int j = 0; j < 26; j++) son[i][j] = 0;
        dfn = tot = 0;
    }
    int insert(char *s) {
        int x = 0;
        for (int l = strlen(s), i = l - 1; i >= 0; i--) {
            if (!son[x][Tr(s[i])]) son[x][Tr(s[i])] = ++tot;
            x = son[x][Tr(s[i])];
        }
        return x;
    }
    void dfs(int x) {
        st[x] = ++dfn;
        for (int i = 0; i < 26; i++)
            if (son[x][i]) dfs(son[x][i]);
        en[x] = dfn;
    }
    void ask(char *s) {
        int x = 0;
        for (int l = strlen(s), i = l - 1; i >= 0; i--) {
            if (!son[x][Tr(s[i])]) {
                ansL = ansR = 0;
                return;
            }
            x = son[x][Tr(s[i])];
        }
        ansL = st[x], ansR = en[x];
    }
};