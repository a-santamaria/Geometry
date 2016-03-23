#ifndef TRIANGLE_SUBDIVISION_H
#define TRIANGLE_SUBDIVISION_H

#include <vector>
#include <map>
#include <iostream>
#include "point.h"

class TriangleSubdivision {
private:
    std::vector<Triangle> polySoup;
    std::vector<Point> points;

    std::map< std::pair<int, int>, int > centers;
public:
    std::vector<Point> newPoints;
    std::vector<Segment> newSegments;
    std::vector<Triangle> newPolys;
public:
    TriangleSubdivision();
    TriangleSubdivision(std::vector<Point>& _points,
                        std::vector<Triangle>& _polySoup);
    void subdivide();
};

#endif
