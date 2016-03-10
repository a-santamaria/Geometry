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

        std::cout << "sacar neuvo evento" << std::endl;
        itEventQueue = eventQueue.begin();
        Event curr(itEventQueue->second);
        std::cout << "saque " << curr.key.y << std::endl;
        eventQueue.erase(itEventQueue);

        sweep_lineY = curr.key.y;

        if (curr.tipo == Event::START) {
                std::cout << "voy a insertar con id " << segments[curr.idSegment].id  << std::endl;
            std::pair<setSegmentIterator, bool>
                returnInsert = st.insert(segments[curr.idSegment]);

            setSegmentIterator itCurr = returnInsert.first;
            std::cout << "inserte con id " << itCurr->id << std::endl;
            locationInSet[curr.idSegment] = itCurr;

            encontrarPrevNext(segments[curr.idSegment], prev, next, st);

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

            if( prev != st.end() &&
                intersect(segments[curr.idSegment], *prev, aux) &&
                aux.y < sweep_lineY ) {
                crearEventoInter(prev, itCurr, aux);
            }

            if( next != st.end() &&
                intersect(segments[curr.idSegment], *next, aux) &&
                aux.y < sweep_lineY ) {
                crearEventoInter(itCurr, next, aux);
            }

        }
        else if (curr.tipo == Event::END) {
            printST();
            encontrarPrevNext(segments[curr.idSegment], prev, next, st);
            std::cout << "soy salida idSEgemt:" << curr.idSegment << std::endl;
            if( prev != st.end())
                std::cout << "hay prev" << std::endl;
            if( next != st.end() ){
                std::cout << "hay next" << std::endl;
            }
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
            std::cout << "soy interseccion" << std::endl;
            if(ids.find(curr.key) == ids.end())
                std::cout << "no existia" << std::endl;
            id = ids[curr.key];
            std::cout << "id de " << curr.key.y << " : "<< id << std::endl;
            //get the subset of segments that contain the poitn curr.key
            subSetMapIterator mapIt = contain.find(id);
            std::cout << "size de subset " << mapIt->second.size() << std::endl;
            setSegmentIterator2 subSetIt = (mapIt->second).begin();
            setSegmentReverseIterator2 subSetReversIt = (mapIt->second).rbegin();

            /*
            setSegmentIterator first = locationInSet[subSetIt->second];
            setSegmentIterator last = locationInSet[subSetReversIt->second];
            */
            std::cout << "swap de ids:" << std::endl;
            std::cout <<subSetIt->second<<"  "<<subSetReversIt->second << std::endl;
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

            std::cout << "cambien quedo: " << std::endl;
            std::cout << "first " << points[first->idp].x << " " << points[first->idp].y << " <> ";
            std::cout << points[first->idq].x << " " << points[first->idq].y << std::endl;

            std::cout << "last " << points[last->idp].x << " " << points[last->idp].y << " <> ";
            std::cout << points[last->idq].x << " " << points[last->idq].y << std::endl;

            encontrarPrev(first, prev, st);
            if( prev != st.end() && intersect(*first, *prev, aux) &&
                aux.y < sweep_lineY ) {
                crearEventoInter(prev, first, aux);
            }
            encontrarNext(last, next, st);
            if( next != st.end() && intersect(*last, *next, aux) &&
                aux.y < sweep_lineY ) {
                crearEventoInter(last, next, aux);
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
    std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&77voy a mater ids: ";
    std::cout << first->id << " " << last->id << std::endl;

    double guardar = sweep_lineY;
    sweep_lineY = aux.y + 1;
    contain[id].insert(std::make_pair(*first, first->id));
    contain[id].insert(std::make_pair(*last, last->id));
    sweep_lineY = guardar;

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
    std::cout << "voy a buscar con st en y: " << sweep_lineY << std::endl;
    it = st.find(s);
    if(it == st.end()) { std::cout<<"==============>paiala find" << std::endl; return;}
    std::cout << "encontre " << it->id << std::endl;
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
