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
Point mid;
bool primer = true;
bool Arc::operator< (const Arc& other) const {
    // this is te case of a degenerate parabola (new site found)
    if (this->sites.first == other.sites.second) {
        return (this->sites.first < other.sites.first) ;
    }
    std::cout << "voy a comparar: " ;
    std::cout << "<" << sites.first;
    std::cout << "-" << sites.second << ">";
    std::cout << " con " << std::endl;
    std::cout << "<" << other.sites.first;
    std::cout << "-" << other.sites.second << ">";
    std::cout << std::endl;
    double thisBreackPoint = getBreakpointX();
    double otherBreakPoint = other.getBreakpointX();
    std::cout << "break point: " << thisBreackPoint << " <-> ";
    std::cout << otherBreakPoint << std::endl;
    return thisBreackPoint < otherBreakPoint;
}

double  Arc::getBreakpointX() const {

    if( sites.first == sites.second ) return sites.first.x;

    double sl_2 = sweep_lineY * sweep_lineY;
    std::cout << " sl " << sweep_lineY << std::endl;
    double dy1 = sites.first.y - sweep_lineY;
    double x1_2 = sites.first.x * sites.first.y;
    double y1_2 = sites.first.y * sites.first.y;

    double a1 = 1.0 / (2.0 * dy1);
    double b1 = - sites.first.x / dy1;
    double c1 = ( x1_2 + y1_2 - sl_2 ) / ( 2.0 * dy1 );

    double dy2 = sites.second.y - sweep_lineY;
    double x2_2 = sites.second.x * sites.second.y;
    double y2_2 = sites.second.y * sites.second.y;

    double a2 =  1.0 / (2.0 * dy2);
    double b2 = - sites.second.x / dy2;
    double c2 = ( x2_2 + y2_2 - sl_2 ) / ( 2.0 * dy2 );

    double disc = ( (b1-b2)*(b1-b2) ) - ( 4.0*(a1-a2)*(c1-c2) );
    if(disc < 0) std::cout << "disc es negativo" << std::endl;
    double x1 = ( -(b1-b2) + sqrt( disc ) ) / ( 2.0 * (a1-a2) );
    double x2 = ( -(b1-b2) - sqrt( disc ) ) / ( 2.0 * (a1-a2) );

    std::cout << "x1 " << x1 << " x2 " << x2 << std::endl;
    double y1 = a1*x1*x1 + b1*x1 + c1;
    double y2 = a2*x2*x2 + b2*x2 + c2;
    if(primer) {
        mid.x = x1; mid.y = y1;
        std::cout << "mid " << mid << std::endl;
        primer = false;
    }
    if(sites.first < sites.second) {
        if( x1 < x2 ) return x2;
        else          return x1;
    } else {
        if( x1 < x2 ) return x1;
        else          return x2;
    }
}
// -------------------------------------------------------------------------
Event::Event() {}
Event::Event(Point _p, bool _isSite) : p(_p), isSite(_isSite) {}
bool Event::operator< (const Event& other) const {
    return p < other.p;
}
// -------------------------------------------------------------------------
VoronoiFilter* VoronoiFilter::New( ) {
  return( new VoronoiFilter( ) );
}

// -------------------------------------------------------------------------
VoronoiFilter::VoronoiFilter( ) : vtkPolyDataAlgorithm( ) {}

// -------------------------------------------------------------------------
VoronoiFilter::~VoronoiFilter( ) {}

// -------------------------------------------------------------------------
int VoronoiFilter::RequestData( vtkInformation* request,
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

    Arc::sweep_lineY = in_points->GetPoint( 2 )[1];

    // Add sites to eventQueue
    for( unsigned long i = 0; i < in_points->GetNumberOfPoints( ); ++i ) {
        eventQueue.push( Event( Point( in_points->GetPoint( i ) ), true ) );

        /** testing **/
        if( i == 2) continue;
        std::vector<Point> vec =  getPoints( Point( in_points->GetPoint( i ) ) );
        for(int i = 0; i < vec.size() ; i++) {
            double ppp[3] = {vec[i].x, vec[i].y, 0.0};
            out_points->InsertNextPoint( ppp );
        }
    }
    for(double i = 0; i < 600 ; i+=50) {
        out_points->InsertNextPoint( i, Arc::sweep_lineY, 0 );
    }

    if(eventQueue.size() >= 2) {
        Point p = eventQueue.top().p;
        eventQueue.pop();
        Point q = eventQueue.top().p;
        eventQueue.pop();
        Arc a(p, q);

        if(q.x < p.x) {
            a.sites.first = q;
            a.sites.second = p;
        }

        status.insert(a);

        while( !eventQueue.empty() ) {
            Event curr = eventQueue.top();
            std::cout << "curr " << curr.p << std::endl;
            Arc::sweep_lineY = curr.p.y;
            eventQueue.pop();
            if(curr.isSite) handleSiteEvent(curr.p);
            else            handleCircleEvent();
        }
    }
    double pp[3];
    std::cout << "mid " << mid << std::endl;
    pp[0] = mid.x;
    pp[1] = mid.y;
    pp[2] = 0;
    out_points->InsertNextPoint( pp );

    std::cout << "points " << out_points->GetNumberOfPoints() << std::endl;

    // Update cells
    for( unsigned int i = 0; i < out_points->GetNumberOfPoints( ); ++i )
    {
      out_verts->InsertNextCell( 1 );
      out_verts->InsertCellPoint( i );

    } // rof
    // Associate output objects
    out->SetPoints( out_points );
    out->SetLines( out_lines );
    out->SetVerts( out_verts );

    // Always return like this
    return( 1 );
}
// -------------------------------------------------------------------------
void VoronoiFilter::handleSiteEvent(Point p) {
    std::set<Arc>::iterator itHi, itLo;
    //TODO no se si esto sirva
    printBeachLine();
    Arc a (p,p);
    std::cout << "voy a hacer upper bound" << std::endl;
    itHi = status.upper_bound( a );
    std::cout << "voy a hacer lower bound" << std::endl;
    itLo = status.lower_bound( a );
    //TODO check for cirlce event

    if(itLo == status.end()) {
        std::cout << "lo: end" << std::endl;
    } else {
        std::cout << "lo: ";
        std::cout << "<" << itLo->sites.first;
        std::cout << "-" << itLo->sites.second << ">";
        std::cout << std::endl;
    }
    if(itHi == status.end()) {
        std::cout << "hi: end" << std::endl;
    } else {
        std::cout << "hi: ";
        std::cout << "<" << itHi->sites.first;
        std::cout << "-" << itHi->sites.second << ">";
        std::cout << std::endl;
    }

    printBeachLine();
}

void VoronoiFilter::handleCircleEvent() {

}

void VoronoiFilter::printBeachLine() {
    std::set<Arc>::iterator it;
    std::cout << "-----------------beach line--------------------" << std::endl;
    for(it = status.begin();  it != status.end(); it++) {
        std::cout << "<" << it->sites.first.x << ", " << it->sites.first.x;
        std::cout << "-" << it->sites.second.x << ", " << it->sites.second.x << ">";
        std::cout << " -> ";
    }
    std::cout << std::endl;
    std::cout << "------------------------------------------" << std::endl;
}


std::vector<Point> VoronoiFilter::getPoints(Point site) {
    std::cout << "entre a get points" << std::endl;
    std::vector<Point> vec;
    double sl_2 = Arc::sweep_lineY * Arc::sweep_lineY;
    std::cout << "site " << site << std::endl;
    std::cout << "sl " << Arc::sweep_lineY << std::endl;
    double dy1 = site.y - Arc::sweep_lineY;
    std::cout << "---------------->dy " << dy1 << std::endl;
    double x1_2 = site.x * site.y;
    double y1_2 = site.y * site.y;

    double a1 = 1.0 / (2.0 * dy1);
    double b1 = - site.x / dy1;
    double c1 = ( x1_2 + y1_2 - sl_2 ) / ( 2.0 * dy1 );
    std::cout << "a " << a1 << " b " << b1 << " c " << c1 << std::endl;
    for(double i = site.x-250; i < site.x+250; i+=20) {
        double y = (a1*i*i + b1*i + c1);
        /*std::cout << "al centro: ";
        std::cout << sqrt( (site.x - i)*(site.x - i) + (site.y - y)*(site.y - y) );
        std::cout << " -- a linea: " << y - Arc::sweep_lineY << std::endl;
        */
        //std::cout << "voy a agregar " << i << " " << y << std::endl;
        vec.push_back( Point(i, y) );
    }
    return vec;

}
// eof - VoronoiFilter.cxx