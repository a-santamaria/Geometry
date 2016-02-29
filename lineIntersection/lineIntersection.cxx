#include "lineIntersection.h"
#include "redBlackBST.h"
#include <iostream>

using namespace std;

std::vector<Point> LineIntersection::sweep_line(){

    std::vector<Point> intersections;

    for(int i = 0; i < segments.size(); i++){
        eventQueue.push(std::make_pair(segments[i].p, segments[i]));
        eventQueue.push(std::make_pair(segments[i].q, segments[i]));
    }


    return intersections;
}

bool LineIntersection::intersect(const Segment s, const Segment t, Point& p){
    std::cout << "entre" << std::endl;
    if( ccw(s.p, s.q, t.p) == ccw(s.p, s.q, t.q) ||
        ccw(t.p, t.q, s.p) == ccw(t.p, t.q, s.q)    )
        return false;
    //TODO falta colineales y extremos

    double den = toVec(s.p, s.q).crossMag(toVec(t.p, t.q));

    double num = toVec(s.p, t.p).crossMag(toVec(t.p, t.q)) * den;


    double ss = num / (den*den);

    std::cout<<"s "<<ss<<std::endl;
    p.x = (s.q.x - s.p.x);
    p.y = (s.q.y - s.p.y);
    p.x = p.x*ss;
    p.y = p.y*ss;
    p = p + s.p;
    return true;
}
