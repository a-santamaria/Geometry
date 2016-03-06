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

    /*vtkSmartPointer<vtkUnsignedCharArray> colors =
      vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName ("Colors");
    */
    double x1, y1, x2, y2;
    vtkCellArray *cells = vtkCellArray::New();
    cells->Initialize();
    int cont = 0;
    
    for(int i = 0 ; i < NumL; i++){
        randEllipse(100, 100, 0, 0, 1, x1, y1);
        points.push_back(Point(x1, y1));
        vtkIdType id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
        randEllipse(100, 100, 0, 0, 1, x2, y2);
        points.push_back(Point(x2, y2));
        vtkIdType id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
        cout<<"segment: x1="<<x1<<", y1="<<y1<<", x2="<<x2<<", y2="<<y2<<endl;
        segments.push_back( Segment(cont++, cont++) );
        vtkLine *line = vtkLine::New();
        line->GetPointIds()->SetId(0, id1);
        line->GetPointIds()->SetId(1, id2);

        cells->InsertNextCell(line);

        //no se si se necesita
        //colors->InsertNextTupleValue(pointColor);
    }


/*

    x1=89, y1=537, x2=466 , y2=194;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
    vtkIdType id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
    vtkIdType id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(0, 1));
    vtkLine *line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);

    x1=162 , y1=154, x2=447  , y2=447;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
     id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
     id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(2, 3));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);


    x1=119 , y1=489, x2=586  , y2=293;
    points.push_back(Point(x1, y1));
    points.push_back(Point(x2, y2));
     id1 = pointsVTK->InsertNextPoint (x1, y1, 0);
     id2 = pointsVTK->InsertNextPoint (x2, y2, 0);
    segments.push_back(Segment(4, 5));
    line = vtkLine::New();
    line->GetPointIds()->SetId(0, id1);
    line->GetPointIds()->SetId(1, id2);

    cells->InsertNextCell(line);
*/


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
