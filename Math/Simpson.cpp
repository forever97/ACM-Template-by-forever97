/*
    自适应Simpson
    给定一个函数f(x),求[a,b]区间内f(x)到x轴所形成区域的面积。
*/
double simpson(double l, double r) {
    return (f(l) + f(r) + 4 * f((l + r) / 2.0)) * (r - l) / 6.0;
}
double rsimpson(double l, double r) {
    double mid = (l + r) / 2.0;
    if (fabs(simpson(l, r) - simpson(l, mid) - simpson(mid, r)) < eps)
        return simpson(l, mid) + simpson(mid, r);
    return rsimpson(l, mid) + rsimpson(mid, r);
}