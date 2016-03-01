#include "lineIntersection.h"
#include "redBlackBST.h"
#include <iostream>
#include <cstdlib>


std::vector<Point> LineIntersection::sweep_line(){

    std::vector<Point> intersections;

    for(int i = 0; i < segments.size(); i++){
        eventQueue.push(Event(segments[i]->p, segments[i]));
        eventQueue.push(Event(segments[i]->q, segments[i]));
    }

    RedBlackBST st;
    Point aux;

    while(!eventQueue.empty()){
        Event curr = eventQueue.top();
        eventQueue.pop();

        std::cout << "(((((((((((evento)))))))))))" << std::endl;
        std::cout << curr.key.x <<"  "<<curr.key.y << std::endl;
        std::cout << std::endl << std::endl;


        Node* prev;
        Node* next;

        //intersect event delete segments befor that
        if(curr.hasPrev){
            std::cout << "*******************************soy evento de inerseccion***************" << std::endl;


            std::list<Point>::iterator it = curr.prevKey.begin();
            std::list<Segment*>::iterator its = curr.segments.begin();
            for( ; it != curr.prevKey.end(); it++){
                std::cout << "voy a borrar "<< it->x<< std::endl;
                st.delOnly(*it, *its);
                its++;
            }

            //TODO change order
            std::list<Segment*>::reverse_iterator rit = curr.segments.rbegin();

            std::list<Segment*>::reverse_iterator first = curr.segments.rbegin();
            std::list<Segment*>::iterator last = curr.segments.begin();

            prev = st.floor( (*first)->lastKey );
            next = st.ceiling( (*last)->lastKey );

            for( ; rit != curr.segments.rend(); rit++)
                st.put(curr.key, *rit);
            // check intersection with new neighbours

            if(prev != NULL){
                std::cout << "encontre prev" << std::endl;
                std::cout << prev->key.x << " "<< prev->key.y << std::endl;
                if( intersect(*first, prev->val.back(), aux) && aux.y <= curr.key.y){
                    intersections.push_back(aux);
                    Segment* prevSeg = prev->val.back();

                    //add event to eventQueue
                    std::cout << "agregar evento de interseccion prev" << std::endl;
                    std::cout << "en "<<aux.x <<" "<<aux.y << std::endl;
                    std::cout << "parents "<< prev->key.x <<" y "<<curr.key.x<< std::endl;
                    Event e(aux);
                    e.addSegment(prevSeg, prev->key);
                    prevSeg->lastKey = aux;
                    e.addSegment(*first, curr.key);
                    (*first)->lastKey = aux;



                    eventQueue.push(e);
                }
            }
            if(next != NULL){
                std::cout << "encontre next" << std::endl;
                std::cout << next->key.x << " "<< next->key.y << std::endl;
                if( intersect(*last, next->val.front(), aux) && aux.y <= curr.key.y){
                    intersections.push_back(aux);
                    Segment* nextSeg = next->val.front();


                    //add event to eventQueue
                    std::cout << "agregar evento de interseccion next" << std::endl;
                    Event e(aux);
                    e.addSegment(*last, curr.key);
                    (*last)->lastKey = aux;
                    e.addSegment(nextSeg, next->key);
                    nextSeg->lastKey = aux;
                    eventQueue.push(e);
                }
            }


        }//first event of segemtne then add to tree
        else if(curr.key.y >= curr.s->p.y &&
            curr.key.y >= curr.s->q.y){

            prev = st.floor(curr.key);
            next = st.ceiling(curr.key);
            //check for intersections with neighbours
            if(prev != NULL){

                std::cout << "encontre prev" << std::endl;
                std::cout << "prev " << (prev->val.back())->p.x << " " << (prev->val.back())->p.y << std::endl;
                std::cout << (prev->val.back())->q.x << " " << (prev->val.back())->q.y << std::endl;
                if( intersect(curr.s, prev->val.back(), aux) && aux.y <= curr.key.y){
                    intersections.push_back(aux);
                    Segment* prevSeg = prev->val.back();

                    //add both intependantly in queue
                    std::cout << "agregar evento de interseccion prev" << std::endl;
                    std::cout << "en "<<aux.x <<" "<<aux.y << std::endl;
                    std::cout << "parents "<< prev->key.x <<" y "<<curr.key.x<< std::endl;
                    Event e(aux);
                    e.addSegment(prevSeg, prev->key);
                    prevSeg->lastKey = aux;
                    e.addSegment(curr.s, curr.key);
                    curr.s->lastKey = aux;
                    eventQueue.push(e);
                }
            }
            if(next != NULL){
                if( intersect(curr.s, next->val.front(), aux) && aux.y <= curr.key.y){
                    intersections.push_back(aux);
                    Segment* nextSeg = next->val.front();


                    //add both intependantly in queue
                    std::cout << "agregar evento de interseccion next" << std::endl;
                    std::cout << "en "<<aux.x <<" "<<aux.y << std::endl;
                    std::cout << "parents "<< next->key.x <<" y "<<curr.key.x<< std::endl;
                    Event e(aux);
                    e.addSegment(curr.s, curr.key);
                    curr.s->lastKey = aux;
                    e.addSegment(nextSeg, next->key);
                    nextSeg->lastKey = aux;
                    eventQueue.push(e);
                }
            }

            //add to the tree
            st.put(curr.key, curr.s);
        }
        else{ //last event of segemnte then delete from tree
            //in case there is more than one with same key
            std::cout << "voy a borrar only" << std::endl;
            st.delOnly(curr.s->lastKey, curr.s);
            //st.del(curr.first);

            prev = st.floor(curr.key);
            next = st.ceiling(curr.key);
            //check intersection between prev and next new neighbours
            if(prev != NULL && next != NULL){
                if( intersect(prev->val.back(), next->val.front(), aux) && aux.y <= curr.key.y){
                    intersections.push_back(aux);
                    Segment* nextSeg = next->val.front();
                    Segment* prevSeg = prev->val.back();

                    //add both intependantly in queue
                    std::cout << "agregar evento de interseccion prev y next" << std::endl;
                    std::cout << "en "<<aux.x <<" "<<aux.y << std::endl;
                    std::cout << "parents "<< next->key.x <<" y "<<prev->key.x<< std::endl;
                    Event e(aux);
                    e.addSegment(prevSeg, prev->key);
                    prevSeg->lastKey = aux;
                    e.addSegment(nextSeg, next->key);
                    nextSeg->lastKey = aux;
                    eventQueue.push(e);
                }
            }
        }


        st.printTree();
        std::cout << std::endl << std::endl;
    }

    return intersections;
}

bool LineIntersection::intersect(const Segment* s, const Segment* t, Point& p){

    if( ccw(s->p, s->q, t->p) == ccw(s->p, s->q, t->q) ||
        ccw(t->p, t->q, s->p) == ccw(t->p, t->q, s->q)    )
        return false;
    //TODO falta colineales y extremos

    double den = toVec(s->p, s->q).crossMag(toVec(t->p, t->q));

    double num = toVec(s->p, t->p).crossMag(toVec(t->p, t->q)) * den;


    double ss = num / (den*den);

    p.x = (s->q.x - s->p.x);
    p.y = (s->q.y - s->p.y);
    p.x = p.x*ss;
    p.y = p.y*ss;
    p = p + s->p;
    return true;
}
