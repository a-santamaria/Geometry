#ifndef __VORONOIFILTER__H__
#define __VORONOIFILTER__H__

#include <vtkPolyDataAlgorithm.h>

#include "point.h"
#include <set>
#include <queue>
#include <map>
#include <vector>

struct Event;
typedef std::set<Event>::iterator eventIterator;

struct Arc {
    std::pair<Point, Point> sites;
    static double sweep_lineY;
    mutable bool hasCircleEvent;
    mutable eventIterator itCircle;
    int idEdge;
    Arc();
    Arc(Point first, Point second, int _idEdge);
    bool operator< (const Arc& other) const;
    Point getBreakpoint() const;

};

struct Event {
    Point p;
    bool isSite;

    std::set<Arc>::iterator leftArc;
    std::set<Arc>::iterator rightArc;
    Point center;

    Event();
    Event(Point _p);
    Event( Point _p,
           std::set<Arc>::iterator _leftArc,
           std::set<Arc>::iterator _rightArc,
           Point _center );
    bool operator< (const Event& other) const;
};

class VoronoiFilter : public vtkPolyDataAlgorithm {
public:
    vtkTypeMacro( VoronoiFilter,vtkPolyDataAlgorithm );

public:
    static VoronoiFilter* New( );

protected:
    VoronoiFilter( );
    virtual ~VoronoiFilter( );

    // Usual data generation method
    virtual int RequestData(
        vtkInformation* request,
        vtkInformationVector** input,
        vtkInformationVector* output
        );

private:
    // Purposely not implemented.
    VoronoiFilter( const VoronoiFilter& other );
    void operator=( const VoronoiFilter& other );

private:
    std::set<Arc> status;
    std::set<Event> eventQueue;
    std::vector<Point> points;
    std::vector<std::pair<int, int> > edgeList;

    void handleSiteEvent(Point p);
    void handleCircleEvent();
    void printBeachLine();
    void addRightCircleEvent(Point& p, std::set<Arc>::iterator& itHi,
                             std::set<Arc>::iterator& itCurrR);
    void addLeftCircleEvent(Point& p, std::set<Arc>::iterator& itLo,
                            std::set<Arc>::iterator& itCurrL);
    std::vector<Point> getPoints(Point site);
    Point circleCenter(Point p, Point q, Point r);

};

#endif // __VORONOIFILTER__H__

// eof - VoronoiFilter.h
