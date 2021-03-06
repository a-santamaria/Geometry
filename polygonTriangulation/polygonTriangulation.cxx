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

MonotoneEvent::MonotoneEvent() {}

MonotoneEvent::MonotoneEvent( int _pId, int _pos ) : pId(_pId), pos(_pos) {}

bool MonotoneEvent::operator< (const MonotoneEvent& other) const {
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
        graph[i+1].push_back(i);
    }
    edges.push_back( Segment( points.size()-1, 0, id ) );
    typeOfVertices.push_back( typeOfVertex( points.size()-1 ) );
    eventQueue.push( Event(points.size()-1) );
    graph[points.size()-1].push_back(0);
    graph[0].push_back(points.size()-1);
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
    trinagulateMonotonePolys();
}


void PolygonTriangulation::handleStart( Event& eCurr ) {
    int pId = eCurr.pId;
    std::pair<setSegmentIterator, bool> resp = st.insert( edges[pId] );
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
    /*
    std::cout << "----------monotone polys ----------------" << std::endl;
    for(int i = 0; i < monotonePolys.size(); i++) {
        for(int j = 0; j < monotonePolys[i].size(); j++) {
            std::cout << monotonePolys[i][j] << " ";
        }
        std::cout<<std::endl;
    }
    std::cout << "--------------------------------------" << std::endl;
    */
}

std::vector<int> PolygonTriangulation::getMonotonePoly(int first, int second) {
    std::vector<int> monoPoly;
    monoPoly.push_back(first);
    monoPoly.push_back(second);

    int prev = first;
    int curr = second;

    while(curr != first){
        //get the most left turn
        double greatest = 0;
        int greatestId = -1;
        bool right = true;
        for(int i = 0; i < graph[curr].size(); i++){
            int currN = graph[curr][i];
            if (currN == prev) continue;
            double alpha = getAngle(points[prev], points[curr], points[currN]);
            bool counterClock = ccw(points[prev], points[curr], points[currN]);
            if(right == true) {
                if(!counterClock) {
                    if(alpha > greatest) {
                        greatest = alpha;
                        greatestId = currN;
                    }
                } else {
                    greatest = alpha;
                    greatestId = currN;
                    right = false;
                }
            } else {
                if(counterClock) {
                    if(alpha < greatest) {
                        greatest = alpha;
                        greatestId = currN;
                    }
                }
            }
        }
        //marcar
        std::pair<int, int> key(curr, greatestId);
        if(marcas.find(key) != marcas.end()) {
            marcas[key] = true;
        }
        monoPoly.push_back(greatestId);
        prev = curr;
        curr = greatestId;
    }
    return monoPoly;
}

void PolygonTriangulation::trinagulateMonotonePolys() {

    if(monotonePolys.size() == 0) {
        std::vector<int> poly;
        for(int i = 0; i < points.size(); i++){
            poly.push_back(i);
        }
        poly.push_back(0);
        trinagulateMonotonePoly(poly);
    }
    for(int i = 0; i < monotonePolys.size(); i++){
        trinagulateMonotonePoly(monotonePolys[i]);
    }
}

void PolygonTriangulation::trinagulateMonotonePoly(std::vector<int>& poly) {
    std::priority_queue<MonotoneEvent> triangulateQueue;
    for(int i = 0; i < poly.size()-1; i++) {
        triangulateQueue.push(MonotoneEvent(poly[i], i));
    }

    MonotoneEvent top1 = triangulateQueue.top();
    triangulateQueue.pop();
    MonotoneEvent top2 = triangulateQueue.top();
    triangulateQueue.pop();
    std::stack< std::pair<int, int> > s;
    //TODO change to LEFT & RIGHT
    std::stack<int> chain;  // 1 = left | -1 = right
    int currChain;
    int savedChain;
    s.push( std::make_pair<int, int>(top1.pId, top1.pos) );
    s.push( std::make_pair<int, int>(top2.pId, top2.pos) );

    if( abs(top1.pos - top2.pos) == 1 ) {
        if (top1.pos > top2.pos) {
            chain.push(1);
            chain.push(-1);
        } else {
            chain.push(-1);
            chain.push(1);
        }
    } else {
        if(top1.pos == 0) {
            chain.push(1);
            chain.push(-1);
        } else {
            chain.push(-1);
            chain.push(1);
        }
    }

    MonotoneEvent curr;
    std::pair<int, int> top;

    for(int i = 2; i < poly.size()-2; i++) {
        curr = triangulateQueue.top();
        triangulateQueue.pop();
        //same chain
        if( (abs(curr.pos - s.top().second) == 1) ||
            (curr.pos == 0       && s.top().second == poly.size()-2) ||
            (s.top().second == 0 && curr.pos == poly.size()-2) ){
                top = s.top();
                s.pop();
                savedChain = currChain = chain.top();
                chain.pop();
                while(!s.empty()) {
                    bool sePuede = true;
                    if(currChain == -1) { //right chain
                        if ( !ccw(points[curr.pId],
                                 points[top.first],
                                 points[s.top().first]) ) {
                            sePuede = false;
                        }
                    } else { //left chain
                        if ( ccw(points[curr.pId],
                                 points[top.first],
                                 points[s.top().first]) ) {
                            sePuede = false;
                        }
                    }
                    if(!sePuede) break;

                    newSegments.push_back(Segment(curr.pId, s.top().first));
                    if(currChain == -1) {
                        polySoup.push_back(
                            Triangle(curr.pId, top.first, s.top().first) );
                    } else {
                        polySoup.push_back(
                            Triangle(curr.pId, s.top().first,  top.first) );
                    }

                    top = s.top();
                    s.pop();
                    savedChain = chain.top();
                    chain.pop();

                }
                s.push( top );
                chain.push(savedChain);
                s.push( std::make_pair<int, int>(curr.pId, curr.pos) );
                chain.push(currChain);

        } else {//different chains
            //insert diagonals to stack ponints excepto to last one
            //save top
            savedChain = chain.top();
            top = s.top();
            while(s.size() > 1) {
                newSegments.push_back( Segment(curr.pId, s.top().first) );
                int prev = s.top().first;
                s.pop();
                if(savedChain == -1) {
                    polySoup.push_back(
                                Triangle(curr.pId, prev, s.top().first) );
                } else {
                    polySoup.push_back(
                                Triangle(curr.pId, s.top().first, prev) );
                }
                chain.pop();

            }
            s.pop();
            chain.pop();

            s.push( top );
            chain.push(savedChain);
            s.push( std::make_pair<int, int>(curr.pId, curr.pos) );
            chain.push(savedChain * -1);

        }
    }
    curr = triangulateQueue.top();
    triangulateQueue.pop();
    s.pop();
    if(s.size() == 1) {
        int posPrev = curr.pos - 1;
        if(posPrev == -1) posPrev = poly.size()-1;
        int posNext = (curr.pos + 1) % poly.size();
        polySoup.push_back( Triangle(poly[posPrev], curr.pId, poly[posNext]) );
    } else {
        while( s.size() > 1 ) {
            newSegments.push_back(Segment(curr.pId, s.top().first));
            int prev = s.top().first;
            s.pop();
            polySoup.push_back( Triangle(curr.pId, s.top().first, prev) );
        }
        //TODO me falta un triangulo acá (no se cual)
        //polySoup.push_back( Triangle(curr.pId, s.top().first, prev) );
    }
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
