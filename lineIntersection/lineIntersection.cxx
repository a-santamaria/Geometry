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

        //update sweep_lineY coordinate
        Segment::sweep_lineY = curr.key.y;

        std::cout << "(((((((((((evento)))))))))))" << std::endl;
        std::cout << curr.key.x <<"  "<<curr.key.y << std::endl;
        std::cout << std::endl << std::endl;

        std::cout << "/* sweep line y */   " << Segment::sweep_lineY << std::endl;

        Segment prev;
        Segment next;

        //intersect event delete segments befor that
        if(curr.isIntersection){
            std::cout << "****************soy evento de inerseccion************" << std::endl;
            std::cout << "en: " << curr.key.x << " " << curr.key.y << std::endl;

            std::cout << "antes de swap" << std::endl;
            //swap order of segements in status tree st
            st.swapOrder(curr.segments[0], curr.segments[1]);

            std::cout << "despues de swap" << std::endl;
            //check intersections with new neighbours
            if( st.floor(curr.segments[1], prev) ){
                /*std::cout << "encontre prev" << std::endl;
                std::cout << prev.p.x << " " <<  prev.p.y << std::endl;
                std::cout << prev.q.x << " " <<  prev.q.y << std::endl;
                std::cout << "yo" << std::endl;
                std::cout << curr.segments[1].p.x << " " <<  curr.segments[1].p.y << std::endl;
                std::cout << curr.segments[1].q.x << " " <<  curr.segments[1].q.y << std::endl;
                */
                if( intersect(curr.segments[1], prev, aux) &&
                    aux.y <= Segment::sweep_lineY ) {
                    //add intersection
                    intersections.push_back(aux);
                    std::cout << "agrege evento con prev en ";
                    std::cout << aux.x << " " << aux.y << std::endl;
                    //add intersection event
                    eventQueue.push(Event(aux, prev, curr.segments[1]));
                }

            }

            if( st.ceiling(curr.segments[0], next) ){
                /*std::cout << "encontre next" << std::endl;
                std::cout << next.p.x << " " <<  next.p.y << std::endl;
                std::cout << next.q.x << " " <<  next.q.y << std::endl;
                std::cout << "yo" << std::endl;
                std::cout << curr.segments[0].p.x << " " <<  curr.segments[0].p.y << std::endl;
                std::cout << curr.segments[0].q.x << " " <<  curr.segments[0].q.y << std::endl;
                */
                if( intersect(curr.segments[0], next, aux) &&
                    aux.y <= Segment::sweep_lineY ) {
                    //add intersection
                    intersections.push_back(aux);
                    std::cout << "agrege evento con next";
                    std::cout << aux.x << " " << aux.y << std::endl;
                    //add intersection event
                    eventQueue.push(Event(aux, curr.segments[0], next));
                }

            }



        }//first event of segemtne then add to tree
        else if(curr.isFirst()) {
            std::cout << "---------------es first----------" << std::endl;
            //check for intersections with neighbours
            if( st.floor(curr.segments[0], prev) ){
                if( intersect(curr.segments[0], prev, aux) &&
                    aux.y <= Segment::sweep_lineY ) {
                        //add intersection
                        intersections.push_back(aux);

                        //add intersection event
                        eventQueue.push(Event(aux, prev, curr.segments[0]));

                }
            }
            if( st.ceiling(curr.segments[0], next) ){
                if( intersect(curr.segments[0], next, aux) &&
                    aux.y <= Segment::sweep_lineY ) {
                        //add intersection
                        intersections.push_back(aux);

                        //add intersection event
                        eventQueue.push(Event(aux, curr.segments[0], next));
                }
            }
            //add to the tree
            st.put(curr.segments[0]);
        }
        else{ //last event of segemnte then delete from tree
            //std::cout << "soy last" << std::endl;
            std::cout << "es salida" << std::endl;
            /*TODO aca hay problemas al borrar aveces no exite el nodo
            siempre deberia exitir
            */
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
                    //add intersection event
                    eventQueue.push(Event(aux, prev, next));
                }
            }
        }
        //st.printTree();
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
