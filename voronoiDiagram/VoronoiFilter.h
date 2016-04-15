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

    Arc();
    Arc(Point first, Point second);
    bool operator< (const Arc& other) const;
    double getBreakpointX() const;

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
    std::map<Point, Event> circleEvents;

    void handleSiteEvent(Point p);
    void handleCircleEvent();
    void printBeachLine();

    std::vector<Point> getPoints(Point site);
    Point circleCenter(Point p, Point q, Point r);

};

#endif // __VORONOIFILTER__H__

// eof - VoronoiFilter.h
