# pragma once

# include <eigen3/Eigen/Core>
# include <eigen3/Eigen/Dense>

# include <cmath>
# include <string>
# include <limits>

typedef double distance_type;

template< typename T, unsigned int Dim >
using vec_type = Eigen::Matrix< T, Dim, 1 >;

typedef vec_type< double, 3 > vec3_type;

template< typename T, unsigned int Dim >
class sphere
{
  public:

    sphere() 
    : _center( 0,0,0 ) 
    , _radius( -std::numeric_limits< T >::infinity() )
    {}

    bool valid() const { return std::isfinite( _radius ); }

    vec3_type getCenter() { return _center; }
    vec3_type getRadius() { return _radius; }

    bool extend( vec_type< T, Dim > const& v )
    {
      const auto diff_norm = ( v - _center ).norm();
      if( diff_norm > _radius ) 
      { 
        _radius = diff_norm;
        return true;
      }
      return false;
    }

  private:
    vec_type< T, Dim > _center;
    T                  _radius;

}; // sphere


template< typename T, size_t Dim >
class box
{
  public:

    box() 
    : _min(   std::numeric_limits< T >::infinity() ) 
    , _max( - std::numeric_limits< T >::infinity() )
    {}

    bool valid() const 
    {
      return    std::isfinite( _min[0] ) && std::isfinite( _min[1] ) && std::isfinite( _min[2] )
             && std::isfinite( _max[0] ) && std::isfinite( _max[1] ) && std::isfinite( _max[2] );
    }

    vec3_type getMin() { return _min; }
    vec3_type getMax() { return _max; }

    bool extend( vec3_type const& v )
    {
      bool was_extended = false;
      for( unsigned int i = 0; i < Dim; ++i )
      {
        if( v[i] < _min[i] )
        {
          _min[i] = v[i];
          was_extended = true;
        }
        if( v[i] > _max[i] )
        {
          _max[i] = v[i];
          was_extended = true;
        }
      }
      return was_extended;
    }

  private:
    vec3_type _min;
    vec3_type _max;
}; // box

template< typename T >
class convex_hull_2d
{
  public:
    convex_hull_2d() = default; 

    bool valid() const { return _point_sequence.empty; }

    const std::vector< vec_type<T,2> >& point_sequence() const { return _point_sequence; }

    //bool extend( const vec_type<T,2>& p )

  private:
    std::vector< vec_type<T,2> > _point_sequence;
};
