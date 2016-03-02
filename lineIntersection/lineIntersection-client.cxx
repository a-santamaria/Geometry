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
#include "redBlackBST.h"

using namespace std;

#define maxNum 10000


int main( int argc, char* argv[] )
{
    /*RedBlackBST st;

    for(int i = 0; i < 10; i++){
        double r = randF();
        cout<<r<<endl;
        Segment* s = new Segment();
        st.put(randF(), s);
    }*/

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

    vtkSmartPointer<vtkUnsignedCharArray> colors =
      vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName ("Colors");

    double x1, y1, x2, y2;
    vtkCellArray *cells = vtkCellArray::New();
    cells->Initialize();

    for(int i = 0 ; i < NumL; i++){
        randEllipse(100, 100, 0, 0, 1, x1, y1);
        points.push_back(Point(x1, y1));
        vtkIdType id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
        randEllipse(100, 100, 0, 0, 1, x2, y2);
        points.push_back(Point(x2, y2));
        vtkIdType id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
        cout<<"segment: x1="<<x1<<", y1="<<y1<<", x2="<<x2<<", y2="<<y2<<endl;
        segments.push_back( Segment(Point(x1, y1), Point(x2, y2)) );
        vtkLine *line = vtkLine::New();
        line->GetPointIds()->SetId(0, id1);
        line->GetPointIds()->SetId(1, id2);

        cells->InsertNextCell(line);

        //no se si se necesita
        colors->InsertNextTupleValue(pointColor);
    }
    /*
    segment: 2.14418 -0.590303  -1.78906 -0.103435
    segment: -0.176729 -1.87436  -0.179778 1.45793
    segment: -0.771935 1.72359  1.89495 1.70407
    segment: -1.63018 -1.36066  1.83392 0.00875436
    segment: 0.175602 1.11141  -1.20948 1.13638
    segment: -1.72962 -1.72707  2.87497 0.111391
    */
    /*
    segment: -0.718467 9.88359    0.632485 -5.86345
    segment: -6.57177 2.92941    -0.364522 2.14119
    segment: 1.56338 -4.17783    -1.9288 -9.00629
    segment: 1.94885 8.15409    -8.84786 -2.81477
    segment: 6.13274 -4.1912    1.00038 -5.64432
    segment: 0.0602298 -2.80132    0.327202 4.5382
*/
/*
    segment: x1=6.28214, y1=0.156946, x2=1.28, y2=-5.85895
    segment: x1=6.49534, y1=5.09243, x2=0.263176, y2=-6.78202
    segment: x1=-1.04893, y1=7.11772, x2=0.601029, y2=7.46132
    segment: x1=4.56062, y1=-7.7804, x2=-5.08734, y2=-2.28247
    segment: x1=-6.79473, y1=4.68961, x2=8.64915, y2=-1.05621
    segment: x1=7.86563, y1=-0.173118, x2=-2.88761, y2=4.95895
*/
/*
    x1=6.28214, y1=0.156946, x2=1.28, y2=-5.85895;
    vtkIdType id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    vtkIdType id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(Point(x1, y1), Point(x2, y2)));
    vtkLine *line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);


    x1=6.49534, y1=5.09243, x2=0.263176, y2=-6.78202;
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(Point(x1, y1), Point(x2, y2)));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);


    x1=-1.04893, y1=7.11772, x2=0.601029, y2=7.46132;
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(Point(x1, y1), Point(x2, y2)));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);


    x1=4.56062, y1=-7.7804, x2=-5.08734, y2=-2.28247;
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(Point(x1, y1), Point(x2, y2)));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);


    x1=-6.79473, y1=4.68961, x2=8.64915, y2=-1.05621;
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(Point(x1, y1), Point(x2, y2)));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);


    x1=7.86563, y1=-0.173118, x2=-2.88761, y2=4.95895;
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(Point(x1, y1), Point(x2, y2)));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);
*/

    //TODO change clock_t for the other library

    LineIntersection lineInt(segments);

    clock_t begin = clock();

    vector<Point> ints = lineInt.sweep_line();

    for(int i = 0; i < ints.size(); i++){
        intersectionPoints->InsertNextPoint (ints[i].x, ints[i].y, 0);
        pointsVTK->InsertNextPoint (ints[i].x, ints[i].y, 0);
    }

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    cout << "time: " << elapsed_secs << endl;




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
