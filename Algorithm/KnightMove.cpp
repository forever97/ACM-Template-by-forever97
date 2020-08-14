/*
    骑士跳跃算法
    只能走日字，求从(0,0)跳到(x,y)所需要的最小步数
*/
int KSP(int x, int y) {
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    if (x < y) swap(x, y);
    if (x == 1 && y == 0) return 3;
    if (x == 2 && y == 2) return 4;
    int d = x - y;
    if (y > d) return 2 * ((y - d + 2) / 3) + d;
    return d - 2 * ((d - y) / 4);
}