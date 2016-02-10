#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include <vector>

using namespace std;


class Point{
public:
    double x, y;

    Point() { x = 0; y = 0; }
    Point(double _x, double _y): x (_x), y (_y){}

    bool operator< (const Point &other) const;
};

class Vec{
public:
    double x, y;

    double cross(Vec b);
    Vec() { x = 0; y = 0; }
    Vec(double _x, double _y): x (_x), y (_y){}
};

double randF();
void randEllipse( double r1, double r2, double cx, double cy, double d,
                  double& x, double& y);
Vec toVec(Point p, Point q);
bool ccw(Point p, Point q, Point r);
bool collinear(Point p, Point q, Point r);

struct CompByAngleStruct {
    Point piv;
    bool operator() (const Point& p, const Point& q);
    CompByAngleStruct();
    CompByAngleStruct(Point pivot_): piv(pivot_) {}
};

class ConvexHull{
private:
    vector<Point> points;
    vector<Point> divideAndConquer(int lo, int hi);
    vector<Point> mergeTangent(vector<Point>& lh, vector<Point>& rh);

public:
    ConvexHull();
    ConvexHull(vector<Point> _points): points(_points) {}
    vector<Point> incremental();
    vector<Point> divideAndConquer();
    vector<Point> naive();
};

#endif
