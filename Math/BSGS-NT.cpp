/*
    BSGS-NT
    求给定模数的原根指标
    BSGS的步长=查询次数*P/步长时为佳
    考虑常数微下调步长
*/
const int P = 1e9 + 7;
namespace NT {
unordered_map<int, int> T;
typedef pair<int, int> PI;
const int K = 300000, G = 5;  // 1e9+7的原根为5
int ind[1010], base, i;
inline int ask(int x) {
    if (x == 1) return 0;
    int t = pow(x, P - 2, P);
    for (int i = K;; i += K) {
        t = 1LL * t * base % P;
        unordered_map<int, int>::iterator it = T.find(t);
        if (it != T.end()) return i - it->second;
    }
}
void init() {
    for (base = 1, i = 0; i < K; i++) {
        T.insert(PI(base, i));
        base = 1LL * base * G % P;
    }
    for (i = 1; i <= 1000; i++) ind[i] = ask(i);
}
}  // namespace NT