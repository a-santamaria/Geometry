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
/*
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
  segment: x1=-4.02914, y1=-6.63292, x2=-2.48544, y2=0.861063
  segment: x1=-1.96109, y1=-1.56069, x2=-6.81248, y2=0.657612
  segment: x1=1.38761, y1=-2.78848, x2=1.67704, y2=-9.603
  segment: x1=-5.67677, y1=0.846205, x2=-5.05215, y2=5.54633
  segment: x1=5.97359, y1=-4.66666, x2=-5.70057, y2=0.60124
  segment: x1=1.69108, y1=7.90375, x2=-0.235008, y2=8.55139
  segment: x1=-6.60502, y1=-5.23483, x2=-6.50729, y2=-5.16058
  segment: x1=-0.251281, y1=-8.32868, x2=-3.199, y2=-7.2287
  segment: x1=0.00627517, y1=-9.78424, x2=-3.58423, y2=3.50728
  segment: x1=-1.22352, y1=8.21843, x2=-5.97388, y2=-2.42994



*/


    x1=-4.02914, y1=-6.63292, x2=-2.48544, y2=0.861063;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);


    x1=-1.96109, y1=-1.56069, x2=-6.81248, y2=0.657612;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);

/*

    x1=1.38761, y1=-2.78848, x2=1.67704, y2=-9.603;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);

/*
    x1=-5.67677, y1=0.846205, x2=-5.05215, y2=5.54633;
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

  x1=5.97359, y1=-4.66666, x2=-5.70057, y2=0.60124;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);

/*
    x1=1.69108, y1=7.90375, x2=-0.235008, y2=8.55139;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);

/*
    x1=-6.60502, y1=-5.23483, x2=-6.50729, y2=-5.16058;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);

/*
    x1=-0.251281, y1=-8.32868, x2=-3.199, y2=-7.2287;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);

/*
  x1=0.00627517, y1=-9.78424, x2=-3.58423, y2=3.50728;
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

    x1=-1.22352, y1=8.21843, x2=-5.97388, y2=-2.42994;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(cont++, cont++));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);




    //TODO change clock_t for the other library

    LineIntersection lineInt(points, segments);

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
