#ifndef __VORONOIFILTER__H__
#define __VORONOIFILTER__H__

#include <vtkPolyDataAlgorithm.h>

#include "point.h"
#include <set>
#include <queue>

struct Arc {
    std::pair<Point, Point> sites;
    static double sweep_lineY;

    Arc();
    Arc(Point first, Point second);
    bool operator< (const Arc& other) const;
    Point getBreakpoint() const;

};

struct Event {
    Point p;
    bool isSite;

    Event();
    Event(Point _p, bool _isSite);
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
    std::priority_queue<Event> eventQueue;
    void handleSiteEvent(Point p);
    void handleCircleEvent();

};

#endif // __VORONOIFILTER__H__

// eof - VoronoiFilter.h
