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
#include <vtkPolyLine.h>
#include <vtkChartXY.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkPen.h>
#include <ctime>

using namespace std;

#define maxNum 10000

double fRand2( double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return f * fMax ;
}


int main2( int argc, char* argv[] )
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

        // Create a table with some points in it
      vtkSmartPointer<vtkTable> table =
        vtkSmartPointer<vtkTable>::New();

      vtkSmartPointer<vtkFloatArray> arrX =
        vtkSmartPointer<vtkFloatArray>::New();
      arrX->SetName("X Axis");
      table->AddColumn(arrX);

      vtkSmartPointer<vtkFloatArray> t =
       vtkSmartPointer<vtkFloatArray>::New();
     t->SetName("time");
     table->AddColumn(t);


      // Fill in the table with some example values

      table->SetNumberOfRows(NumP);
      for (int i = 2; i < NumP; ++i)
      {
          vector<Point> points;

          for(int j = 0 ; j < i; j++){
              double x = fRand2(maxNum);
              double y = fRand2(maxNum);
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

          double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

        //x scale
        table->SetValue(i, 0, i);

        //time line
        table->SetValue(i, 1, elapsed_secs);
      }

      // Set up the view
      vtkSmartPointer<vtkContextView> view =
        vtkSmartPointer<vtkContextView>::New();
      view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);

      // Add multiple line plots, setting the colors etc
      vtkSmartPointer<vtkChartXY> chart =
        vtkSmartPointer<vtkChartXY>::New();
      view->GetScene()->AddItem(chart);
      vtkPlot *line = chart->AddPlot(vtkChart::LINE);

      line->SetInputData(table, 0, 1);

      line->SetColor(0, 255, 0, 255);
      line->SetWidth(1.0);
      line = chart->AddPlot(vtkChart::LINE);
/*
      line->SetInputData(table, 0, 2);

      line->SetColor(255, 0, 0, 255);
      line->SetWidth(5.0);
*/

      //view->GetRenderWindow()->SetMultiSamples(0);

      // Start interactor
      view->GetInteractor()->Initialize();
      view->GetInteractor()->Start();

    return EXIT_SUCCESS;
}
