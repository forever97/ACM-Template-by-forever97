/*
    ZKW费用流
*/
const int maxint = ~0U >> 1;
int n, m, pi1, cost = 0;
bool v[550];
struct etype {
    int t, c, u;
    etype *next, *pair;
    etype() {}
    etype(int t_, int c_, int u_, etype *next_)
        : t(t_), c(c_), u(u_), next(next_) {}
    void *operator new(unsigned, void *p) { return p; }
} * e[550];
int aug(int no, int m) {
    if (no == n) return cost += pi1 * m, m;
    v[no] = true;
    int l = m;
    for (etype *i = e[no]; i; i = i->next)
        if (i->u && !i->c && !v[i->t]) {
            int d = aug(i->t, l < i->u ? l : i->u);
            i->u -= d, i->pair->u += d, l -= d;
            if (!l) return m;
        }
    return m - l;
}
bool modlabel() {
    int d = maxint;
    for (int i = 1; i <= n; ++i)
        if (v[i])
            for (etype *j = e[i]; j; j = j->next)
                if (j->u && !v[j->t] && j->c < d) d = j->c;
    if (d == maxint) return false;
    for (int i = 1; i <= n; ++i)
        if (v[i])
            for (etype *j = e[i]; j; j = j->next) j->c -= d, j->pair->c += d;
    pi1 += d;
    return true;
}
int main() {
    scanf("%d %d", &n, &m);
    etype *Pe = new etype[m + m];
    while (m--) {
        int s, t, c, u;
        scanf("%d%d%d%d", &s, &t, &u, &c);
        e[s] = new (Pe++) etype(t, c, u, e[s]);
        e[t] = new (Pe++) etype(s, -c, 0, e[t]);
        e[s]->pair = e[t];
        e[t]->pair = e[s];
    }
    do
        do
            memset(v, 0, sizeof(v));
        while (aug(1, maxint));
    while (modlabel());
    printf("%d\n", cost);
    return 0;
}