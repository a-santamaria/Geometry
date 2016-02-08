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

double fRand( double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return f * fMax ;
}


int main( int argc, char* argv[] )
{

    if(argc < 3){
        cerr<< "Usage: " << argv[0] << " num_points name_algo" << endl;
        cerr<< "name_algo: (naive - incremental - divide)" << endl;
        return -1;
    }
    int NumP = atof( argv[ 1 ] );
    if(NumP < 2){
        cerr<<"num_ponts most be grater than 1"<<endl;
        return -1;
    }
    string algo = argv[2];
    if(algo != "naive" && algo != "incremental" && algo != "divide"){
        cerr<<"name_algo "<< algo << " not valid"<<endl;
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

    for(int i = 0 ; i < NumP; i++){
        double x = fRand(maxNum);
        double y = fRand(maxNum);
        points.push_back(Point(x, y));
        pointsVTK->InsertNextPoint (x, y, 0);

        colors->InsertNextTupleValue(pointColor);
    }

    ConvexHull convexHull(points);
    vector<Point> hull;
    if(algo == "naive")
        hull = convexHull.naive();
    if(algo == "incremental")
        hull = convexHull.incremental();
    if(algo == "divide")
        hull = convexHull.divideAndConquer();


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
