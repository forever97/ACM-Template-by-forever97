/*
    分数类
*/
#include <bits/stdc++.h>
using namespace std;
const int N = 500;
struct fraction {
    long long numerator;    // 分子
    long long denominator;  // 分母
    fraction() {
        numerator = 0;
        denominator = 1;
    }
    fraction(long long num) {
        numerator = num;
        denominator = 1;
    }
    fraction(long long a, long long b) {
        numerator = a;
        denominator = b;
        this->reduction();
    }
    void operator=(const long long num) {
        numerator = num;
        denominator = 1;
        this->reduction();
    }
    void operator=(const fraction &b) {
        numerator = b.numerator;
        denominator = b.denominator;
        this->reduction();
    }
    fraction operator+(const fraction &b) const {
        long long gcdnum = __gcd(denominator, b.denominator);
        fraction tmp = fraction(numerator * (b.denominator / gcdnum) +
                                    b.numerator * (denominator / gcdnum),
                                denominator / gcdnum * b.denominator);
        tmp.reduction();
        return tmp;
    }
    fraction operator+(const int &b) const { return ((*this) + fraction(b)); }
    fraction operator-(const fraction &b) const {
        return ((*this) + fraction(-b.numerator, b.denominator));
    }
    fraction operator-(const int &b) const { return ((*this) - fraction(b)); }
    fraction operator*(const fraction &b) const {
        fraction tmp =
            fraction(numerator * b.numerator, denominator * b.denominator);
        tmp.reduction();
        return tmp;
    }
    fraction operator*(const int &b) const { return ((*this) * fraction(b)); }
    fraction operator/(const fraction &b) const {
        return ((*this) * fraction(b.denominator, b.numerator));
    }
    void reduction() {
        if (numerator == 0) {
            denominator = 1;
            return;
        }
        long long gcdnum = __gcd(numerator, denominator);
        numerator /= gcdnum;
        denominator /= gcdnum;
    }
    void print() {
        if (denominator == 1)
            printf("%lld\n", numerator);
        else {
            long long num = numerator / denominator;
            long long tmp = num;
            int len = 0;
            while (tmp) {
                len++;
                tmp /= 10;
            }
            for (int i = 0; i < len; i++) printf(" ");
            if (len != 0) printf(" ");
            printf("%lld\n", numerator % denominator);
            if (num != 0) printf("%lld ", num);
            tmp = denominator;
            while (tmp) {
                printf("-");
                tmp /= 10;
            }
            puts("");
            for (int i = 0; i < len; i++) printf(" ");
            if (len != 0) printf(" ");
            printf("%lld\n", denominator);
        }
    }
} f[N];
int n;
/*
    要抽齐n种卡片期望要买的卡片数
*/
int main() {
    while (~scanf("%d", &n)) {
        f[0] = 0;
        for (int i = 1; i <= n; i++) f[i] = f[i - 1] + fraction(1, i);
        f[n] = f[n] * n;
        f[n].print();
    }
    return 0;
}