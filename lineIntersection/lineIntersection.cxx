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


Event::Event(Point _key, int _idSegment) : key(_key), tipo(Event::START) {
    idSegment = _idSegment;
}

Event::Event(Point _key, Type _tipo) : key(_key), tipo(_tipo) { }

Event::Event(const Event& other) {
    key = other.key;
    idSegment = other.idSegment;
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
    return (getXInSweepLine(b) - getXInSweepLine(a)) > EPS;
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
operator()(const  std::pair< Segment, int >& a,
           const  std::pair< Segment, int >& b) const {
    return (getXInSweepLine(b.first) - getXInSweepLine(a.first)) > EPS;
}


std::vector<Point> LineIntersection::sweep_line() {

    std::map<Point, Event>::iterator itEventQueue;

    ids.clear();
    for(int i = 0; i < points.size(); i++){
        ids[ points[i] ] = i;
    }
    eventQueue.clear();
    for(int i = 0; i < segments.size(); i++) {
        segments[i].id = i;
        Event ep(points[segments[i].idp], i);
        Event eq(points[segments[i].idq], i);
        if( points[segments[i].idp] < points[segments[i].idq] )
            eq.tipo = Event::END;
        else
            ep.tipo = Event::END;
        eventQueue[ points[segments[i].idp] ] = ep;
        eventQueue[ points[segments[i].idq] ] = eq;
    }

    setSegmentIterator next;
    setSegmentIterator prev;
    Point aux;
    int id;

    intersections.clear();
    st.clear();
    contain.clear();
    locationInSet.clear();


    while(!eventQueue.empty()){

        itEventQueue = eventQueue.begin();
        Event curr(itEventQueue->second);
        std::cout << "saque " << curr.key.y << std::endl;
        eventQueue.erase(itEventQueue);

        sweep_lineY = curr.key.y;

        if (curr.tipo == Event::START) {
            std::pair<setSegmentIterator, bool>
                returnInsert = st.insert(segments[curr.idSegment]);

            setSegmentIterator itCurr = returnInsert.first;
            locationInSet[curr.idSegment] = itCurr;

            encontrarPrevNext(segments[curr.idSegment], prev, next, st);


            if( prev != st.end() &&
                intersect(segments[curr.idSegment], *prev, aux) &&
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
                contain[id].insert(std::make_pair(*prev, prev->id));
                contain[id].insert(
                        std::make_pair(segments[curr.idSegment], itCurr->id));
            }

            if( next != st.end() &&
                intersect(segments[curr.idSegment], *next, aux) &&
                aux.y < sweep_lineY ) {
                //TODO copiar
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
                contain[id].insert(std::make_pair(*next, next->id));
                contain[id].insert(
                        std::make_pair(segments[curr.idSegment], itCurr->id));
            }

        }
        else if (curr.tipo == Event::END) {
            encontrarPrevNext(segments[curr.idSegment], prev, next, st);
            if( prev != st.end() && next  != st.end() &&
                intersect(*prev, *next, aux) && aux.y < sweep_lineY &&
                eventQueue.find(aux) == eventQueue.end() ){

                    Event e(aux, Event::INTER);
                    eventQueue[aux] = e;
                intersections.push_back(aux);
                crearEventoInter(prev, next, aux);
            }
            st.erase(segments[curr.idSegment]);
            locationInSet[curr.idSegment] = st.end();
        }
        else if (curr.tipo == Event::INTER) {
            id = ids[curr.key];
            //get the subset of segments that contain the poitn curr.key
            subSetMapIterator mapIt = contain.find(id);
            setSegmentIterator2 subSetIt = (mapIt->second).begin();
            setSegmentReverseIterator2 subSetReversIt = (mapIt->second).rbegin();

            /*
            setSegmentIterator first = locationInSet[subSetIt->second];
            setSegmentIterator last = locationInSet[subSetReversIt->second];
            */

            //swap outer segments
            swapOrder(subSetIt->second, subSetReversIt->second);

            //change order of iner segments in st
            for(int i = 1; i < (mapIt->second).size()/2; i++){
                swapOrder(subSetIt->second, subSetReversIt->second);

                subSetIt++;
                subSetReversIt++;
            }
            setSegmentIterator last = locationInSet[subSetIt->second];
            setSegmentIterator first = locationInSet[subSetReversIt->second];

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
                contain[id].insert(std::make_pair(*prev, prev->id));
                contain[id].insert(std::make_pair(*first, first->id));
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
                contain[id].insert(std::make_pair(*next, next->id));
                contain[id].insert(std::make_pair(*last, last->id));
            }


        }

    }


    return intersections;
}


void LineIntersection::printST(){
    setSegmentIterator it = st.begin();
    std::cout << "/* -----------st----------- */" << std::endl;
    for(;it != st.end(); it++){
        std::cout<<it->id<<" ";
    }
    std::cout <<std::endl<< "/* ---------------------- */" << std::endl;
}

void LineIntersection::crearEventoInter(setSegmentIterator first,
                                        setSegmentIterator last,
                                        const Point& aux){
    std::map<Point, Event>::iterator  itEventQueue = eventQueue.find(aux);
    int id;
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

    sweep_lineY -= 1;
    contain[id].insert(std::make_pair(*first, first->id));
    contain[id].insert(std::make_pair(*last, last->id));
    sweep_lineY += 1;

}

void LineIntersection::encontrarPrev(setSegmentIterator me, setSegmentIterator &prev,
                                    std::set<Segment, SegmentComparator>& st) {
    setSegmentIterator it = me;
    if( it != st.end() ) {
        setSegmentIterator itOther = it;
        if( itOther != st.begin() ) {
            itOther--;
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
    next = st.end();
    prev = st.end();
    it = st.find(s);
    if(it == st.end()) { return;}
    itOther = it;
    itOther++;
    if(itOther != st.end()){

        next =  itOther;

    }
    itOther = it;
    if(itOther != st.begin()){
        itOther--;
        prev = itOther;
    }
}

void LineIntersection::swapOrder(int ids, int idt) {

    assert(locationInSet[ids] != st.end());
    assert(locationInSet[idt] != st.end());

    Segment aux( *(locationInSet[ids]) );

    locationInSet[ids]->idp = locationInSet[idt]->idp;
    locationInSet[ids]->idq = locationInSet[idt]->idq;
    locationInSet[ids]->id  = locationInSet[idt]->id;

    locationInSet[idt]->idp = aux.idp;
    locationInSet[idt]->idq = aux.idq;
    locationInSet[idt]->id  = aux.id;

    //cange iterators
    setSegmentIterator auxIt = locationInSet[ids];
    locationInSet[ids] = locationInSet[idt];
    locationInSet[idt] = auxIt;
}

bool LineIntersection::intersect(const Segment& s, const Segment& t, Point& p){
    if( ccw(points[s.idp], points[s.idq], points[t.idp]) ==
        ccw(points[s.idp], points[s.idq], points[t.idq]) ||
        ccw(points[t.idp], points[t.idq], points[s.idp]) ==
        ccw(points[t.idp], points[t.idq], points[s.idq])    ){
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
    return true;
}
