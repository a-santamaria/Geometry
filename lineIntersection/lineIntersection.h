#ifndef LINE_INTERSECTION_H
#define LINE_INTERSECTION_H

#include <queue>
#include "point.h"

class LineIntersection{
private:
    std::vector<Segment> segments;
    std::priority_queue< std::pair<Point, Segment> > eventQueue;
    bool intersect(const Segment s, const Segment t, Point& p);

public:
    LineIntersection();
    LineIntersection(std::vector<Segment> _segments) : segments(_segments) {}
    std::vector<Point> sweep_line();
};

#endif
