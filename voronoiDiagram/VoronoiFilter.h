#ifndef __VORONOIFILTER__H__
#define __VORONOIFILTER__H__

#include <vtkPolyDataAlgorithm.h>

class VoronoiFilter
  : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro( VoronoiFilter,vtkPolyDataAlgorithm );

public:
  static VoronoiFilter* New( );

protected:
  VoronoiFilter( );
  virtual ~VoronoiFilter( );

  // Usual data generation method
  virtual int RequestData(
    vtkInformation* request,
    vtkInformationVector** input,
    vtkInformationVector* output
    );

private:
  // Purposely not implemented.
  VoronoiFilter( const VoronoiFilter& other );
  void operator=( const VoronoiFilter& other );
};

#endif // __VORONOIFILTER__H__

// eof - VoronoiFilter.h
