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

class Event{
public:
    Point key;
    Segment segment;
    int tipo;

    static const int INTER = 0;
    static const int START = 1;
    static const int END = -1;

    Event() {}
    Event(Point _key, Segment _segment);
    Event(Point _key, int _tipo);
    Event(const Event& other);

    bool isFirst();
    bool operator< (const Event& other) const;
};

class LineIntersection {
public:
    static std::vector< Point > points;
    static double sweep_lineY;

private:
    std::vector< Segment > segments;


public:

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
        bool operator()(const  std::pair< Segment, setSegmentIterator >& a,
                        const  std::pair< Segment, setSegmentIterator >& b) const;
    };
    typedef std::set< std::pair< Segment, setSegmentIterator >,
                        SegmentComparator2 >::iterator setSegmentIterator2;

    typedef std::set< std::pair< Segment, setSegmentIterator >,
                        SegmentComparator2 >::reverse_iterator
                                                setSegmentReverseIterator2;

    typedef std::map<int, std::set< std::pair< Segment, setSegmentIterator >,
                        SegmentComparator2 > >::iterator subSetMapIterator;

private:
    void swapOrder(const setSegmentIterator &p, const setSegmentIterator &q);
    bool intersect(const Segment& s, const Segment& t, Point& p);
    void encontrarPrev(setSegmentIterator me, setSegmentIterator &prev,
                                     std::set<Segment, SegmentComparator>& st);
    void encontrarNext(setSegmentIterator me, setSegmentIterator &next,
                                     std::set<Segment, SegmentComparator>& st);
    void encontrarPrevNext(Segment& s,
                           setSegmentIterator &prev,
                           setSegmentIterator &next,
                           std::set<Segment, SegmentComparator>& st);

};

#endif
