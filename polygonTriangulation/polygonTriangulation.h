#ifndef POLYGON_TRINAGULATION_H
#define POLYGON_TRINAGULATION_H

#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <cmath>
#include <cfloat>
#include <iostream>
#include <cstdlib>
#include "point.h"


#define INF DBL_MAX

class PolygonTriangulation;

class Event {
public:
    int pId;

    Event();
    Event( int _pId );
    bool operator< (const Event& other) const;
};

class MonotoneEvent {
public:
    int pId;
    int pos;

    MonotoneEvent();
    MonotoneEvent(int _pId, int _pos);
    bool operator< (const MonotoneEvent& other) const;
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

   std::vector< Segment > newSegments;
   std::vector<Triangle> polySoup;

private:
    //type of vertices
    enum Type { START, END, REGULAR, MERGE, SPLIT };
    //type of each vertex
    std::vector<Type> typeOfVertices;
    //counterclockwise segments
    std::vector<Segment> edges;
    //event queue
    std::priority_queue<Event> eventQueue;
    //status tree (ordenado al reves para poder usar upper_bound
                   //que encuentra el izquierdo mas cercano)
    std::set<Segment, SegmentComparator> st;
    // helper <idEdge, idHelperVertex>
    std::map<int, int> helper;
    // map from segment id to iterator in st
    std::map<int, setSegmentIterator> segmentIterators;
    //counterclockwise doubly-connected edge list (it's a graph)
    std::vector< std::vector<int> > graph;
    //marcas
    std::map< std::pair<int, int>, bool > marcas;
    //y-monotone polygons
    std::vector< std::vector<int> > monotonePolys;


public:
    PolygonTriangulation();
    PolygonTriangulation( std::vector<Point> _points );
    //TODO change name to create edcges something y private
    void triangulate();

private:
    void handleStart  ( Event& eCurr );
    void handleEnd    ( Event& eCurr );
    void handleSplit  ( Event& eCurr );
    void handleMerge  ( Event& eCurr );
    void handleRegular( Event& eCurr );
    Type typeOfVertex ( int id );
    bool regionToRight ( int id );
    void createEdge ( int pId, int idHelper );
    void constructPolygons();
    std::vector<int> getMonotonePoly(int first, int second);
    void trinagulateMonotonePolys();
    void trinagulateMonotonePoly(std::vector<int>& poly);

    void printGraph();
};

#endif
