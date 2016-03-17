#include "polygonTriangulation.h"
#include <cassert>

double PolygonTriangulation::sweep_lineY = INF;
std::vector< Point > PolygonTriangulation::points;

Event::Event() {}

Event::Event( int _pId ) : pId(_pId) {}

bool Event::operator< (const Event& other) const {
    if( PolygonTriangulation::points[pId].y == PolygonTriangulation::points[other.pId].y )
        return PolygonTriangulation::points[pId].x > PolygonTriangulation::points[other.pId].x;
    return PolygonTriangulation::points[pId].y < PolygonTriangulation::points[other.pId].y;
}

PolygonTriangulation::PolygonTriangulation() {}

PolygonTriangulation::Type PolygonTriangulation::typeOfVertex( int id ){
    int prev = id-1;
    if( prev == -1 ) prev = points.size()-1;
    int next = (id+1) % points.size();

    if( points[prev].y < points[id].y && points[next].y < points[id].y ) {
        if( ccw( points[prev], points[id], points[next] ) ){
            return START;
        }
        return SPLIT;

    }
    if ( points[prev].y > points[id].y && points[next].y > points[id].y ) {
        if( ccw( points[prev], points[id], points[next] ) ){
            return END;
        }
        return MERGE;
    }

    return REGULAR;
}

PolygonTriangulation::PolygonTriangulation( std::vector<Point> _points ) {
    points = _points;

    int id = 0;
    for(int i = 0; i < points.size()-1; i++){
        edges.push_back( Segment( i, i+1, id++ ) );
        edgesList.push_back(i);
        typeOfVertices.push_back( typeOfVertex(i) );
        eventQueue.push( Event(i) );
    }
    edges.push_back( Segment( points.size()-1, 0, id ) );
    edgesList.push_back( points.size()-1 );
    typeOfVertices.push_back( typeOfVertex( points.size()-1 ) );
    eventQueue.push( Event(points.size()-1) );

    //make it cyclic
    edgesList.tail->next = edgesList.head;
    edgesList.head->prev = edgesList.tail;
}


double PolygonTriangulation::SegmentComparator::
                getXInSweepLine(const Segment& s) const {
    //TODO if it is horizontal or vertical
    double dx = ( points[s.idq].x - points[s.idp].x );
    double dy = ( points[s.idq].y - points[s.idp].y );
    double m = dx / dy;
    return points[s.idp].x - (m * ( points[s.idp].y - sweep_lineY ) );
}

bool PolygonTriangulation::SegmentComparator::
            operator()(const Segment& a, const Segment& b) const {
    return getXInSweepLine(a) > getXInSweepLine(b);
}

void PolygonTriangulation::triangulate() {

    while( !eventQueue.empty() ) {
        Event eCurr = eventQueue.top();
        eventQueue.pop();

        int pId = eCurr.pId;
        Point pCurr = points[pId];

        sweep_lineY = pCurr.y;

        std::cout << pCurr << " type " << typeOfVertices[pId] << std::endl;

        if( typeOfVertices[pId] == START ){
            handleStart( eCurr );
        } else if ( typeOfVertices[pId] == END ) {
            handleEnd( eCurr );
        } else if ( typeOfVertices[pId] == SPLIT ) {
            handleSplit( eCurr );
        } else if ( typeOfVertices[pId] == MERGE ) {
            handleMerge( eCurr );
        } else if ( typeOfVertices[pId] == REGULAR ) {
            handleRegular( eCurr );
        }


    }

}


void PolygonTriangulation::handleStart( Event& eCurr ) {
    int pId = eCurr.pId;
    std::pair<setSegmentIterator, bool> resp = st.insert( edges[pId] );
    std::cout << "meti " << pId << std::endl;
    segmentIterators[pId] = resp.first;
    helper[pId] = pId;
}

void PolygonTriangulation::handleEnd( Event& eCurr ) {
    int pId = eCurr.pId;
    Point pCurr = points[pId];
    int idPrev = pId - 1;
    if( idPrev == -1 ) idPrev = points.size()-1;

    assert(helper.find(idPrev) != helper.end());
    int idHelper = helper[idPrev];
    if( typeOfVertices[ idHelper ] == MERGE ) {
        //TODO insert diagonal pId to idHelper
        newSegments.push_back( Segment( pId, idHelper ) );
    }
    setSegmentIterator it = segmentIterators[idPrev];
    st.erase( it );
}

void PolygonTriangulation::handleSplit( Event& eCurr ) {
    int pId = eCurr.pId;
    Point pCurr = points[pId];
    setSegmentIterator leftEdge = st.upper_bound(edges[pId]);

    assert(helper.find(leftEdge->id) != helper.end());
    int idHelper = helper[leftEdge->id];
    //TODO insert diagonal pId to idHelper
    newSegments.push_back( Segment( pId, idHelper ) );

    //change helper of left edge
    helper[leftEdge->id] = pId;

    std::pair<setSegmentIterator, bool> resp = st.insert( edges[pId] );
    std::cout << "meti " << pId << std::endl;
    segmentIterators[pId] = resp.first;
    helper[pId] = pId;
}

void PolygonTriangulation::handleMerge( Event& eCurr ) {
    int pId = eCurr.pId;
    Point pCurr = points[pId];
    int idPrev = pId - 1;
    if( idPrev == -1 ) idPrev = points.size()-1;

    int idHelper = helper[idPrev];
    if( typeOfVertices[ idHelper ] == MERGE ) {
        //TODO insert diagonal pId to idHelper
        newSegments.push_back( Segment( pId, idHelper ) );
    }
    setSegmentIterator it = segmentIterators[idPrev];
    st.erase( it );

    setSegmentIterator leftEdge = st.upper_bound(edges[pId]);

    idHelper = helper[leftEdge->id];
    if( typeOfVertices[ idHelper ] == MERGE ) {
        //TODO insert diagonal pId to idHelper
        newSegments.push_back( Segment( pId, idHelper ) );
    }

    //change helper of left edge
    helper[leftEdge->id] = pId;

}

void PolygonTriangulation::handleRegular( Event& eCurr ) {
    int pId = eCurr.pId;
    Point pCurr = points[pId];
    int idPrev = pId - 1;
    if( idPrev == -1 ) idPrev = points.size()-1;

    int idHelper = helper[idPrev];
    if( regionToRight(pId) ) {
        if( typeOfVertices[idHelper] == MERGE ) {
            //TODO insert diagonal pId to idHelper
            newSegments.push_back( Segment( pId, idHelper ) );
        }
        setSegmentIterator it = segmentIterators[idPrev];
        st.erase( it );

        std::pair<setSegmentIterator, bool> resp = st.insert( edges[pId] );
        std::cout << "meti " << pId << std::endl;
        segmentIterators[pId] = resp.first;
        helper[pId] = pId;
    } else {
        setSegmentIterator leftEdge = st.upper_bound(edges[pId]);

        idHelper = helper[leftEdge->id];
        if( typeOfVertices[ idHelper ] == MERGE ) {
            //TODO insert diagonal pId to idHelper
            newSegments.push_back( Segment( pId, idHelper ) );
        }

        //change helper of left edge
        helper[leftEdge->id] = pId;
    }

}

bool PolygonTriangulation::regionToRight ( int id ) {
    int prev = id-1;
    if( prev == -1 ) prev = points.size()-1;
    int next = (id+1) % points.size();

    if( points[prev].y > points[id].y ) return true;
    else                                return false;
}
