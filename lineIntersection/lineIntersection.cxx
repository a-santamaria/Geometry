#include "lineIntersection.h"
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <set>
#include <map>

#include "point.h"


std::vector< Point > LineIntersection::points;
double LineIntersection::sweep_lineY = INF;


Event::Event(Point _key, Segment _segment) : key(_key), tipo(Event::START) {
    segment = _segment;
}

Event::Event(Point _key, int _tipo) : key(_key), tipo(_tipo) { }

Event::Event(const Event& other) {
    key = other.key;
    segment = other.segment;
    tipo = other.tipo;
}

bool Event::isFirst() {
    return tipo == Event::START;
}

bool Event::operator< (const Event& other) const {
    if(key.y == other.key.y) return key.x < other.key.x;
    return key.y < other.key.y;
}

LineIntersection::LineIntersection(std::vector< Point > _points,
                 std::vector<Segment> _segments) : segments(_segments) {
    LineIntersection::points = _points;
    sweep_lineY = INF;
}

double LineIntersection::SegmentComparator::
                getXInSweepLine(const Segment& s) const {
    //TODO if it is horizontal
    double dx = ( points[s.idq].x - points[s.idp].x );
    double dy = ( points[s.idq].y - points[s.idp].y );
    double m = dx / dy;
    return points[s.idp].x - (m * ( points[s.idp].y - sweep_lineY ) );
}

bool LineIntersection::SegmentComparator::
            operator()(const Segment& a, const Segment& b) const {
    return getXInSweepLine(a) < getXInSweepLine(b);
}

double LineIntersection::SegmentComparator2::
                getXInSweepLine(const Segment& s) const {
    //TODO if it is horizontal

    double dx = ( points[s.idq].x - points[s.idp].x );
    double dy = ( points[s.idq].y - points[s.idp].y );
    double m = dx / dy;
    return points[s.idp].x - (m * ( points[s.idp].y - sweep_lineY ) );
}

bool LineIntersection::SegmentComparator2::
operator()(const  std::pair< Segment, setSegmentIterator >& a,
           const  std::pair< Segment, setSegmentIterator >& b) const {
    return getXInSweepLine(a.first) < getXInSweepLine(b.first);
}


std::vector<Point> LineIntersection::sweep_line() {
    std::vector<Point> intersections;

    std::map<Point, Event> eventQueue;
    std::map<Point, Event>::iterator itEventQueue;

    std::map<Point, int> ids;
    for(int i = 0; i < points.size(); i++){
        ids[ points[i] ] = i;
    }

    for(int i = 0; i < segments.size(); i++) {
        Event ep(points[segments[i].idp], segments[i]);
        Event eq(points[segments[i].idq], segments[i]);
        if( points[segments[i].idp] < points[segments[i].idq] )
            eq.tipo = Event::END;
        else
            ep.tipo = Event::END;
        eventQueue[ points[segments[i].idp] ] = ep;
        eventQueue[ points[segments[i].idq] ] = eq;
    }

    std::set<Segment, SegmentComparator>::iterator next;
    std::set<Segment, SegmentComparator>::iterator prev;
    Point aux;
    int id;
    std::set<Segment, SegmentComparator> st;

    //subsets of segments that contain the point in the interior
    std::map<int, std::set< std::pair< Segment, setSegmentIterator >,
                        SegmentComparator2 > >contain;

    while(!eventQueue.empty()){

        std::cout << "sacar neuvo evento" << std::endl;
        itEventQueue = eventQueue.begin();
        Event curr(itEventQueue->second);
        std::cout << "saque " << curr.key.y << std::endl;
        eventQueue.erase(itEventQueue);
        std::cout << "borre" << std::endl;

        sweep_lineY = curr.key.y;

        if (curr.tipo == Event::START) {

            std::pair<setSegmentIterator, bool>
                returnInsert = st.insert(curr.segment);
            setSegmentIterator itCurr = returnInsert.first;
            encontrarPrevNext(curr.segment, prev, next, st);

            if(next != st.end()){
                std::cout << "---------------next está en ----" << std::endl;
                std::cout << points[next->idp].x <<" "<<points[next->idp].y << " <> ";
                std::cout  << points[next->idq].x <<" "<<points[next->idq].y << std::endl;
            }

            if(prev != st.end()){
                std::cout << "---------------prev está en ----" << std::endl;
                std::cout << points[prev->idp].x <<" "<<points[prev->idp].y << " <> ";
                std::cout  << points[prev->idq].x <<" "<<points[prev->idq].y << std::endl;
            }

            if( prev != st.end() && intersect(curr.segment, *prev, aux) &&
                aux.y < sweep_lineY ) {
                std::cout << aux.x << " "<<aux.y << std::endl;
                itEventQueue = eventQueue.find(aux);
                if( itEventQueue == eventQueue.end() ){
                    Event e(aux, Event::INTER);
                    eventQueue[aux] = e;
                    intersections.push_back(aux);
                    id = points.size();
                    points.push_back(aux);
                    ids[aux] = id;
                }else{
                    id = ids[aux];
                }

                //either way insert in subset of segments that contain aux
                contain[id].insert(std::make_pair(*prev, prev));
                contain[id].insert(std::make_pair(curr.segment, itCurr));
            }

            if( next != st.end() && intersect(curr.segment, *next, aux) &&
                aux.y < sweep_lineY ) {
                std::cout << "encontre interesecion en " << std::endl;
                std::cout << aux.x << " "<<aux.y << std::endl;
                itEventQueue = eventQueue.find(aux);
                if( itEventQueue == eventQueue.end() ){
                    std::cout << "guardar" << std::endl;
                    Event e(aux, Event::INTER);
                    eventQueue[aux] = e;
                    intersections.push_back(aux);
                    id = points.size();
                    points.push_back(aux);
                    ids[aux] = id;
                }else{
                    id = ids[aux];
                }
                //either way insert in subset of segments that contain aux
                contain[id].insert(std::make_pair(*next, next));
                contain[id].insert(std::make_pair(curr.segment, itCurr));
            }

        }
        else if (curr.tipo == Event::END) {
            if( prev != st.end() && next  != st.end() &&
                intersect(*prev, *next, aux) && aux.y < sweep_lineY &&
                eventQueue.find(aux) == eventQueue.end() ){

                    Event e(aux, Event::INTER);
                    eventQueue[aux] = e;
                intersections.push_back(aux);
            }
        }
        else if (curr.tipo == Event::INTER) {
            std::cout << "soy interseccion" << std::endl;
            id = ids[curr.key];
            //get the subset of segments that contain the poitn curr.key
            subSetMapIterator mapIt = contain.find(id);

            setSegmentIterator2 subSetIt = (mapIt->second).begin();
            setSegmentReverseIterator2 subSetReversIt = (mapIt->second).rbegin();

            setSegmentIterator first = subSetIt->second;
            setSegmentIterator last = subSetReversIt->second;

            //swap outer segments
            swapOrder(subSetIt->second, subSetReversIt->second);

            //change order of iner segments in st
            for(int i = 1; i < (mapIt->second).size()/2; i++){
                swapOrder(subSetIt->second, subSetReversIt->second);

                subSetIt++;
                subSetReversIt++;
            }
            std::cout << "cambien quedo: " << std::endl;
            std::cout << "first " << points[first->idp].x << " " << points[first->idp].y << " <> ";
            std::cout << points[first->idq].x << " " << points[first->idq].y << std::endl;

            std::cout << "last " << points[last->idp].x << " " << points[last->idp].y << " <> ";
            std::cout << points[last->idq].x << " " << points[last->idq].y << std::endl;

            encontrarPrev(first, prev, st);
            if( prev != st.end() && intersect(*first, *prev, aux) &&
                aux.y < sweep_lineY ) {

                itEventQueue = eventQueue.find(aux);
                if( itEventQueue == eventQueue.end() ){
                    Event e(aux, Event::INTER);
                    eventQueue[aux] = e;
                    intersections.push_back(aux);
                    id = points.size();
                    points.push_back(aux);
                    ids[aux] = id;
                }else{
                    id = ids[aux];
                }

                //either way insert in subset of segments that contain aux
                contain[id].insert(std::make_pair(*prev, prev));
                contain[id].insert(std::make_pair(*first, first));
            }
            encontrarNext(last, next, st);
            if( next != st.end() && intersect(*last, *next, aux) &&
                aux.y < sweep_lineY ) {

                itEventQueue = eventQueue.find(aux);
                if( itEventQueue == eventQueue.end() ){
                    Event e(aux, Event::INTER);
                    eventQueue[aux] = e;
                    intersections.push_back(aux);
                    id = points.size();
                    points.push_back(aux);
                    ids[aux] = id;
                }else{
                    id = ids[aux];
                }

                //either way insert in subset of segments that contain aux
                contain[id].insert(std::make_pair(*next, next));
                contain[id].insert(std::make_pair(*last, last));
            }


        }

    }


    return intersections;
}

void LineIntersection::encontrarPrev(setSegmentIterator me, setSegmentIterator &prev,
                                    std::set<Segment, SegmentComparator>& st) {
    setSegmentIterator it = me;
    if( it != st.end() ) {
        setSegmentIterator itOther = it;
        if( itOther != st.begin() ) {
            itOther--;
            std::cout << "entoncre prev" << std::endl;
            std::cout << points[itOther->idp].x << " " << points[itOther->idp].y << " <> ";
            std::cout << points[itOther->idq].x << " " << points[itOther->idq].y << std::endl;
            prev = itOther;
            return;
        }
    }

    prev = st.end();
}

void LineIntersection::encontrarNext(setSegmentIterator me, setSegmentIterator &next,
                                    std::set<Segment, SegmentComparator>& st) {
    setSegmentIterator it = me;
    if( it != st.end() ) {
        setSegmentIterator itOther = it;
        itOther++;
        if( itOther != st.end() ) {
            std::cout << "encontre next" << std::endl;
            std::cout << points[itOther->idp].x << " " << points[itOther->idp].y << " <> ";
            std::cout << points[itOther->idq].x << " " << points[itOther->idq].y << std::endl;
            next = itOther;
            return;
        }
    }

    next = st.end();
}

void LineIntersection::encontrarPrevNext(Segment& s,
                        setSegmentIterator &prev,
                        setSegmentIterator &next,
                        std::set<Segment, SegmentComparator>& st) {
	std::set<Segment, SegmentComparator>::iterator it;
    std::set<Segment, SegmentComparator>::iterator itOther;
	std::cout << "----------------------st size----------->  " << st.size() << std::endl;
    next = st.end();
    prev = st.end();
    it = st.find(s);
    if(it == st.end()) { std::cout<<"==============>paiala find" << std::endl; return;}
    itOther = it;
    itOther++;
		std::cout << "----------------------st size----------->  " << st.size() << std::endl;
    if(itOther != st.end()){
        std::cout << "----------------------00000000000----------->encontre next" << std::endl;

        next =  itOther;

        std::cout << points[itOther->idp].x <<" "<<points[itOther->idp].y << " <> ";
        std::cout  << points[itOther->idq].x <<" "<<points[itOther->idq].y << std::endl;
    }
    itOther = it;
    if(itOther != st.begin()){
        itOther--;
        std::cout << "-------------------0000000000000000-------------->encontre prev" << std::endl;
        prev = itOther;
    }
}

void LineIntersection::swapOrder(const setSegmentIterator &p,
                                 const setSegmentIterator &q) {

    Segment aux( *p );

    p->idp = q->idp;
    p->idq = q->idq;

    q->idp = aux.idp;
    q->idq = aux.idq;
}

bool LineIntersection::intersect(const Segment& s, const Segment& t, Point& p){
    if( ccw(points[s.idp], points[s.idq], points[t.idp]) ==
        ccw(points[s.idp], points[s.idq], points[t.idq]) ||
        ccw(points[t.idp], points[t.idq], points[s.idp]) ==
        ccw(points[t.idp], points[t.idq], points[s.idq])    ){
            std::cout << "************************No intersecta" << std::endl;
        return false;
    }

    //TODO falta colineales y extremos

    double den = toVec(points[s.idp], points[s.idq]).
                    crossMag(toVec(points[t.idp], points[t.idq]));

    double num = toVec(points[s.idp], points[t.idp]).
                    crossMag(toVec(points[t.idp], points[t.idq])) * den;


    double ss = num / (den*den);

    p.x = (points[s.idq].x - points[s.idp].x);
    p.y = (points[s.idq].y - points[s.idp].y);
    p.x = p.x*ss;
    p.y = p.y*ss;
    p = p + points[s.idp];
    std::cout << "intersecta en "<<p.x<< " "<<p.y << std::endl;
    return true;
}
