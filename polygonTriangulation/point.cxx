#include <algorithm>
#include <iostream>
#include <cfloat>
#include <cmath>
#include <cassert>
#include "point.h"

bool Point::operator< (const Point& other) const{
    if(y == other.y)
        return x > other.x;
    return y < other.y;
}

std::ostream& operator<<(std::ostream &strm, const Point &p) {
    return strm << "(" << p.x << " " << p.y << ")";
}

Segment::Segment(int _idp, int _idq) : idp(_idp), idq(_idq) { }

Segment::Segment(int _idp, int _idq, int _id) : idp(_idp), idq(_idq), id(_id) {}

Segment::Segment(const Segment& _s) {
    id = _s.id;
    idp = _s.idp;
    idq = _s.idq;
}

Triangle::Triangle(int p, int q, int r) {
    idPoints[0] = p;
    idPoints[1] = q;
    idPoints[2] = r;
}

std::ostream& operator<<(std::ostream &strm, const Segment &s) {
    return strm << "(" << s.idp << " " << s.idq << ")";
}


double randF(){
    return ((double)rand() / RAND_MAX);
}

void randEllipse( double r1, double r2, double cx, double cy, double d,
                  double& x, double& y)
{
    double rx = randF()*r1;
    double ry = randF()*r2;
    double theta = randF() * 2 * M_PI;
    double a = sqrt(rx);
    double b = sqrt(ry);
    double alpha = d * M_PI;
    x = cx + ( a * cos(theta) * cos(alpha) ) - ( b * sin(theta) * sin(alpha) );
    y = cy + ( a * cos(theta) * sin(alpha) ) + ( b * sin(theta) * cos(alpha) );
}

double Vec::crossMag(Vec b){
    return this->x * b.y - this->y * b.x;
}

Vec toVec(Point p, Point q){
    return Vec(q.x - p.x, q.y - p.y);
}

bool ccw(Point p, Point q, Point r){
    return toVec(q, p).crossMag( toVec(q, r) ) < 0;
}

double getAngle(Point p, Point q, Point r){
    Vec qp =toVec(q, p);
    Vec qr = toVec(q, r);
    double n1 = sqrt( (qp.x*qp.x) + (qp.y*qp.y) );
    double n2 = sqrt( (qr.x*qr.x) + (qr.y*qr.y) );
    double dot = (qp.x * qr.x) + (qp.y * qr.y);
    double alpah = acos( dot / (n1*n2) );

    return alpah;
}

bool collinear(Point p, Point q, Point r){
    return toVec(q, p).crossMag( toVec(q, r) ) == 0;
}

double dist(Point p, Point q){
    double x = q.x-p.x;
    double y = q.y-p.y;
    return sqrt(x*x + y*y);
}

bool CompByAngleStruct::operator() (const Point& p, const Point& q){

    if(collinear(piv, p, q)){
        return dist(piv, p) < dist(piv, q);
    }

    return ccw(p, q, piv);
}
