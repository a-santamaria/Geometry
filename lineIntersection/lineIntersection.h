#ifndef LINE_INTERSECTION_H
#define LINE_INTERSECTION_H

#include <vector>
#include <list>
#include <set>
#include <cassert>
#include "point.h"
#include <cmath>
#include <cfloat>
#include <map>

#define INF DBL_MAX
#define EPS 1e-9

class Event{
public:
    Point key;
    int idSegment;
    int tipo;
    enum Type { INTER, START, END };

    Event() {}
    Event(Point _key, int _idSegment);
    Event(Point _key, Type _tipo);
    Event(const Event& other);

    bool isFirst();
    bool operator< (const Event& other) const;
};

class LineIntersection {
public:
    static std::vector< Point > points;
    static double sweep_lineY;


    LineIntersection();
    LineIntersection(std::vector< Point > _points,
                     std::vector<Segment> _segments);
    std::vector<Point> sweep_line();

    struct SegmentComparator {
        double getXInSweepLine(const Segment& s) const;
        bool operator()(const Segment& a, const Segment& b) const;
    };
    typedef std::set<Segment, SegmentComparator>::iterator setSegmentIterator;
    typedef std::set<Segment, SegmentComparator>::reverse_iterator setSegmentReverseIterator;
    struct SegmentComparator2 {
        double getXInSweepLine(const Segment& s) const;
        bool operator()(const  std::pair< Segment, int >& a,
                        const  std::pair< Segment, int >& b) const;
    };
    typedef std::set< std::pair< Segment, int >,
                        SegmentComparator2 >::iterator setSegmentIterator2;

    typedef std::set< std::pair< Segment, int >,
                        SegmentComparator2 >::reverse_iterator
                                                setSegmentReverseIterator2;

    typedef std::map<int, std::set< std::pair< Segment, int >,
                        SegmentComparator2 > >::iterator subSetMapIterator;

private:
    std::vector< Segment > segments;
    //subsets of segments that contain the point in the interior
    std::map<int, std::set< std::pair< Segment, int >,
                        SegmentComparator2 > >contain;
    //segment id to iterator in status tree
    //TODO hasmap in c++ 11
    std::map<int, setSegmentIterator> locationInSet;
    //status tree
    std::set<Segment, SegmentComparator> st;
    //queue of events sorted by y coordinate
    std::map<Point, Event> eventQueue;
    //intersection points
    std::vector<Point> intersections;
    //point identifiers
    std::map<Point, int> ids;

    void swapOrder(int ids, int idt);
    bool intersect(const Segment& s, const Segment& t, Point& p);
    void encontrarPrev(setSegmentIterator me, setSegmentIterator &prev,
                                     std::set<Segment, SegmentComparator>& st);
    void encontrarNext(setSegmentIterator me, setSegmentIterator &next,
                                     std::set<Segment, SegmentComparator>& st);
    void encontrarPrevNext(Segment& s,
                           setSegmentIterator &prev,
                           setSegmentIterator &next,
                           std::set<Segment, SegmentComparator>& st);
   void crearEventoInter(setSegmentIterator first, setSegmentIterator last,
                         const Point& aux);

    void printST();

};

#endif
