#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "point.h"
#include "lineIntersection.h"

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
#include <vtkLine.h>
#include <vtkCellArray.h>
#include "vtkPolyLine.h"

using namespace std;

#define maxNum 10000


int main( int argc, char* argv[] )
{

    if(argc < 2){
        cerr<< "Usage: " << argv[0] << " num_lines" << endl;
        cerr<< "\t num_lines: number of random lkines" << endl;
        return -1;
    }

    int NumL = atoi( argv[1] );
    if(NumL < 1){
        cerr<<"num_lines most be grater than 0"<<endl;
        return -1;
    }

    srand (time(NULL));
    vector<Point> points;
    vector<Segment> segments;

    vtkSmartPointer<vtkPoints> pointsVTK =
      vtkSmartPointer<vtkPoints>::New();

    vtkSmartPointer<vtkPoints> intersectionPoints =
      vtkSmartPointer<vtkPoints>::New();

    unsigned char pointColor[3] = {255, 255, 255};

    /*vtkSmartPointer<vtkUnsignedCharArray> colors =
      vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName ("Colors");
    */
    double x1, y1, x2, y2;
    vtkCellArray *cells = vtkCellArray::New();
    cells->Initialize();
    vtkIdType id1, id2;
    vtkLine *line;
    int cont = 0;

    for(int i = 0 ; i < NumL; i++){
        randEllipse(100, 100, 0, 0, 1, x1, y1);
        points.push_back(Point(x1, y1));
        id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
        randEllipse(100, 100, 0, 0, 1, x2, y2);
        points.push_back(Point(x2, y2));
        id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
        cout<<"segment: x1="<<x1<<", y1="<<y1<<", x2="<<x2<<", y2="<<y2<<endl;
        segments.push_back( Segment(cont++, cont++) );
        line = vtkLine::New();
        line->GetPointIds()->SetId(0, id1);
        line->GetPointIds()->SetId(1, id2);

        cells->InsertNextCell(line);

        //no se si se necesita
        //colors->InsertNextTupleValue(pointColor);
    }
/*
    segment: x1=-5.24531, y1=4.25387, x2=-1.23299, y2=7.48874
    segment: x1=-5.4769, y1=4.83451, x2=-3.98637, y2=-2.48149
    segment: x1=-8.99284, y1=2.47201, x2=5.18864, y2=7.18932
    segment: x1=7.58691, y1=1.48698, x2=8.81464, y2=-2.84071
    segment: x1=-8.66381, y1=0.0600866, x2=-3.16307, y2=0.374151
    segment: x1=-0.680331, y1=-3.82604, x2=-2.88053, y2=2.4124


*/
/*

    x1=-5.24531, y1=4.25387, x2=-1.23299, y2=7.48874;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);


    x1=-5.4769, y1=4.83451, x2=-3.98637, y2=-2.48149;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);



     x1=-8.99284, y1=2.47201, x2=5.18864, y2=7.18932;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);


    x1=7.58691, y1=1.48698, x2=8.81464, y2=-2.84071;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);


    x1=-8.66381, y1=0.0600866, x2=-3.16307, y2=0.374151;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);


    x1=-0.680331, y1=-3.82604, x2=-2.88053, y2=2.4124;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);

*/


    //TODO change clock_t for the other library

    LineIntersection lineInt(points, segments);


    vector<Point> ints = lineInt.sweep_line();

    for(int i = 0; i < ints.size(); i++){
        intersectionPoints->InsertNextPoint (ints[i].x, ints[i].y, 0);
        pointsVTK->InsertNextPoint (ints[i].x, ints[i].y, 0);
    }






/** ---------------------------------------------------- **/

    vtkSmartPointer<vtkPolyData> pointsPolydata =
      vtkSmartPointer<vtkPolyData>::New();

    pointsPolydata->SetPoints(pointsVTK);

    vtkSmartPointer<vtkPolyData> pointsPolydata2 =
      vtkSmartPointer<vtkPolyData>::New();

    pointsPolydata2->SetPoints(intersectionPoints);

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
