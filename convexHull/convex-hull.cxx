#include "convex-hull.h"
#include <algorithm>
#include <iostream>
#include <cfloat>
#include <cmath>

using namespace std;

bool Point::operator< (const Point &other) const{
    return x < other.x;
}

double Vec::cross(Vec b){
    return this->x * b.y - this->y * b.x;
}

Vec toVec(Point p, Point q){
    return Vec(q.x - p.x, q.y - p.y);
}

bool ccw(Point p, Point q, Point r){
    return toVec(q, p).cross( toVec(q, r) ) < 0;
}

bool collinear(Point p, Point q, Point r){
    return toVec(q, p).cross( toVec(q, r) ) == 0;
}

double dist(Point p, Point q){
    double x = q.x-p.x;
    double y = q.y-p.y;
    return sqrt(x*x + y*y);
}

bool CompByAngleStruct::operator() (const Point& p, const Point& q){

    if(collinear(piv, p, q)){
        return dist(piv, p) < dist(piv, q);
    }

    return ccw(p, q, piv);
}

vector<Point> ConvexHull::naive(){

    vector<Point> hull;
    for(int i = 0; i < points.size(); i++){
        for(int j = 0; j < points.size(); j++){
            if(i != j){
                bool valid = true;
                for(int k = 0; k < points.size(); k++){
                    if(k != j && k != i)
                        if(ccw(points[i], points[j], points[k])){
                            valid = false;
                            break;
                        }
                }
                if(valid){
                    hull.push_back(Point(points[i]));
                }
            }
        }
    }

    sort(hull.begin()+1, hull.end(), CompByAngleStruct(hull[0]));

    hull.push_back(hull[0]);
    return hull;
}

vector<Point> ConvexHull::incremental(){

    vector<Point> hull;
    sort(this->points.begin(), this->points.end());

    //upper hull
    hull.push_back(points[0]);
    hull.push_back(points[1]);

    for(int i = 2; i < points.size(); i++){
        int j = hull.size()-1;
        //check counter clock wise turn (left)
        while( hull.size() > 1 && ccw(hull[j-1], hull[j], points[i]) ){
            hull.pop_back();
            j = hull.size()-1;
        }
        hull.push_back(points[i]);
    }

    //lower hull
    hull.push_back(points[points.size()-2]);

    for(int i = points.size()-3; i >= 0; i--){
        int j = hull.size()-1;
        //check counter clock wise turn (left)
        while( hull.size() > 1 && ccw(hull[j-1], hull[j], points[i]) ){
            hull.pop_back();
            j = hull.size()-1;
        }
        hull.push_back(points[i]);
    }

    return hull;
}

vector<Point> ConvexHull::divideAndConquer(){

    sort(this->points.begin(), this->points.end());

    int lo = 0;
    int hi = points.size()-1;
    int mid = (lo + hi)/2;
    vector<Point> lh = divideAndConquer(lo, mid);
    vector<Point> rh = divideAndConquer(mid+1, hi);
    vector<Point> convexHull = mergeTangent(lh, rh);
    convexHull.push_back(convexHull[0]);

    return convexHull;

}

vector<Point> ConvexHull::divideAndConquer(int lo, int hi){

    std::vector<Point> hull;
    if(hi - lo == 0){

        hull.push_back(points[lo]);
        return hull;
    }
    if(hi - lo == 1){

            hull.push_back(points[lo]);

            hull.push_back(points[hi]);
            return hull;
    }
    if(hi - lo == 2){
        //brute force
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++){
                if(i != j)
                {
                    int k = 3 - j - i;

                    if(!ccw(points[lo+i], points[lo+j], points[lo+k])){
                        hull.push_back(points[lo+i]);
                        hull.push_back(points[lo+j]);
                        hull.push_back(points[lo+k]);

                        return hull;
                    }
                }
            }
    }

    int mid = (lo + hi)/2;
    vector<Point> lh = divideAndConquer(lo, mid);
    vector<Point> rh = divideAndConquer(mid+1, hi);
    return mergeTangent(lh, rh);

}

vector<Point> ConvexHull::mergeTangent(vector<Point>& lh, vector<Point>& rh){

    vector<Point> hull;
    int midL, midR, startL, startR, endL, endR;

    //find rightmost point left hull lh
    int indexL;
    double maxL = 0;

    for(int i = 0; i < lh.size(); i++)
        if(lh[i].x > maxL){
            maxL = lh[i].x;
            midL = i;
        }

    //find leftmost point right hull rh
    int indexR;
    double minR = DBL_MAX;

    for(int i = 0; i < rh.size(); i++)
        if(rh[i].x < minR){
            minR = rh[i].x;
            midR = i;
        }

    Point l1, l2, r1, r2;
    //lower tangent
    indexL =  midL;
    indexR = midR;

    l1 = lh[indexL];
    l2 = lh[ (indexL+1) % lh.size() ];
    r1 = rh[indexR];

    if(indexR-1 < 0)
        r2 = rh[ rh.size() -1 ];
    else
        r2 = rh[ (indexR-1) ];

    while( ccw(r1, l1, l2) || ccw(r2, r1, l1) ){
        //not tanget for left hull
        while( ccw(r1, l1, l2) ){
            indexL++;
            indexL %= lh.size();
            l1 = lh[indexL];
            l2 = lh[ (indexL+1) % lh.size() ];
        }
        //not tanget for right hull
        while( ccw(r2, r1, l1) ){
            indexR--;
            if(indexR < 0)
                indexR = rh.size() -1;
            r1 = rh[indexR];
            if(indexR-1 < 0)
                r2 = rh[ rh.size() -1 ];
            else
                r2 = rh[ (indexR-1) ];
        }
    }
    startL = indexL;
    endR = indexR;

    //upper tangent
    indexL =  midL;
    indexR = midR;

    l1 = lh[indexL];
    if(indexL-1 < 0)
        l2 = lh[ lh.size() -1 ];
    else
        l2 = lh[ (indexL-1) ];
    r1 = rh[indexR];
    r2 = rh[ (indexR+1) % rh.size() ];

    while( ccw(l2, l1, r1) || ccw(l1, r1, r2) ){
        //not tanget for left hull
        while( ccw(l2, l1, r1) ){
            indexL--;
            if(indexL < 0)
                indexL = lh.size() -1;
            l1 = lh[indexL];
            if(indexL-1 < 0)
                l2 = lh[ lh.size() -1 ];
            else
                l2 = lh[ (indexL-1) ];
        }
        //not tanget for right hull
        while( ccw(l1, r1, r2) ){
            indexR++;
            indexR %= rh.size();
            r1 = rh[indexR];
            r2 = rh[ (indexR+1) % rh.size() ];
        }
    }
    endL = indexL;
    startR = indexR;

    //copy left side
    for(int i = startL; i != endL; i = (i+1)%lh.size()){
        hull.push_back(lh[i]);
    }
    hull.push_back(lh[endL]);
    //copy right side
    for(int i = startR; i != endR; i = (i+1)%rh.size()){
        hull.push_back(rh[i]);
    }
    hull.push_back(rh[endR]);

    return hull;
}
