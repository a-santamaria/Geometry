#ifndef LINE_INTERSECTION_H
#define LINE_INTERSECTION_H

#include <queue>
#include <vector>
#include "point.h"

class Event{
public:
    Point key;
    Segment* s;
    bool hasPrev;
    Point prevKey;

    Event() {}
    Event(Point _key, Segment* _s, Point _prevKey) :
          key(_key), s(_s), hasPrev(true), prevKey(_prevKey) {}
    Event(Point _key, Segment* _s) : key(_key), s(_s), hasPrev(false) {}
    bool operator< (const Event& other) const{
        if(key.y == other.key.y) return key.x < other.key.x;
        return key.y < other.key.y;
    }
};

class LineIntersection{
private:
    std::vector<Segment*> segments;
    std::priority_queue< Event > eventQueue;
    bool intersect(const Segment* s, const Segment* t, Point& p);

public:
    LineIntersection();
    LineIntersection(std::vector<Segment*> _segments) : segments(_segments) {}
    std::vector<Point> sweep_line();
};

#endif
