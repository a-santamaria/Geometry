#include "VoronoiFilter.h"

#include <vtkCellArray.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>

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
