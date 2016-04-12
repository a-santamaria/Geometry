#include "VoronoiFilter.h"

#include <vtkCellArray.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>

// -------------------------------------------------------------------------
double Arc::sweep_lineY = INF;
Arc::Arc() {}

Arc::Arc(Point first, Point second) {
    sites = std::make_pair(first, second);
}

bool Arc::operator< (const Arc& other) const {
    // this is te case of a degenerate parabola (new site found)
    if (this->sites.first == other.sites.second) {
        return (this->sites.first < other.sites.first) ;
    }
    return getBreakpoint().x < other.getBreakpoint().x;
}

Point  Arc::getBreakpoint() const {
    double sl_2 = sweep_lineY * sweep_lineY;

    double dy1 = (sites.first.y - sweep_lineY);
    double x1_2 = sites.first.x * sites.first.y;
    double y1_2 = sites.first.y * sites.first.y;

    double a1 = 0.5 * dy1;
    double b1 = - sites.first.x / dy1;
    double c1 = ( x1_2 + y1_2 - sl_2 ) / dy1;

    double dy2 = (sites.second.y - sweep_lineY);
    double x2_2 = sites.second.x * sites.second.y;
    double y2_2 = sites.second.y * sites.second.y;

    double a2 = 0.5 * dy2;
    double b2 = - sites.second.x / dy2;
    double c2 = ( x2_2 + y2_2 - sl_2 ) / dy2;

    double disc = ( (b1-b2)*(b1-b2) ) - ( 4.0*(a1-a2)*(c1-c2) );

    Point p, q;
    p.x = ( -(b1-b2) + sqrt( disc ) ) / ( 2.0 * (a1-a2) );
    q.x = ( -(b1-b2) - sqrt( disc ) ) / ( 2.0 * (a1-a2) );

    p.y = a1*p.x*p.x + b1*p.x + c1;
    q.y = a2*q.x*q.x + b2*q.x + c2;

    if(sites.first < sites.second) {
        if( p.x < q.x ) return p;
        else            return q;
    } else {
        if( p.x < q.x ) return q;
        else            return p;
    }
}

// -------------------------------------------------------------------------
VoronoiFilter* VoronoiFilter::
New( )
{
  return( new VoronoiFilter( ) );
}

// -------------------------------------------------------------------------
VoronoiFilter::
VoronoiFilter( )
  : vtkPolyDataAlgorithm( )
{
}

// -------------------------------------------------------------------------
VoronoiFilter::
~VoronoiFilter( )
{
}

// -------------------------------------------------------------------------
int VoronoiFilter::
RequestData(
  vtkInformation* request,
  vtkInformationVector** input,
  vtkInformationVector* output
  )
{
  // Get input
  vtkInformation* input_info = input[ 0 ]->GetInformationObject( 0 );
  vtkPolyData* in = vtkPolyData::SafeDownCast(
    input_info->Get( vtkDataObject::DATA_OBJECT( ) )
    );
  vtkPoints* in_points = in->GetPoints( );

  // Get output
  vtkInformation* output_info = output->GetInformationObject( 0 );
  vtkPolyData* out = vtkPolyData::SafeDownCast(
    output_info->Get( vtkDataObject::DATA_OBJECT( ) )
    );

  // Real output objects
  vtkPoints* out_points = vtkPoints::New( );
  vtkCellArray* out_lines = vtkCellArray::New( );
  vtkCellArray* out_verts = vtkCellArray::New( );

  // Dummy code
  for( unsigned long i = 0; i < in_points->GetNumberOfPoints( ); ++i )
    out_points->InsertNextPoint( in_points->GetPoint( i ) );

  for( unsigned long i = 0; i < in_points->GetNumberOfPoints( ); ++i )
  {
    out_verts->InsertNextCell( 1 );
    out_verts->InsertCellPoint( i );
    for( unsigned long j = i + 1; j < in_points->GetNumberOfPoints( ); ++j )
    {
      out_lines->InsertNextCell( 2 );
      out_lines->InsertCellPoint( i );
      out_lines->InsertCellPoint( j );

    } // rof

  } // rof

  // Associate output objects
  out->SetPoints( out_points );
  out->SetLines( out_lines );
  out->SetVerts( out_verts );

  // Always return like this
  return( 1 );
}

// eof - VoronoiFilter.cxx
