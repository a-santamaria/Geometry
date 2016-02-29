#include "lineIntersection.h"
#include "redBlackBST.h"
#include <iostream>
#include <cstdlib>


std::vector<Point> LineIntersection::sweep_line(){

    std::vector<Point> intersections;

    for(int i = 0; i < segments.size(); i++){
        eventQueue.push(std::make_pair(segments[i]->p, segments[i]));
        eventQueue.push(std::make_pair(segments[i]->q, segments[i]));
    }

    RedBlackBST st;
    Point aux;

    while(!eventQueue.empty()){
        std::pair<Point, Segment*> curr = eventQueue.top();
        eventQueue.pop();

        Node* prev = st.floor(curr.first);
        Node* next = st.ceiling(curr.first);

        //first event of segemtne then add to tree
        if(curr.first.y >= curr.second->p.y &&
            curr.first.y >= curr.second->q.y){

            //check for intersections with neighbours
            if(prev != NULL){
                if( intersect(curr.second, prev->val.back(), aux) ){
                    intersections.push_back(aux);
                    Segment* prevSeg = prev->val.back();

                    //no se si hay una mejor forma en vez de borrarlos
                    st.del(prev->key);
                    st.del(curr.first);

                    //cange order put in the same key
                    st.put(aux, curr.second);
                    st.put(aux, prevSeg);

                    //add both intependantly in queue
                    eventQueue.push(std::make_pair(aux, curr.second));
                    eventQueue.push(std::make_pair(aux, prevSeg));
                }
            }
            if(next != NULL){
                if( intersect(curr.second, next->val.back(), aux) ){
                    intersections.push_back(aux);
                    Segment* nextSeg = next->val.front();

                    //no se si hay una mejor forma en vez de borrarlos
                    st.del(next->key);
                    st.del(curr.first);

                    //cange order put in the same key
                    st.put(aux, nextSeg);
                    st.put(aux, curr.second);

                    //add both intependantly in queue
                    eventQueue.push(std::make_pair(aux, nextSeg));
                    eventQueue.push(std::make_pair(aux, curr.second));
                }
            }

            //add to the tree
            st.put(curr.first, curr.second);
        }
        else{ //last event of segemnte then delete from tree
            //in case there is more than one with same key
            st.delOnly(curr.first, curr.second);
            //st.del(curr.first);
        }

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
