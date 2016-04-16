#ifndef __VORONOIFILTER__H__
#define __VORONOIFILTER__H__

#include <vtkPolyDataAlgorithm.h>

#include "point.h"
#include <set>
#include <queue>
#include <map>
#include <vector>


struct Arc {
    std::pair<Point, Point> sites;
    static double sweep_lineY;
    bool hasCircleEvent;
    Arc();
    Arc(Point first, Point second);
    bool operator< (const Arc& other) const;
    double getBreakpointX() const;

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

typedef std::set<Event>::iterator eventIterator;

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
