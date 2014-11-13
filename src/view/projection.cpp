# include "view/projection.hpp"

using namespace std;
using namespace ::view;

Projection::Projection( double near, double far, double aspect )
: _matrix( mat44_type::Identity() ), _matrix_dirty( true )
{
  set_near_plane( near );
  set_far_plane( far );
  set_aspect_ratio( aspect );
}


void Projection::set_aspect_ratio( double const& aspect_ratio )
{
  if( aspect_ratio <= 0 ) throw std::invalid_argument( "Aspect ratio must be positive." );
  _aspect_ratio = aspect_ratio;
  _matrix_dirty = true;
}

void Projection::set_near_plane( double const& near_plane )
{
  //if( near_plane <= 0 ) throw std::invalid_argument( "Near plane must be positive." );
  _near_plane = near_plane;
  _matrix_dirty = true;
}

void Projection::set_far_plane( double const& far_plane )
{
  //if( far_plane <= 0 ) throw std::invalid_argument( "Far plane must be positive." );
  _far_plane = far_plane;
  _matrix_dirty = true;
}

PerspectiveProjection::PerspectiveProjection( double fovy, double near, double far, double aspect )
: Projection( near, far, aspect )
{
  set_field_of_view( fovy );
}


void PerspectiveProjection::set_field_of_view( const double& fovy )
{
  if( fovy <= 0 ) throw std::invalid_argument( "Far plane must be positive." );
  _fov_y = fovy;
  _matrix_dirty = true;
}

void PerspectiveProjection::compute_matrix() const
{
  double  theta = .5 * _fov_y;
  double  range = _far_plane - _near_plane;
  double invtan = 1. / tan(theta);
  
  _matrix(0,0) = invtan / _aspect_ratio;
  _matrix(1,1) = invtan;
  _matrix(2,2) = -(_near_plane + _far_plane) / range;
  _matrix(3,2) = -1;
  _matrix(2,3) = -2 * _near_plane * _far_plane / range;
  _matrix(3,3) = 0;
}

OrthographicProjection::OrthographicProjection( double width, double height, double near, double far, double aspect )
: Projection( near, far, aspect )
{
  set_width (  width );
  set_height( height );
}

void OrthographicProjection::set_width( const double& width )
{
  if( width <= 0 ) throw std::invalid_argument( "Width must be positive." );
  _width = width;
  _matrix_dirty = true;
}

void OrthographicProjection::set_height( const double& height )
{
  if( height <= 0 ) throw std::invalid_argument( "Height must be positive." );
  _height = height;
  _matrix_dirty = true;
}

void OrthographicProjection::compute_matrix() const
{
  float range = _far_plane - _near_plane;

  _matrix(0,0) =  2. / _width;
  _matrix(1,1) =  2. / _height;
  _matrix(2,2) = -2. / ( _far_plane - _near_plane );
  _matrix(3,2) = - ( _far_plane + _near_plane ) / ( _far_plane - _near_plane );
}