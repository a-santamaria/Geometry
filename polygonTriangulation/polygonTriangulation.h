#ifndef POLYGON_TRINAGULATION_H
#define POLYGON_TRINAGULATION_H

#include <vector>
#include <queue>
#include "point.h"
#include "list.h"
#include "list.hxx"

class PolygonTriangulation;

class Event {
public:
    int pId;
    const PolygonTriangulation* pt;

    Event();
    Event( int pId, const PolygonTriangulation* _pt );
    bool operator< (const Event& other) const;
};


class PolygonTriangulation {
public:
    //counterclockwise points of polygon
    std::vector<Point> points;
private:
    //counterclockwise edges of polygon
    List<Segment> edges;
    //event queue
    std::priority_queue<Event> eventQueue;

public:
    PolygonTriangulation();
    PolygonTriangulation( std::vector<Point> _points );
    void triangulate();
};

#endif
