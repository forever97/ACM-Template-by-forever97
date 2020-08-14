/*
    蔡勒公式
    输入年月日，输出星期几
*/
int Zeller(int y, int m, int d) {
    if (m == 1 || m == 2) m += 12, y--;
    return (d + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
}