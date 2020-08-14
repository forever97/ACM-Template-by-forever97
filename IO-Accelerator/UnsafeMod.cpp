/*
    用于模为奇数且取模次数特别多的情况
    UnsafeMod::set_mod(P) 初始化模数
    x.get() 得到值
    各种运算后取模直接运算即可
*/
#include <bits/stdc++.h>
using namespace std;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef uint32 word;
typedef uint64 dword;
typedef int sword;
word mod;
struct UnsafeMod {
    word x;
    UnsafeMod() : x(0) {}
    UnsafeMod(word _x) : x(init(_x)) {}
    UnsafeMod& operator+=(const UnsafeMod& rhs) {
        (x += rhs.x) >= mod && (x -= mod);
        return *this;
    }
    UnsafeMod& operator-=(const UnsafeMod& rhs) {
        sword(x -= rhs.x) < 0 && (x += mod);
        return *this;
    }
    UnsafeMod& operator*=(const UnsafeMod& rhs) {
        x = reduce(dword(x) * rhs.x);
        return *this;
    }
    UnsafeMod operator+(const UnsafeMod& rhs) const { return UnsafeMod(*this) += rhs; }
    UnsafeMod operator-(const UnsafeMod& rhs) const { return UnsafeMod(*this) -= rhs; }
    UnsafeMod operator*(const UnsafeMod& rhs) const { return UnsafeMod(*this) *= rhs; }
    word get() const { return reduce(x); }
    static word modulus() { return mod; }
    static word init(word w) { return reduce(dword(w) * r2); }
    static void set_mod(word m) {
        mod = m;
        Modinv = mul_inv(mod);
        r2 = -dword(mod) % mod;
    }
    static word reduce(dword x) {
        word y = word(x >> word_bits) - word((dword(word(x) * Modinv) * mod) >> word_bits);
        return sword(y) < 0 ? y + mod : y;
    }
    static word mul_inv(word n, int e = 6, word x = 1) { return !e ? x : mul_inv(n, e - 1, x * (2 - x * n)); }
}x,y;

