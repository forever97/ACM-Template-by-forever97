/*
    小数转化为分数
    0.2...  2/9
    0.20...  1/5
    0.474612399...  1186531/2500000
*/
string s;
int pow(int n) {
    int ret = 1;
    for (int i = 1; i <= n; i++) ret *= 10;
    return ret;
}
int main() {
    while (cin >> s) {
        if (s == "0") break;
        s = s.substr(2, s.size() - 5);
        int n = atoi(s.c_str()), sz = s.size();
        if (n == 0)
            cout << "0/1" << endl;
        else {
            int ansx, ansy = 1000000000;
            for (int i = 1; i <= sz; i++) {
                string xx = s.substr(0, sz - i);
                int x = n - atoi(xx.c_str());
                int y = pow(sz) - pow(sz - i);
                int gcd = __gcd(x, y);
                x /= gcd;
                y /= gcd;
                if (y < ansy) ansx = x, ansy = y;
            }
            cout << ansx << '/' << ansy << endl;
        }
    }
    return 0;
}
