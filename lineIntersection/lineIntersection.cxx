#include "lineIntersection.h"
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <set>

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

    //status tree
    std::set<Segment> st;
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

        Segment* prev = NULL;
        Segment* next = NULL;

        //intersect event delete segments befor that
        if(curr.isIntersection){
            std::cout << "****************soy evento de inerseccion************" << std::endl;
            std::cout << "en: " << curr.key.x << " " << curr.key.y << std::endl;
            int n;
            //std::cin>>n;
            std::cout << "antes de swap" << std::endl;
            //swap order of segements in status tree st
            swapOrder(st, curr.segments[0], curr.segments[1], prev, next);

            std::cout << "despues de swap" << std::endl;
            //check intersections with new neighbours

            if( prev != NULL ){
                std::cout << "encontre prev" << std::endl;
                std::cout << prev->p.x << " " <<  prev->p.y << std::endl;
                std::cout << prev->q.x << " " <<  prev->q.y << std::endl;
                std::cout << "yo" << std::endl;
                std::cout << curr.segments[1].p.x << " " <<  curr.segments[1].p.y << std::endl;
                std::cout << curr.segments[1].q.x << " " <<  curr.segments[1].q.y << std::endl;

                if( intersect(curr.segments[1], *prev, aux) &&
                    aux.y <= Segment::sweep_lineY ) {
                    //add intersection
                    intersections.push_back(aux);
                    std::cout << "agrege evento con prev en ";
                    std::cout << aux.x << " " << aux.y << std::endl;
                    //add intersection event
                    eventQueue.push(Event(aux, *prev, curr.segments[1]));
                }

            }

            if( next != NULL ){
                std::cout << "encontre next" << std::endl;
                std::cout << next->p.x << " " <<  next->p.y << std::endl;
                std::cout << next->q.x << " " <<  next->q.y << std::endl;
                std::cout << "yo" << std::endl;
                std::cout << curr.segments[0].p.x << " " <<  curr.segments[0].p.y << std::endl;
                std::cout << curr.segments[0].q.x << " " <<  curr.segments[0].q.y << std::endl;
                if( intersect(curr.segments[0], *next, aux) &&
                    aux.y <= Segment::sweep_lineY ) {
                    //add intersection
                    intersections.push_back(aux);
                    std::cout << "agrege evento con next";
                    std::cout << aux.x << " " << aux.y << std::endl;
                    //add intersection event
                    eventQueue.push(Event(aux, curr.segments[0], *next));
                }

            }



        }//first event of segemtne then add to tree
        else if(curr.isFirst()) {

            std::cout << "---------------es first----------" << std::endl;
						//add to the tree
            st.insert(curr.segments[0]);

            encontrarPrevNext(curr.segments[0], prev, next, st);
            std::cout << "encontre" << std::endl;
            //check for intersections with neighbours


            if( prev != NULL ){
                std::cout << "--------------->>>>>>>>>>>>>>>>provar prev" << std::endl;
                if( intersect(curr.segments[0], *prev, aux) &&
                    aux.y <= Segment::sweep_lineY ) {
                        std::cout << "00000000000000000Agrege interseccion000000000000000000000" << std::endl;
                        //add intersection
                        intersections.push_back(aux);

                        //add intersection event
                        eventQueue.push(Event(aux, *prev, curr.segments[0]));

                }
            }else{ std::cout<<"es nulll prev "<<std::endl;}
            if( next != NULL ){
                std::cout << "--------------->>>>>>>>>>>>>>>>provar next" << std::endl;
                std::cout << "afuera " << (*next).p.x <<" "<<(*next).p.y << " <> " << (*next).q.x <<" "<<(*next).q.y << std::endl;
                if( intersect(curr.segments[0], *next, aux) &&
                    aux.y <= Segment::sweep_lineY ) {
                        std::cout << "000000000000000000Agrege interseccion00000000000000000000" << std::endl;
                        //add intersection
                        intersections.push_back(aux);

                        //add intersection event
                        eventQueue.push(Event(aux, curr.segments[0], *next));
                }
            }{ std::cout<<"es nulll prev "<<std::endl;}

            std::cout << "-------------------->inserte " << std::endl;
        }
        else{ //last event of segemnte then delete from tree
            //std::cout << "soy last" << std::endl;
            encontrarPrevNext(curr.segments[0], prev, next, st);
            std::cout << "es salida" << std::endl;
            /*TODO aca hay problemas al borrar aveces no exite el nodo
            siempre deberia exitir
            */
            st.erase(curr.segments[0]);
            //check intersection between prev and next new neighbours
            if( prev != NULL && next != NULL ){

                if( intersect(*prev, *next, aux) &&
                    aux.y <= Segment::sweep_lineY ){

                    intersections.push_back(aux);

                    //add intersection event
                    std::cout << "agregar evento de interseccion prev y next" << std::endl;
                    std::cout << "en "<<aux.x <<" "<<aux.y << std::endl;
                    //add intersection event
                    eventQueue.push(Event(aux, *prev, *next));
                }
            }
        }
        //st.printTree();
        std::cout << std::endl << std::endl;

				if(next != NULL) delete next;
				if(prev != NULL) delete prev;
    }

    return intersections;
}

void LineIntersection::encontrarPrevNext(Segment& s, Segment* &prev, Segment* &next, std::set<Segment>& st){
		std::cout << "---ya--" << std::endl;
		std::set<Segment>::iterator it;
    std::set<Segment>::iterator itOther;
		std::cout << "----------------------st size----------->  " << st.size() << std::endl;
		if(next != NULL) delete next;
		if(prev != NULL) delete prev;
		std::cout << "---ya--" << std::endl;
    next = NULL;
    prev = NULL;
    it = st.find(s);
    if(it == st.end()) { std::cout<<"==============>paiala find" << std::endl; return;}
    itOther = it;
    itOther++;
		std::cout << "----------------------st size----------->  " << st.size() << std::endl;
    if(itOther != st.end()){
        std::cout << "----------------------00000000000----------->encontre next" << std::endl;

        next =  new Segment(itOther->p, itOther->q);
        std::cout << (*next).p.x <<" "<<(*next).p.y << " <> " << (*next).q.x <<" "<<(*next).q.y << std::endl;
    }
    itOther = it;
    if(itOther != st.begin()){
        itOther--;
        std::cout << "-------------------0000000000000000-------------->encontre prev" << std::endl;
        prev = new Segment(itOther->p, itOther->q);
    }
}

void LineIntersection::swapOrder(std::set<Segment>& st, Segment first,
                                 Segment second, Segment* &prev, Segment* &next) {

    std::set<Segment>::iterator itf;
    std::set<Segment>::iterator itOther;
		if(next != NULL) delete next;
		if(prev != NULL) delete prev;
    next = NULL;
    prev = NULL;

    itf = st.find(first);
		//itf2 = st.erase(itf, itf);
    itOther =  itf;
    itOther++;
    if( itOther != st.end() ) {
        if( ((Segment)(*itOther)).equals(second) ){
						(*itOther).a = first.a;
						(*itOther).b = first.b;
						(*itOther).c = first.c;

						(*itOther).p = first.p;
						(*itOther).q = first.q;


            (*itf).a = second.a;
            (*itf).b = second.b;
            (*itf).c = second.c;

            (*itf).p = second.p;
            (*itf).q = second.q;
            assert(! ((Segment)*itf).equals(first));
            assert(! ((Segment)*itf).equals((Segment)*itOther));
        }else if( ((Segment)(*itOther)).equals(first) ){

						(*itOther).a = second.a;
						(*itOther).b = second.b;
						(*itOther).c = second.c;

						(*itOther).p = second.p;
						(*itOther).q = second.q;


            (*itf).a = first.a;
            (*itf).b = first.b;
            (*itf).c = first.c;

            (*itf).p = first.p;
            (*itf).q = first.q;

            assert(! ((Segment)*itf).equals(second));
            assert(! ((Segment)*itf).equals((Segment)*itOther));
        }
        itOther++;
        if(itOther != st.end()){
            next = new Segment(itOther->p, itOther->q);
        }
    }else{
        itOther =  itf;
        if( itOther != st.begin() ){
            itOther--;
            if( ((Segment)(*itOther)).equals(second) ){
                (*itOther).a = first.a;
								(*itOther).b = first.b;
								(*itOther).c = first.c;

								(*itOther).p = first.p;
								(*itOther).q = first.q;


				        (*itf).a = second.a;
				        (*itf).b = second.b;
				        (*itf).c = second.c;

				        (*itf).p = second.p;
				        (*itf).q = second.q;
                assert(! ((Segment)*itf).equals(first));
                assert(! ((Segment)*itf).equals((Segment)*itOther));
            }else if( ((Segment)(*itOther)).equals(first) ){
                (*itOther).a = second.a;
								(*itOther).b = second.b;
								(*itOther).c = second.c;

								(*itOther).p = second.p;
								(*itOther).q = second.q;


				        (*itf).a = first.a;
				        (*itf).b = first.b;
				        (*itf).c = first.c;

				        (*itf).p = first.p;
				        (*itf).q = first.q;
                assert(! ((Segment)*itf).equals(second));
                assert(! ((Segment)*itf).equals((Segment)*itOther));
            }
            if(itOther != st.begin()){
                itOther--;
                prev = new Segment(itOther->p, itOther->q);
            }
        }
    }


    /*std::cout << "--------------.>>> voy a intercabbiar" << std::endl;
    Node* nodeF = get(first);
    //TODO problema aca porque veces es null... nunca deberia ser null
    if(nodeF == NULL) return;
    Node* nodeHi = NULL;
    Node* nodeLo = NULL;
    if(nodeF->right != NULL) Node* nodeHi = min(nodeF->right);
    if(nodeF->left != NULL) Node* nodeLo = max(nodeF->left);
    if(nodeHi != NULL){
        if(nodeF->key.equals(first)){
            if(nodeHi->key.equals(second)){
                nodeF->key = second;
                nodeHi->key = first;
                return;
            }
        }else{
            if(nodeHi->key.equals(first)){
                nodeF->key = first;
                nodeHi->key = second;
                return;
            }
        }
    }

    if(nodeLo != NULL){
        if(nodeF->key.equals(first)){
            if(nodeLo->key.equals(second)){
                nodeF->key = second;
                nodeLo->key = first;
                return;
            }
        }else{
            if(nodeLo->key.equals(first)){
                nodeF->key = first;
                nodeLo->key = second;
                return;
            }
        }
    }
    //printTree();
    */
}

bool LineIntersection::intersect(const Segment& s, const Segment& t, Point& p){
		std::cout << "/////////////////////////////////////////////entre //////////////////////////////" << std::endl;
    std::cout << s.p.x <<" "<<s.p.y << " <> " << s.q.x <<" "<<s.q.y << std::endl;
    std::cout << t.p.x <<" "<<t.p.y << " <> " << t.q.x <<" "<<t.q.y << std::endl;
    if( ccw(s.p, s.q, t.p) == ccw(s.p, s.q, t.q) ||
        ccw(t.p, t.q, s.p) == ccw(t.p, t.q, s.q)    ){
            std::cout << "**********************************No intersecta" << std::endl;
        return false;
    }
    std::cout << "==================================================================si inter" << std::endl;
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
