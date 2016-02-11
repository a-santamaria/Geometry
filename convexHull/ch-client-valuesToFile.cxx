#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "convex-hull.h"

using namespace std;

#define maxNum 10000


int main( int argc, char* argv[] )
{

    if(argc < 6){
        cerr<< "Usage: " << argv[0];
        cerr<< " name_algo num_points_start num_points_end delta file";
        cerr<<endl;
        cerr<< "\t name_algo: (naive - incremental - divide)" << endl;
        cerr<< "\t num_points_start: number of random points start" << endl;
        cerr<< "\t num_points_end: number of random points end" << endl;
        cerr<< "\t delta: delta between num_points" << endl;
        cerr<< "\t file: output file" << endl;
        return -1;
    }
    string algo = argv[1];
    if(algo != "naive" && algo != "incremental" && algo != "divide"){
        cerr<<"name_algo "<< algo << " not valid"<<endl;
        return -1;
    }

    int numS = atoi( argv[2] );
    int numE = atoi( argv[3] );
    if(numS < 2 || numE < 2){
        cerr<<"num_ponts most be grater than 1"<<endl;
        return -1;
    }
    if(numE <= numS){
        cerr<<"num_points_end most be bigger than num_points_start"<<endl;
        return -1;
    }

    int delta = atof(argv[4]);
    if(delta <= 0 || delta > numE - numS){
        cerr<<"delta must be bigger than 0 and lower than num_points_end - num_points_start"<<endl;
        return -1;
    }


    ofstream out(argv[5] , ios::out);

    srand (time(NULL));
    double centerX, centerY, radious1, radious2, pct;
    double x, y;
    double elapsed_secs;


    for(int i = numS ; i <= numE; i += delta){

        radious1 = randF() / (double)100000;
        radious2 = randF() / (double)100000;
        centerX = 0;
        centerY = 0;
        pct = randF();
        vector<Point> points;
        for(int j = 0 ; j < i; j++){
            randEllipse(radious1, radious2, centerX, centerY, pct, x, y);
            points.push_back(Point(x, y));
        }

        clock_t begin = clock();

        ConvexHull convexHull(points);
        vector<Point> hull;
        if(algo == "naive")
            hull = convexHull.naive();
        if(algo == "incremental")
            hull = convexHull.incremental();
        if(algo == "divide")
            hull = convexHull.divideAndConquer();

        clock_t end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        out << i << ", " << elapsed_secs << endl;
        //cout << i << ", " << elapsed_secs << endl;
    }
    return EXIT_SUCCESS;
}
