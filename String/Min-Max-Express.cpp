/*
    最小最大表示法
    flag=1：表示最小表示法，flag=0：最大表示法
*/
int Min_Max_Express(char *s, int len, bool flag) {
    int i = 0, j = 1, k = 0;
    while (i < len && j < len && k < len) {
        int t = s[(j + k) % len] - s[(i + k) % len];
        if (t == 0)
            k++;
        else {
            if (flag) {
                if (t > 0)
                    j += k + 1;
                else
                    i += k + 1;
            } else {
                if (t > 0)
                    i += k + 1;
                else
                    j += k + 1;
            }
            if (i == j) j++;
            k = 0;
        }
    }
    return min(i, j);
}
