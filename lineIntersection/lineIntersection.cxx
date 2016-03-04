#include "lineIntersection.h"
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <set>

Event::Event(Point _key, Segment _segment) : key(_key), tipo(Event::START) {
    segment = _segment;
}

Event::Event(Point _key, int _tipo) : tipo(_tipo) { }

bool Event::isFirst(){
    return tipo == Event::START;
}

bool Event::operator< (const Event& other) const{
    if(key.y == other.key.y) return key.x < other.key.x;
    return key.y < other.key.y;
}


std::vector<Point> LineIntersection::sweep_line(){
    std::vector<Point> intersections;


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
