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
    graph.assign( _points.size(), std::vector<int>() );

    int id = 0;
    for(int i = 0; i < points.size()-1; i++){
        edges.push_back( Segment( i, i+1, id++ ) );
        typeOfVertices.push_back( typeOfVertex(i) );
        eventQueue.push( Event(i) );
        graph[i].push_back(i+1);
    }
    edges.push_back( Segment( points.size()-1, 0, id ) );
    typeOfVertices.push_back( typeOfVertex( points.size()-1 ) );
    eventQueue.push( Event(points.size()-1) );
    graph[points.size()-1].push_back(0);
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
    printGraph();
    constructPolygons();

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
        //insert diagonal pId to idHelper
        newSegments.push_back( Segment( pId, idHelper ) );
        //insert new edges in graph
        graph[pId].push_back(idHelper);
        graph[idHelper].push_back(pId);
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

    //insert diagonal pId to idHelper
    newSegments.push_back( Segment( pId, idHelper ) );
    //insert new edges in graph
    graph[pId].push_back(idHelper);
    graph[idHelper].push_back(pId);

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
        //insert diagonal pId to idHelper
        newSegments.push_back( Segment( pId, idHelper ) );
        //insert new edges in graph
        graph[pId].push_back(idHelper);
        graph[idHelper].push_back(pId);
    }
    setSegmentIterator it = segmentIterators[idPrev];
    st.erase( it );

    setSegmentIterator leftEdge = st.upper_bound(edges[pId]);

    idHelper = helper[leftEdge->id];
    if( typeOfVertices[ idHelper ] == MERGE ) {
        //insert diagonal pId to idHelper
        newSegments.push_back( Segment( pId, idHelper ) );
        //insert new edges in graph
        graph[pId].push_back(idHelper);
        graph[idHelper].push_back(pId);
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
            //insert diagonal pId to idHelper
            newSegments.push_back( Segment( pId, idHelper ) );
            //insert new edges in graph
            graph[pId].push_back(idHelper);
            graph[idHelper].push_back(pId);
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
            //insert diagonal pId to idHelper
            newSegments.push_back( Segment( pId, idHelper ) );
            //insert new edges in graph
            graph[pId].push_back(idHelper);
            graph[idHelper].push_back(pId);
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

void PolygonTriangulation::constructPolygons() {
    std::pair<int, int> key;
    std::queue< std::pair<int, int> > q;
    for(int i = 0; i < newSegments.size(); i++) {
        key.first = newSegments[i].idp;
        key.second = newSegments[i].idq;
        marcas[key] = false;
        q.push(key);
        key.first = newSegments[i].idq;
        key.second = newSegments[i].idp;
        marcas[key] = false;
        q.push(key);
    }
    std::pair<int, int> curr;
    while(!q.empty()) {
        curr = q.front();
        q.pop();

        if(!marcas[curr]) {
            marcas[curr] = true;
            std::vector<int> monoPoly = getMonotonePoly(curr.first, curr.second);
            monotonePolys.push_back( monoPoly );
        }
    }
    std::cout << "----------monotone polys ----------------" << std::endl;
    for(int i = 0; i < monotonePolys.size(); i++) {
        for(int j = 0; j < monotonePolys[i].size(); j++) {
            std::cout << monotonePolys[i][j] << " ";
        }
        std::cout<<std::endl;
    }
    std::cout << "--------------------------------------" << std::endl;
}

std::vector<int> PolygonTriangulation::getMonotonePoly(int first, int second) {
    std::vector<int> monoPoly;
    monoPoly.push_back(first);
    monoPoly.push_back(second);

    int prev = first;
    int curr = second;

    while(curr != first){

        //get the most left turn
        int lowest = INF;
        int lowestId = -1;
        std::cout << "menor " << lowest  << std::endl;
        for(int i = 0; i < graph[curr].size(); i++){
            int currN = graph[curr][i];
            if (currN == prev) continue;
            int turn = getTurn(points[prev], points[curr], points[currN]);
            std::cout << prev << " " << curr << " " << currN << std::endl;
            std::cout << "trun " << turn << std::endl;
            if(turn < lowest) {
                lowest = turn;
                lowestId = currN;
            }
        }
        //marcar
        std::pair<int, int> key(curr, lowestId);
        if(marcas.find(key) != marcas.end()) {
            marcas[key] = true;
        }
        monoPoly.push_back(lowestId);
        prev = curr;
        curr = lowestId;
    }
    return monoPoly;
}

void PolygonTriangulation::printGraph() {
    std::cout << "------------graph------------" << std::endl;
    std::cout << "size " << graph.size() << std::endl;
    for(int i = 0; i < graph.size(); i++){
        std::cout << i << ": " ;
        for(int j = 0; j < graph[i].size(); j++){
            std::cout << graph[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "----------------------------" << std::endl;
}
