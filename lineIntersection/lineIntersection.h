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
    Segment segment;
    int tipo;

    static const int INTER = 0;
    static const int START = 1;
    static const int END = -1;

    Event() {}
    Event(Point _key, Segment _segment);
    Event(Point _key, int _tipo);

    bool isFirst();
    bool operator< (const Event& other) const;
};

class LineIntersection{
private:
    std::vector< Segment > segments;
    std::priority_queue< Event > eventQueue;
    void swapOrder(std::set<Segment>& st, Segment first, Segment second,
                   Segment* &prev, Segment* &next);
    bool intersect(const Segment& s, const Segment& t, Point& p);
    void encontrarPrevNext(Segment& s, Segment* &prev, Segment* &next, std::set<Segment>& st);

public:
    LineIntersection();
    LineIntersection(std::vector<Segment> _segments) : segments(_segments) {}
    std::vector<Point> sweep_line();
};

#endif
