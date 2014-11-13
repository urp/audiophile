# pragma once

# include "math.hpp"

namespace view
{

  struct Projection
  {
      Projection( double near, double far, double aspect = 1.);

      mat44_type get_matrix() const
      { 
        if( _matrix_dirty ) compute_matrix();
        _matrix_dirty = false;
        return _matrix;
      }

      void set_near_plane( double const& );
      void set_far_plane( double const& );
      void set_aspect_ratio( double const& );

    protected:
      virtual void compute_matrix() const= 0;

      mutable mat44_type _matrix;
      mutable bool _matrix_dirty;
      double _aspect_ratio;
      double _near_plane;
      double _far_plane;

  }; // Projection

  struct PerspectiveProjection : public Projection
  {
      PerspectiveProjection( double fovy, double near, double far, double aspect = 1. );
      void set_field_of_view( double const& );

    protected:
      virtual void compute_matrix() const override;
      double _fov_y;

  }; // PerspectiveProjection

  struct OrthographicProjection : public Projection
  {
      OrthographicProjection( double width, double heigth, double near, double far, double aspect = 1. );
      void set_width( double const& );
      void set_height( double const& );
    protected:
      virtual void compute_matrix() const override;
      double _width;
      double _height;
  }; // OrthographicProjection
  
} // view::