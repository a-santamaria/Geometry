#include "lineIntersection.h"
#include "redBlackBST.h"
#include <iostream>
#include <cstdlib>

Event::Event(Point _key, Segment _s) : key(_key), isIntersection(false) {
    segments[0] = _s;
}

Event::Event(Point _key, Segment _s, Segment _t) : key(_key),
            isIntersection(true) {
    segments[0] = _s;
    segments[1] = _t;
}

bool Event::isFirst(){
    return ( key.y >= segments[0].p.y && key.y >= segments[0].q.y );
}

bool Event::operator< (const Event& other) const{
    if(key.y == other.key.y) return key.x < other.key.x;
    return key.y < other.key.y;
}


std::vector<Point> LineIntersection::sweep_line(){
    std::vector<Point> intersections;


    for(int i = 0; i < segments.size(); i++){
        eventQueue.push(Event(segments[i].p, segments[i]));
        eventQueue.push(Event(segments[i].q, segments[i]));
    }

    RedBlackBST st;
    Point aux;

    while(!eventQueue.empty()){
        Event curr = eventQueue.top();
        eventQueue.pop();

        std::cout << "(((((((((((evento)))))))))))" << std::endl;
        std::cout << curr.key.x <<"  "<<curr.key.y << std::endl;
        std::cout << std::endl << std::endl;


        Segment prev;
        Segment next;

        //intersect event delete segments befor that
        if(curr.isIntersection){
            std::cout << "****************soy evento de inerseccion************" << std::endl;





        }//first event of segemtne then add to tree
        else if(curr.isFirst()) {

            //check for intersections with neighbours
            if( st.floor(curr.segments[0], prev) ){
                if( intersect(curr.segments[0], prev, aux) &&
                    aux.y <= Segment::sweep_lineY ) {

                }
            }
            if( st.ceiling(curr.segments[0], next) ){
                if( intersect(curr.segments[0], next, aux) &&
                    aux.y <= Segment::sweep_lineY ) {

                }
            }
            //add to the tree
            st.put(curr.segments[0]);
        }
        else{ //last event of segemnte then delete from tree
            st.del(curr.segments[0]);

            //check intersection between prev and next new neighbours
            if( st.floor(curr.segments[0], prev) &&
                st.ceiling(curr.segments[0], next) ){

                if( intersect(prev, next, aux) &&
                    aux.y <= Segment::sweep_lineY ){

                    intersections.push_back(aux);

                    //add intersection event
                    std::cout << "agregar evento de interseccion prev y next" << std::endl;
                    std::cout << "en "<<aux.x <<" "<<aux.y << std::endl;
                    Event e(aux, prev, next);
                    eventQueue.push(e);
                }
            }
        }
        st.printTree();
        std::cout << std::endl << std::endl;
    }

    return intersections;
}

bool LineIntersection::intersect(const Segment& s, const Segment& t, Point& p){

    if( ccw(s.p, s.q, t.p) == ccw(s.p, s.q, t.q) ||
        ccw(t.p, t.q, s.p) == ccw(t.p, t.q, s.q)    )
        return false;
    //TODO falta colineales y extremos

    double den = toVec(s.p, s.q).crossMag(toVec(t.p, t.q));

    double num = toVec(s.p, t.p).crossMag(toVec(t.p, t.q)) * den;


    double ss = num / (den*den);

    p.x = (s.q.x - s.p.x);
    p.y = (s.q.y - s.p.y);
    p.x = p.x*ss;
    p.y = p.y*ss;
    p = p + s.p;
    return true;
}
