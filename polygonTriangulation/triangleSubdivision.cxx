#include "triangleSubdivision.h"

TriangleSubdivision::TriangleSubdivision() {}
TriangleSubdivision::TriangleSubdivision(std::vector<Point>& _points,
                                         std::vector<Triangle>& _polySoup)
                                         : points(_points),
                                           polySoup(_polySoup) {}

void TriangleSubdivision::subdivide() {

    Point p, q, r;
    int cont = 0;
    std::pair<int, int> key;
    int* ids;
    int id;
    for(int i = 0; i < polySoup.size(); i++) {

        std::cout << "new points size " << newPoints.size() << std::endl;
        ids = polySoup[i].idPoints;

        key.first = ids[0]; key.second = ids[1];
        if(centers.find(key) == centers.end() ) {
            p.x = (points[ids[0]].x + points[ids[1]].x) / 2.0;
            p.y = (points[ids[0]].y + points[ids[1]].y) / 2.0;
            newPoints.push_back(p);
            id = newPoints.size()-1;
            centers[key] = id;
            key.first = ids[1]; key.second = ids[0];
            centers[key] = id;
        }

        key.first = ids[1]; key.second = ids[2];
        if(centers.find(key) == centers.end() ) {
            q.x = (points[ids[1]].x + points[ids[2]].x) / 2.0;
            q.y = (points[ids[1]].y + points[ids[2]].y) / 2.0;
            newPoints.push_back(q);
            id = newPoints.size()-1;
            centers[key] = id;
            key.first = ids[2]; key.second = ids[1];
            centers[key] = id;
        }

        key.first = ids[2]; key.second = ids[0];
        if(centers.find(key) == centers.end() ) {
            r.x = (points[ids[2]].x + points[ids[0]].x) / 2.0;
            r.y = (points[ids[2]].y + points[ids[0]].y) / 2.0;
            newPoints.push_back(r);
            id = newPoints.size()-1;
            centers[key] = id;
            key.first = ids[0]; key.second = ids[2];
            centers[key] = id;
        }

    }

    int idP, idQ, idR;
    for(int i = 0; i < polySoup.size(); i++) {
        ids = polySoup[i].idPoints;

        key.first = ids[0]; key.second = ids[1];
        idP = centers[key];

        key.first = ids[1]; key.second = ids[2];
        idQ = centers[key];

        key.first = ids[2]; key.second = ids[0];
        idR = centers[key];

        newSegments.push_back( Segment(idP, idQ) );
        newSegments.push_back( Segment(idQ, idR) );
        newSegments.push_back( Segment(idR, idP) );

        newPolys.push_back( Triangle(idP, idQ, idR) );

    }
}
