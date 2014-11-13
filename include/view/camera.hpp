# pragma once

# include "view/projection.hpp"

# include <memory>

namespace view
{

  class Camera
  {
    public:
      Camera( std::shared_ptr< Projection > const& );

      void lookAt( vec3_type const& position, vec3_type const& target, vec3_type const& up);

      void set_projection( std::shared_ptr< Projection > const& p ) { _projection = p; }

      mat44_type view_matrix() const { return _view_matrix; }

      std::shared_ptr< Projection > projection() { return _projection; }
      std::shared_ptr< Projection const > projection() const { return _projection; }

    private:

      mat44_type _view_matrix;
      std::shared_ptr< Projection > _projection;
  };

}