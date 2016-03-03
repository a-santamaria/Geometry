#ifndef LINE_INTERSECTION_H
#define LINE_INTERSECTION_H

#include <queue>
#include <vector>
#include <list>
#include <set>
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

    bool isFirst();
    bool operator< (const Event& other) const;
};

class LineIntersection{
private:
    std::vector< Segment > segments;
    std::priority_queue< Event > eventQueue;
    void swapOrder(std::set<Segment>& st, Segment first, Segment second,
                   Segment* prev, Segment* next);
    bool intersect(const Segment& s, const Segment& t, Point& p);
    void encontrarPrevNext(Segment& s, Segment* prev, Segment* next, std::set<Segment>& st);

public:
    LineIntersection();
    LineIntersection(std::vector<Segment> _segments) : segments(_segments) {}
    std::vector<Point> sweep_line();
};

#endif
