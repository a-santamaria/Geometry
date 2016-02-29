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


        Node* prev = st.floor(curr.key);
        Node* next = st.ceiling(curr.key);

        //first event of segemtne then add to tree
        if(curr.key.y >= curr.s->p.y &&
            curr.key.y >= curr.s->q.y){

            //check for intersections with neighbours
            if(prev != NULL){
                if( intersect(curr.s, prev->val.back(), aux) ){
                    intersections.push_back(aux);
                    Segment* prevSeg = prev->val.back();

                    //intersect event delete segments befor that
                    if(curr.hasPrev){
                        st.del(curr.prevKey);
                    }

                    //cange order put in the same key
                    st.put(aux, curr.s);
                    st.put(aux, prevSeg);

                    //add both intependantly in queue
                    eventQueue.push(Event(aux, curr.s, curr.key));
                    eventQueue.push(Event(aux, prevSeg, prev->key));
                }
            }
            if(next != NULL){
                if( intersect(curr.s, next->val.back(), aux) ){
                    intersections.push_back(aux);
                    Segment* nextSeg = next->val.front();

                    //intersect event delete segments befor that
                    if(curr.hasPrev){
                        st.del(curr.prevKey);
                    }

                    //cange order put in the same key
                    st.put(aux, nextSeg);
                    st.put(aux, curr.s);

                    //add both intependantly in queue
                    eventQueue.push(Event(aux, nextSeg, next->key));
                    eventQueue.push(Event(aux, curr.s, curr.key));
                }
            }

            //add to the tree
            st.put(curr.key, curr.s);
        }
        else{ //last event of segemnte then delete from tree
            //in case there is more than one with same key
            st.delOnly(curr.key, curr.s);
            //st.del(curr.first);
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
