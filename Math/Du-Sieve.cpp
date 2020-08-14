/* 
    杜教筛
    g(1)S(n)=\sum_{i=1}^{n}h(i)−\sum_{d=2}^{n}g(d)S(n/d)
*/
/*
    Example1: 
        莫比乌斯函数区段和
*/
const int mod = 1333331;
typedef long long LL;
LL a, b, miu[5000010];
int p[500010], cnt = 0;
bool vis[5000010];
struct HASHMAP {
    int h[mod + 10], cnt, nxt[100010];
    LL st[100010], S[100010];
    void push(LL k, LL v) {
        int key = k % mod;
        for (int i = h[key]; i; i = nxt[i]) {
            if (S[i] == k) return;
        }
        ++cnt;
        nxt[cnt] = h[key];
        h[key] = cnt;
        S[cnt] = k;
        st[cnt] = v;
    }
    LL ask(LL k) {
        int key = k % mod;
        for (int i = h[key]; i; i = nxt[i]) {
            if (S[i] == k) return st[i];
        }
        return -1;
    }
} H;
void Get_Prime() {
    miu[1] = 1;
    for (int i = 2; i <= 5000000; ++i) {
        if (!vis[i]) {
            p[++cnt] = i;
            miu[i] = -1;
        }
        for (int j = 1; j <= cnt; ++j) {
            if (1LL * p[j] * i > 5000000) break;
            int ip = i * p[j];
            vis[ip] = true;
            if (i % p[j] == 0) break;
            miu[ip] = -miu[i];
        }
    }
    for (int i = 2; i <= 5000000; ++i) miu[i] += miu[i - 1];
}
LL miu_sum(LL n) {
    if (n <= 5000000) return miu[n];
    LL tmp = H.ask(n), la, A = 1;
    if (tmp != -1) return tmp;
    for (LL i = 2; i <= n; i = la + 1) {
        LL now = n / i;
        la = n / now;
        A = A - (la - i + 1) * miu_sum(n / i);
    }
    H.push(n, A);
    return A;
}
int main() {
    scanf("%lld%lld", &a, &b);
    Get_Prime();
    printf("%lld\n", miu_sum(b) - miu_sum(a - 1));
    return 0;
}
/*
    Example2:
        欧拉函数前缀和
*/
typedef long long LL;
const int mod = 1333331, inv2 = 500000004;
const LL MOD = 1e9 + 7;
LL a, b, miu[5000010], phi[5000010];
int p[500010], cnt = 0, i, tot;
bool v[5000010];
struct HASHMAP {
    int h[mod + 10], cnt, nxt[100010];
    LL st[100010], S[100010];
    void push(LL k, LL v) {
        int key = k % mod;
        for (int i = h[key]; i; i = nxt[i]) {
            if (S[i] == k) return;
        }
        ++cnt;
        nxt[cnt] = h[key];
        h[key] = cnt;
        S[cnt] = k;
        st[cnt] = v;
    }
    LL ask(LL k) {
        int key = k % mod;
        for (int i = h[key]; i; i = nxt[i]) {
            if (S[i] == k) return st[i];
        }
        return -1;
    }
} H;
void Get_Prime() {
    for (miu[1] = phi[1] = 1, i = 2; i <= 5000000; i++) {
        if (!v[i]) p[tot++] = i, miu[i] = -1, phi[i] = i - 1;
        for (int j = 0; i * p[j] <= 5000000 && j < tot; j++) {
            v[i * p[j]] = 1;
            if (i % p[j]) {
                miu[i * p[j]] = -miu[i];
                phi[i * p[j]] = phi[i] * (p[j] - 1);
            } else {
                miu[i * p[j]] = 0;
                phi[i * p[j]] = phi[i] * p[j];
                break;
            }
        }
    }
    for (int i = 2; i <= 5000000; ++i) phi[i] = (phi[i - 1] + phi[i]) % MOD;
}
LL phi_sum(LL n) {
    if (n <= 5000000) return phi[n];
    LL tmp = H.ask(n), la, A = 0;
    if (tmp != -1) return tmp;
    for (LL i = 2; i <= n; i = la + 1) {
        LL now = n / i;
        la = n / now;
        (A += (la - i + 1) % MOD * phi_sum(n / i) % MOD) %= MOD;
    }
    A = ((n % MOD) * (n % MOD + 1) % MOD * inv2 % MOD - A + MOD) % MOD;
    H.push(n, A);
    return A;
}
int main() {
    scanf("%lld", &a);
    Get_Prime();
    printf("%lld\n", phi_sum(a));
    return 0;
}
/*
    Example3：
        求[1,n][1,n]最大公约数之和
        枚举最大公约数k，答案为2*∑(k*phi_sum(n/k))-n*(n+1)/2
        phi_sum利用杜教筛实现
*/
typedef long long LL;
const int mod = 1333331, inv2 = 500000004;
const LL MOD = 1e9 + 7;
LL a, b, n, miu[5000010], phi[5000010];
int p[500010], cnt = 0, i, tot;
bool v[5000010];
struct HASHMAP {
    int h[mod + 10], cnt, nxt[100010];
    LL st[100010], S[100010];
    void push(LL k, LL v) {
        int key = k % mod;
        for (int i = h[key]; i; i = nxt[i]) {
            if (S[i] == k) return;
        }
        ++cnt;
        nxt[cnt] = h[key];
        h[key] = cnt;
        S[cnt] = k;
        st[cnt] = v;
    }
    LL ask(LL k) {
        int key = k % mod;
        for (int i = h[key]; i; i = nxt[i]) {
            if (S[i] == k) return st[i];
        }
        return -1;
    }
} H;
void Get_Prime() {
    for (miu[1] = phi[1] = 1, i = 2; i <= 5000000; i++) {
        if (!v[i]) p[tot++] = i, miu[i] = -1, phi[i] = i - 1;
        for (int j = 0; i * p[j] <= 5000000 && j < tot; j++) {
            v[i * p[j]] = 1;
            if (i % p[j]) {
                miu[i * p[j]] = -miu[i];
                phi[i * p[j]] = phi[i] * (p[j] - 1);
            } else {
                miu[i * p[j]] = 0;
                phi[i * p[j]] = phi[i] * p[j];
                break;
            }
        }
    }
    for (int i = 2; i <= 5000000; ++i) phi[i] = (phi[i - 1] + phi[i]) % MOD;
}
LL phi_sum(LL n) {
    if (n <= 5000000) return phi[n];
    LL tmp = H.ask(n), la, A = 0;
    if (tmp != -1) return tmp;
    for (LL i = 2; i <= n; i = la + 1) {
        LL now = n / i;
        la = n / now;
        (A += (la - i + 1) % MOD * phi_sum(n / i) % MOD) %= MOD;
    }
    A = ((n % MOD) * (n % MOD + 1) % MOD * inv2 % MOD - A + MOD) % MOD;
    H.push(n, A);
    return A;
}
int main() {
    scanf("%lld", &n);
    Get_Prime();
    LL la, ans = 0;
    for (LL i = 1; i <= n; i = la + 1) {
        LL now = n / i;
        la = n / now;
        ans = (ans + (i + la) % MOD * (la - i + 1) % MOD * inv2 % MOD *
                         phi_sum(now) % MOD) %
              MOD;
    }
    ans = ans * 2 % MOD;
    LL k = n % MOD;
    ans = (ans - k * (k + 1) % MOD * inv2 % MOD + MOD) % MOD;
    printf("%lld\n", ans);
    return 0;
}
/*
    Example4：
        求[1,n][1,m]两两LCM的和
*/
namespace Sum_LCM {
int tot, p[N], miu[N], v[N];
LL sum[N];
void mobius(int n) {
    int i, j;
    for (miu[1] = 1, i = 2; i <= n; i++) {
        if (!v[i]) p[tot++] = i, miu[i] = -1;
        for (j = 0; j < tot && i * p[j] <= n; j++) {
            v[i * p[j]] = 1;
            if (i % p[j])
                miu[i * p[j]] = -miu[i];
            else
                break;
        }
    }
    for (i = 1; i <= n; i++)
        sum[i] = (sum[i - 1] + (1LL * miu[i] * i % mod * i % mod)) % mod;
}
LL Sum(LL x, LL y) {
    return (((x * (x + 1) / 2) % mod) * ((y * (y + 1) / 2) % mod) % mod);
}
LL F(int n, int m) {
    LL t = 0;
    if (n > m) swap(n, m);
    for (int i = 1, j = 0; i <= n; i = j + 1) {
        j = min(n / (n / i), m / (m / i));
        t = (t + (sum[j] - sum[i - 1] + mod) % mod * Sum((LL)n / i, (LL)m / i) %
                     mod) %
            mod;
    }
    return t;
}
LL Cal_Sum(int n, int m) {
    if (n > m) swap(n, m);
    mobius(n);
    LL ans = 0;
    for (int i = 1, j = 0; i <= n; i = j + 1) {
        j = min(n / (n / i), m / (m / i));
        ans = (ans +
               1LL * (i + j) * (j - i + 1) / 2 % mod * F(n / i, m / i) % mod) %
              mod;
    }
    return ans;
}
}  // namespace Sum_LCM