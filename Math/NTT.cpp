/*
    NTT
    总初始化：NTT::GetWn();
    while(len<n<<1)len<<=1;
    for(i=n;i<len;i++)A[i]=B[i]=0;
    NTT::NTT(A,len,1);
    NTT::NTT(B,len,1);
    for(i=0;i<len;i++)A[i]=NTT::mul(A[i],B[i],P);
    NTT::NTT(A,len,-1);
    要求P为费马素数
    root[998244353]=3
    root[1e9+7]=5
*/
namespace NTT {
const int G = 3;
const int NUM = 20;
int wn[20];
int mul(int x, int y, int P) { return (LL)x * y % P; }
int PowMod(int a, int b) {
    int res = 1;
    a %= P;
    while (b) {
        if (b & 1) res = mul(res, a, P);
        a = mul(a, a, P);
        b >>= 1;
    }
    return res;
}
void GetWn() {
    for (int i = 0; i < NUM; i++) {
        int t = 1 << i;
        wn[i] = PowMod(G, (P - 1) / t);
    }
}
void Change(int a[], int len) {
    int i, j, k;
    for (i = 1, j = len / 2; i < len - 1; i++) {
        if (i < j) swap(a[i], a[j]);
        k = len / 2;
        while (j >= k) {
            j -= k;
            k /= 2;
        }
        if (j < k) j += k;
    }
}
void NTT(int a[], int len, int on) {
    Change(a, len);
    int id = 0;
    for (int h = 2; h <= len; h <<= 1) {
        id++;
        for (int j = 0; j < len; j += h) {
            int w = 1;
            for (int k = j; k < j + h / 2; k++) {
                int u = a[k] % P;
                int t = mul(a[k + h / 2], w, P);
                a[k] = (u + t) % P;
                a[k + h / 2] = ((u - t) % P + P) % P;
                w = mul(w, wn[id], P);
            }
        }
    }
    if (on == -1) {
        for (int i = 1; i < len / 2; i++) swap(a[i], a[len - i]);
        int inv = PowMod(len, P - 2);
        for (int i = 0; i < len; i++) a[i] = mul(a[i], inv, P);
    }
}
}  // namespace NTT
