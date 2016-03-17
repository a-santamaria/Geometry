#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <ostream>


#define EPS 1e-12



class Point{
public:
    double x;
    double y;

    Point() : x(0), y(0) {}
    Point(double _x, double _y) : x(_x), y(_y) {}

    bool operator< (const Point& other) const;
    friend std::ostream& operator<<(std::ostream&, const Point& p);
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
    int idp;
    int idq;

    Segment() : idp(-1), idq(-1) {}
    Segment(const Segment& _s);
    Segment(int _idp, int _idq);

    friend std::ostream& operator<<(std::ostream&, const Segment& s);
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
