#ifndef LINE_INTERSECTION_H
#define LINE_INTERSECTION_H

#include <queue>
#include <vector>
#include "point.h"

class compareByY{
public:
    compareByY() {}
    bool operator() (const std::pair<Point, Segment>& p, const std::pair<Point, Segment>& q) const
    {
      if(p.first.y == q.first.y) return p.first.x < q.first.x;
      return p.first.y < q.first.y;
    }
};

class LineIntersection{
private:
    std::vector<Segment> segments;
    std::priority_queue< std::pair<Point, Segment>,
                         std::vector<std::pair<Point, Segment> >,
                         compareByY > eventQueue;
    bool intersect(const Segment s, const Segment t, Point& p);

public:
    LineIntersection();
    LineIntersection(std::vector<Segment> _segments) : segments(_segments) {}
    std::vector<Point> sweep_line();
};

#endif
