#ifndef POLYGON_TRINAGULATION_H
#define POLYGON_TRINAGULATION_H

#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cmath>
#include <cfloat>
#include "point.h"
#include "list.h"
#include "list.hxx"

#define INF DBL_MAX

class PolygonTriangulation;

class Event {
public:
    int pId;

    Event();
    Event( int pId );
    bool operator< (const Event& other) const;
};

class PolygonTriangulation {
public:
    //counterclockwise points of polygon
    static std::vector<Point> points;

    static double sweep_lineY;

    struct SegmentComparator {
       double getXInSweepLine(const Segment& s) const;
       bool operator()(const Segment& a, const Segment& b) const;
   };
   typedef std::set<Segment, SegmentComparator>::iterator setSegmentIterator;

private:
    //type of vertices
    enum Type { START, END, REGULAR, MERGE, SPLIT };
    //type of each vertex
    std::vector<Type> typeOfVertices;
    //counterclockwise segments
    std::vector<Segment> edges;
    //counterclockwise doubly-connected cyclic list of edges
    List<int> edgesList;
    //event queue
    std::priority_queue<Event> eventQueue;
    //status tree (ordenado al reves para poder usar upper_bound
                   //que encuentra el izquierdo mas cercano)
    std::set<Segment, SegmentComparator> st;
    // helper <idEdge, idHelperVertex>
    std::map<int, int> helper;
    // map from segment id to iterator in st
    std::map<int, setSegmentIterator> segmentIterators;

public:
    PolygonTriangulation();
    PolygonTriangulation( std::vector<Point> _points );
    void triangulate();

private:
    void handleStart  ( Event& eCurr );
    void handleEnd    ( Event& eCurr );
    void handleSplit  ( Event& eCurr );
    void handleMerge  ( Event& eCurr );
    void handleRegular( Event& eCurr );
    Type typeOfVertex ( int id );
    bool regionToRight ( int id );
};

#endif
