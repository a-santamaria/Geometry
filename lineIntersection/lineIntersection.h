#ifndef LINE_INTERSECTION_H
#define LINE_INTERSECTION_H

#include <queue>
#include <vector>
#include <list>
#include "point.h"

class Event{
public:
    Point key;
    Segment* s;
    std::list<Segment*> segments;
    bool hasPrev;
    std::list<Point> prevKey;

    Event() {}
    Event(Point _key) : key(_key), hasPrev(false) {
    }
    Event(Point _key, Segment* _s) : key(_key), s(_s), hasPrev(false) {
        segments.push_back(_s);
    }
    void addSegment(Segment* s, Point prev){
        hasPrev = true;
        segments.push_back(s);
        prevKey.push_back(prev);
    }

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
