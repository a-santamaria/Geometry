#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "convex-hull.h"

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkProperty.h>
#include "vtkPolyLine.h"

using namespace std;

#define maxNum 10000


int main( int argc, char* argv[] )
{

    if(argc < 8){
        cerr<< "Usage: " << argv[0];
        cerr<< " name_algo num_points centerX centerY radious1 radious2 pct";
        cerr<<endl;
        cerr<< "\t name_algo: (naive - incremental - divide)" << endl;
        cerr<< "\t num_points: number of random points" << endl;
        cerr<< "\t centerX: center in x" << endl;
        cerr<< "\t centerY: center in y" << endl;
        cerr<< "\t radious1: radious in first axis" << endl;
        cerr<< "\t radious2: radious in second axis" << endl;
        cerr<< "\t pct: porcentage of rotation from 0 to 1" << endl;
        return -1;
    }
    string algo = argv[1];
    if(algo != "naive" && algo != "incremental" && algo != "divide"){
        cerr<<"name_algo "<< algo << " not valid"<<endl;
        return -1;
    }

    int NumP = atoi( argv[2] );
    if(NumP < 2){
        cerr<<"num_ponts most be grater than 1"<<endl;
        return -1;
    }

    double centerX = atof(argv[3]);
    double centerY = atof(argv[4]);

    double radious1 = atof(argv[5]);
    double radious2 = atof(argv[6]);
    if(radious1 <= 0 || radious2 <= 0){
        cerr<<"ellipse radii must be grater than 0"<<endl;
        return -1;
    }

    double pct = atof(argv[7]);
    if(pct < 0 || pct > 1){
        cerr<<"pct most be between 0 and 1"<<endl;
        return -1;
    }

    srand (time(NULL));
    vector<Point> points;

    vtkSmartPointer<vtkPoints> pointsVTK =
      vtkSmartPointer<vtkPoints>::New();

    unsigned char pointColor[3] = {255, 255, 255};

    vtkSmartPointer<vtkUnsignedCharArray> colors =
      vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName ("Colors");

    double x, y;
    for(int i = 0 ; i < NumP; i++){
        randEllipse(radious1, radious2, centerX, centerY, pct, x, y);
        points.push_back(Point(x, y));
        pointsVTK->InsertNextPoint (x, y, 0);

        colors->InsertNextTupleValue(pointColor);
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
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    cout << "time: " << elapsed_secs << endl;

    vtkSmartPointer<vtkPolyLine> xPolyLine =
      vtkSmartPointer<vtkPolyLine>::New();
    xPolyLine->GetPointIds()->SetNumberOfIds(hull.size());

    for(int i = 0; i < hull.size(); i++)
    {
        vtkIdType id = pointsVTK->InsertNextPoint(hull[i].x, hull[i].y, 0);
        xPolyLine->GetPointIds()->SetId(i,id);
    }

    vtkSmartPointer<vtkCellArray> cells =
      vtkSmartPointer<vtkCellArray>::New();
    cells->InsertNextCell(xPolyLine);

    vtkSmartPointer<vtkPolyData> pointsPolydata =
      vtkSmartPointer<vtkPolyData>::New();

    pointsPolydata->SetPoints(pointsVTK);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter =
      vtkSmartPointer<vtkVertexGlyphFilter>::New();

    vertexFilter->SetInputData(pointsPolydata);
    vertexFilter->Update();

    vtkSmartPointer<vtkPolyData> polyData =
      vtkSmartPointer<vtkPolyData>::New();
    polyData->ShallowCopy(vertexFilter->GetOutput());

    //polyData->GetPointData()->SetScalars(colors);
    polyData->SetLines(cells);

    // Visualization
    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyData);

    vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetPointSize(5);

    vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();

    vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();

    renderWindow->AddRenderer(renderer);
    renderWindow->SetSize( 600, 600 );

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();

    renderWindowInteractor->SetRenderWindow(renderWindow);

    renderer->AddActor(actor);

    renderWindow->Render();
    renderWindowInteractor->Start();

    return EXIT_SUCCESS;
}
