#ifndef POINT_H
#define POINT_H

#define eps 1e-9

class Point{
public:
    double x;
    double y;

    Point() : x(0), y(0) {}
    Point(double _x, double _y) : x(_x), y(_y) {}

    bool operator< (const Point& other) const;
    bool operator== (const Point& other) const;
    Point operator- (const Point& other);
    Point operator+ (const Point& other);
};

class Vec{
public:
    double x, y;

    double crossMag(Vec b);
    Vec() : x(0), y(0) {}
    Vec(double _x, double _y) : x(_x), y(_y) {}
};

class Segment{
public:
    Point p;
    Point q;

    Segment() : p(0, 0), q(0, 0) {}
    Segment(Point _p, Point _q) : p(_p), q(_q) {}

    bool operator< (const Segment &other) const;
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

#endif
