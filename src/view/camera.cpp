# include "view/camera.hpp"

using namespace view;

Camera::Camera( std::shared_ptr< Projection > const& p )
: _projection( p )
{}


void Camera::lookAt( vec3_type const& position, vec3_type const& target, vec3_type const& up)
{
  mat33_type R;
  R.col(2) = (position-target).normalized();
  R.col(0) = up.cross(R.col(2)).normalized();
  R.col(1) = R.col(2).cross(R.col(0));
  _view_matrix.topLeftCorner<3,3>() = R.transpose();
  _view_matrix.topRightCorner<3,1>() = -R.transpose() * position;
  _view_matrix(3,3) = 1.0f;
}

