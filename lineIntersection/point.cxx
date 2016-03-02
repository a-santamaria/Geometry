#include <algorithm>
#include <iostream>
#include <cfloat>
#include <cmath>
#include <cassert>
#include "point.h"

double Segment::sweep_lineY = INF;

bool Point::operator< (const Point& other) const{
    if(x == other.x)
        return y < other.y;
    return x < other.x;
}

bool Point::operator> (const Point& other) const{
    if(x == other.x)
        return y >= other.y;
    return x > other.x;
}

bool Point::operator== (const Point& other) const{
    if((y == other.y) && (x == other.x))
        return true;
    return false;
}

Point Point::operator- (const Point& other){
    Point p(this->x-other.x, this->y-other.y);
    return p;
}

Point Point::operator+ (const Point& other){
    Point p(this->x+other.x, this->y+other.y);
    return p;
}

Segment::Segment(Point _p, Point _q) : p(_p), q(_q) {

    if(fabs(p.x - q.x) < EPS){//vertical line
        a = 1.0;
        b = 0.0;
        c = -p.x;
    }else{
        a = -(double) (p.y - q.y) / (p.x - q.x);
        b = 1.0;
        c = -(double) (a * p.x) - p.y;
    }
}

bool Segment::operator< (const Segment& other) const{
    assert(Segment::sweep_lineY != INF);
    return this->getXInSweepLine() < other.getXInSweepLine();
}

bool Segment::operator> (const Segment& other) const{
    assert(Segment::sweep_lineY != INF);
    return this->getXInSweepLine() > other.getXInSweepLine();
}

bool Segment::operator== (const Segment &other) const{
    return (p == other.p && q == other.q);
}

double Segment::getXInSweepLine() const{
    return - ( (b*Segment::sweep_lineY) - c ) / a;
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
