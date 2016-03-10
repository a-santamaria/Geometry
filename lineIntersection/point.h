#ifndef POINT_H
#define POINT_H

#include <cmath>


#define EPS 1e-9



class Point{
public:
    double x;
    double y;

    Point() : x(0), y(0) {}
    Point(double _x, double _y) : x(_x), y(_y) {}

    bool operator< (const Point& other) const;
    bool operator> (const Point& other) const;
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
    mutable int id;
    mutable int idp;
    mutable int idq;

    Segment() : idp(-1), idq(-1) {}
    Segment(const Segment& _s);
    Segment(int _idp, int _idq);
    bool equals(const Segment& other) const{
        return idp == other.idp && idq == other.idq;
    }
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
