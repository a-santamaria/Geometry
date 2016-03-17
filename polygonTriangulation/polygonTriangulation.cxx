#include "polygonTriangulation.h"

Event::Event() {}

Event::Event( int _pId, const PolygonTriangulation* _pt )
                                    : pId(_pId), pt(_pt) {}

bool Event::operator< (const Event& other) const {
    if( pt->points[pId].y == pt->points[other.pId].y )
        return pt->points[pId].x > pt->points[other.pId].x;
    return pt->points[pId].y < pt->points[other.pId].y;
}

PolygonTriangulation::PolygonTriangulation() {}

PolygonTriangulation::PolygonTriangulation( std::vector<Point> _points ) {
    points = _points;

    for(int i = 0; i < points.size()-1; i++){
        edges.push_back( Segment( i, i+1 ) );
        eventQueue.push( Event(i, this) );
    }
    edges.push_back( Segment( points.size()-1, 0 ) );
}

void PolygonTriangulation::triangulate() {

    while( !eventQueue.empty() ) {
        Event eCurr = eventQueue.top();
        eventQueue.pop();

        int pId = eCurr.pId;
        Point pCurr = points[pId];

        std::cout << pCurr << std::endl;

        int pPrev = pId - 1;
        if( pPrev == -1 ) pPrev = points.size()-1;
        int pNext = (pId + 1) % points.size();

        


    }

}
