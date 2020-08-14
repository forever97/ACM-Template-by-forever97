/*
    快速乘法
*/
// O(1)
LL mul(LL x, LL y, LL mod) {
    return (x * y - (LL)(x / (long double)mod * y + 1e-3) * mod + mod) % mod;
}
// O(logn)
LL mul(LL x, LL y, LL mod) {
    LL ret = 0;
    for (; y; y >>= 1) {
        if (y & 1) ret = (ret + x) % mod;
        x = (x + x) % mod;
    }
    return ret;
}