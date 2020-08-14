/*
    筛法求素数
*/
const int M = 3000500;
int p[400010], pNum;
bool f[M];
void Prime() {
    int i, j;
    for (i = 2; i < M; i++) {
        if (!f[i]) {
            p[pNum++] = i;
        }
        for (j = 0; j < pNum && p[j] * i < M; j++) {
            f[p[j] * i] = 1;
            if (!(i % p[j])) break;
        }
    }
}