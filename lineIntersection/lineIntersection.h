#ifndef LINE_INTERSECTION_H
#define LINE_INTERSECTION_H

#include <queue>
#include <vector>
#include <list>
#include "point.h"

class Event{
public:
    Point key;
    Segment segments[2];
    bool isIntersection;
    std::list<Point> prevKey;

    Event() {}
    Event(Point _key, Segment _s);
    Event(Point _key, Segment _s, Segment _t);

    bool operator< (const Event& other) const;
};

class LineIntersection{
private:
    std::vector< Segment > segments;
    std::priority_queue< Event > eventQueue;
    bool intersect(const Segment& s, const Segment& t, Point& p);

public:
    LineIntersection();
    LineIntersection(std::vector<Segment> _segments) : segments(_segments) {}
    std::vector<Point> sweep_line();
};

#endif
