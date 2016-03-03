#ifndef POINT_H
#define POINT_H

#include<cmath>


#define EPS 1e-12
#define INF DBL_MAX


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
    mutable double a;
    mutable double b;
    mutable double c;
    double getXInSweepLine() const;

    mutable Point p;
    mutable Point q;

    static double sweep_lineY;

    Segment() : p(0, 0), q(0, 0) {}
    Segment(const Segment& _s){
        a = _s.a;
        b = _s.b;
        c = _s.c;
        p = _s.p;
        q = _s.q;
    }
    Segment(Point _p, Point _q);
    bool operator< (const Segment &other) const;
    bool operator> (const Segment &other) const;
    bool operator== (const Segment &other) const;
    bool operator!= (const Segment &other) const;
    bool equals(const Segment& other) const{
        return p == other.p && q == other.q;
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
