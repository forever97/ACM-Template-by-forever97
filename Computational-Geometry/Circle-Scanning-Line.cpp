/*
    圆上扫描线
    Example：给出若干个圆，可以互相嵌套但不相交或相切。
    每次删去一个圆和它内部的圆，进行博弈，问谁赢。
    Solution: 我们将最外面一层空间看做一个大圆，则这个游戏等价于树上删边游戏，
    则只要处理圆上关系即可，我们用圆上扫描线来处理。
    (注意只能处理包含和相离的情况)
*/
#include <algorithm>
#include <cstdio>
#include <set>
#include <vector>
using namespace std;
const int N = 100010;
vector<int> v[N];
namespace Cir_SCLine {
const int U = 0, D = 1, L = 0, R = 1;
int Time, n;
struct Circle {
    int x, y, r, w, f;
    void read() {
        w = 0;
        scanf("%d%d%d", &x, &y, &r);
    }
    int getX(int dir) { return dir == L ? x - r : x + r; }
    double getY(int dir) {
        double res = sqrt((double)r * r - (double)(Time - x) * (Time - x));
        return dir == U ? res + y : -res + y;
    }
} cir[N];
struct Event {
    int x, y, id, dir;
    void get(int _x, int _y, int _id, int _dir) {
        x = _x;
        y = _y;
        id = _id;
        dir = _dir;
    };
    bool operator<(const Event rhs) const {
        return x < rhs.x || x == rhs.x && y > rhs.y;
    }
} et[N << 1];
struct node {
    int id, dir;
    node() {}
    node(int _id, int _dir) {
        id = _id;
        dir = _dir;
    }
    bool operator<(const node rhs) const {
        double y1 = cir[id].getY(dir);
        double y2 = cir[rhs.id].getY(rhs.dir);
        return y1 > y2 || y1 == y2 && dir < rhs.dir;
    }
};
set<node> line;
set<node>::iterator it, it1, it2;
int etn;
void SCLine() {
    line.clear();
    for (int i = 0; i < etn; i++) {
        Time = et[i].x;
        if (et[i].dir == R) {
            line.erase(node(et[i].id, U));
            line.erase(node(et[i].id, D));
        } else {
            it = line.insert(node(et[i].id, U)).first;
            it1 = it2 = it;
            it1++;
            int id = it->id;
            // 当该圆上的点在扫描线边界时，为一层圆
            if (it == line.begin() || it1 == line.end())
                cir[id].w = 1;
            else {
                it2--;
                if (it2->id == it1->id) {
                    // 当扫描线上在当前圆上方和下方的点属于同个圆时，被该圆包含
                    cir[id].w = cir[it2->id].w + 1;
                    cir[id].f = it2->id;
                    v[it2->id].push_back(id);
                } else {
                    // 当上下方的点不属于同个圆时
                    cir[id].w = max(cir[it1->id].w, cir[it2->id].w);
                    if (cir[it1->id].w == cir[it2->id].w) {
                        //  当上下方圆层次相同，则两个圆是该圆的同级圆，被同个圆包含
                        cir[id].f = cir[it2->id].f;
                        v[cir[it2->id].f].push_back(id);
                    } else {
                        //  当上下方圆层次不同，被层次较小的圆包含，较大的是同级圆
                        int f = it1->id;
                        if (cir[it1->id].w > cir[it2->id].w) f = it2->id;
                        cir[id].f = f;
                        v[f].push_back(id);
                    }
                }
            }
            line.insert(node(et[i].id, D));
        }
    }
}
}  // namespace Cir_SCLine
using namespace Cir_SCLine;
int val[N];
int SG(int x) {
    int sg = 0;
    for (int i = 0; i < v[x].size(); i++) sg ^= SG(v[x][i]);
    val[x] += sg;
    return val[x];
}
int T;
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 0; i <= n; i++) v[i].clear();
        etn = 0;
        cir[0].x = cir[0].y = 0;
        cir[0].r = 100000;
        et[etn++].get(cir[0].getX(L), cir[0].y, 0, L);
        et[etn++].get(cir[0].getX(R), cir[0].y, 0, R);
        for (int i = 1; i <= n; i++) {
            cir[i].read();
            et[etn++].get(cir[i].getX(L), cir[i].y, i, L);
            et[etn++].get(cir[i].getX(R), cir[i].y, i, R);
        }
        sort(et, et + etn);
        SCLine();
        val[0] = 0;
        for (int i = 1; i <= n; i++) val[i] = 1;
        puts(SG(0) ? "Alice" : "Bob");
    }
    return 0;
}
