/*
    旋转卡壳
    注意在使用前先求凸包
*/
// 旋转卡壳，求平面最远点对
double rotating_calipers(Point p[], int n) {
    double ans = 0;
    Point v;
    int cur = 1, ans1 = 0, ans2 = 0;
    for (int i = 0; i < n; i++) {
        v = p[i] - p[(i + 1) % n];
        while ((v ^ (p[(cur + 1) % n] - p[cur])) < 0) cur = (cur + 1) % n;
        double tmp = dist(p[i], p[cur]);
        if (tmp > ans) ans = tmp, ans1 = i, ans2 = cur;
        tmp = dist(p[(i + 1) % n], p[(cur + 1) % n]);
        if (tmp > ans) ans = tmp, ans1 = (i + 1) % n, ans2 = (cur + 1) % n;
    }
    printf("%.12f %.12f %.12f %.12f\n", p[ans1].x, p[ans1].y, p[ans2].x,
           p[ans2].y);
    return ans;
}
// 旋转卡壳，求三角形面积最大值的两倍
double rotating_calipers(Point p[], int n) {
    double ans = 0;
    Point v;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int k = (j + 1) % n;
        while (j != i && k != i) {
            ans = max(ans, abs((p[j] - p[i]) ^ (p[k] - p[i])));
            while (((p[i] - p[j]) ^ (p[(k + 1) % n] - p[k])) < 0)
                k = (k + 1) % n;
            j = (j + 1) % n;
        }
    }
    return ans;
}
// 旋转卡壳，求两个凸包的最小距离
// 点p0到线段p1p2的距离
double pointtoseg(Point p0, Point p1, Point p2) {
    return dist(p0, NearestPointToLineSeg(p0, Line(p1, p2)));
}
// 平行线段p0p1和p2p3的距离
double dispallseg(Point p0, Point p1, Point p2, Point p3) {
    double ans1 = min(pointtoseg(p0, p2, p3), pointtoseg(p1, p2, p3));
    double ans2 = min(pointtoseg(p2, p0, p1), pointtoseg(p3, p0, p1));
    return min(ans1, ans2);
}
// 得到向量a1a2和b1b2的位置关系
double Get_angle(Point a1, Point a2, Point b1, Point b2) {
    Point t = b1 - (b2 - a1);
    return (a2 - a1) ^ (t - a1);
}
double rotating_calipers(Point p[], int np, Point q[], int nq) {
    int sp = 0, sq = 0;
    for (int i = 0; i < np; i++)
        if (sgn(p[i].y - p[sp].y) < 0) sp = i;
    for (int i = 0; i < nq; i++)
        if (sgn(q[i].y - q[sq].y) > 0) sq = i;
    double tmp;
    double ans = 1e99;
    for (int i = 0; i < np; i++) {
        while (sgn(tmp = Get_angle(p[sp], p[(sp + 1) % np], q[sq],
                                   q[(sq + 1) % nq])) < 0)
            sq = (sq + 1) % nq;
        if (sgn(tmp) == 0)
            ans = min(ans, dispallseg(p[sp], p[(sp + 1) % np], q[sq],
                                      q[(sq + 1) % nq]));
        else
            ans = min(ans, pointtoseg(q[sq], p[sp], p[(sp + 1) % np]));
        sp = (sp + 1) % np;
    }
    return ans;
}
double solve(Point p[], int n, Point q[], int m) {
    return min(rotating_calipers(p, n, q, m), rotating_calipers(q, m, p, n));
}