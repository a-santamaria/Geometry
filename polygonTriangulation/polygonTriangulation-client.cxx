#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkPointPicker.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkRendererCollection.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkLine.h>
#include <vtkProperty.h>
#include <vtkCellArray.h>
#include <vector>
#include "point.h"
#include "polygonTriangulation.h"


// Define interaction style
class customMouseInteractorStyle : public vtkInteractorStyleTrackballCamera
{
  public:
    std::vector<Point> points;

    static customMouseInteractorStyle* New();
    vtkTypeMacro(customMouseInteractorStyle, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown()
    {
      std::cout << "Pressed left mouse button." << std::endl;
      // Forward events
      vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
      int pos[2];
      this->Interactor->GetEventPosition(pos[0], pos[1]);
      std::cout << "Picked value: " << pos[0] << " " << pos[1] << std::endl;
      points.push_back(Point(pos[0], pos[1]));
      // Forward events
      vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    virtual void OnRightButtonDown()
    {
      //TODO crear edges y update window
    }

};

vtkStandardNewMacro(customMouseInteractorStyle);

int main(int, char *[])
{

  vtkSmartPointer<vtkPoints> pointsVTK =
        vtkSmartPointer<vtkPoints>::New();

  vtkSmartPointer<vtkPolyData> pointsPolydata =
     vtkSmartPointer<vtkPolyData>::New();

   pointsPolydata->SetPoints(pointsVTK);

    vtkSmartPointer<vtkPolyData> polyData =
      vtkSmartPointer<vtkPolyData>::New();


    vtkCellArray *cells = vtkCellArray::New();
      cells->Initialize();

    polyData->SetLines(cells);




  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(polyData);

  vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  //renderer->SetBackground(1,1,1); // Background color white
  renderer->AddActor(actor);


  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize( 600, 600 );

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow ( renderWindow );

  vtkSmartPointer<customMouseInteractorStyle> style =
    vtkSmartPointer<customMouseInteractorStyle>::New();
  renderWindowInteractor->SetInteractorStyle( style );

  renderWindowInteractor->Initialize();
  renderWindowInteractor->Start();

  std::vector<vtkIdType> ids;
  std::vector<Point> points = style->points;
  for(int i = 0; i < points.size(); i++){
    vtkIdType id = pointsVTK->InsertNextPoint ( points[i].x, points[i].y, 0 );
    ids.push_back(id);
  }

  std::vector<Segment > edges;

  PolygonTriangulation pt( points );

  pt.triangulate();

  for(int i = 0; i < points.size()-1; i++){

    edges.push_back( Segment(i, i+1) );

    vtkLine *line = vtkLine::New();
    line->GetPointIds()->SetId(0, ids[i]);
    line->GetPointIds()->SetId(1, ids[i+1]);

    cells->InsertNextCell(line);
  }

  for(int i = 0; i < pt.newSegments.size(); i++){
      vtkLine *line = vtkLine::New();
      line->GetPointIds()->SetId(0, ids[ pt.newSegments[i].idp ]);
      line->GetPointIds()->SetId(1, ids[ pt.newSegments[i].idq ]);

      cells->InsertNextCell(line);
  }

  edges.push_back( Segment(points.size()-1, 0) );

  vtkLine *line = vtkLine::New();
  line->GetPointIds()->SetId(0, ids[points.size()-1]);
  line->GetPointIds()->SetId(1, ids[0]);

  cells->InsertNextCell(line);

  std::cout << "acabe de crear edges" << std::endl;

/*
  LineIntersection li(points, edges);

  std::vector<Point> intersections = li.sweep_line();

  std::cout << "size " << intersections.size() << std::endl;
  std::vector<vtkIdType> idsIntersection;
  for(int i = 0; i < intersections.size(); i++){
      vtkIdType id = pointsVTK->InsertNextPoint
                                ( intersections[i].x, intersections[i].y, 0 );
      idsIntersection.push_back(id);
  }

*/
  pointsPolydata->SetPoints(pointsVTK);


  vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter =
    vtkSmartPointer<vtkVertexGlyphFilter>::New();

  vertexFilter->SetInputData(pointsPolydata);
  vertexFilter->Update();

   polyData =
    vtkSmartPointer<vtkPolyData>::New();
  polyData->ShallowCopy(vertexFilter->GetOutput());

  polyData->SetLines(cells);

  // Visualization
  mapper->SetInputData(polyData);

  actor =  vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetPointSize(5);

  renderer =  vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);

  renderWindow =  vtkSmartPointer<vtkRenderWindow>::New();



  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize( 600, 600 );

  renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();

    renderWindowInteractor->SetRenderWindow(renderWindow);

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
