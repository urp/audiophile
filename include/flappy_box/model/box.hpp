# pragma once

# include "model/game_object.hpp"
# include "flappy_box/model/tube.hpp"
# include "math.hpp"

namespace flappy_box
{

  namespace model
  {
    class Box : public ::model::GameObject
    {
      public:

        Box( std::shared_ptr< Tube const > const&
           , double const& box_diameter = 1.
           , std::string const& = "Box" );

        double const& diameter() const { return _diameter; }
        void setDiameter( double d ) { _diameter = d; }

        double const& angle() const { return _angle; }
        void setAngle( double a ) { _angle = a; }

        vec3_type const& position() const { return _position; }
        void setPosition( vec3_type const& p ) { _position = p; }

        vec3_type const& velocity() const { return _velocity; }
        void setVelocity( vec3_type const& v ) { _velocity = v; }

        vec3_type const& acceleration() const { return _acceleration; }
        void setAcceleration( vec3_type const& a ) { _acceleration = a; }

        std::shared_ptr< Tube const > const& tube_object() const { return _tube; }
        void setTube( std::shared_ptr< Tube const > const& t ) { _tube = t; }

    private:

        double _diameter;
        double _angle;

        vec3_type _position;
        vec3_type _velocity;
        vec3_type _acceleration;

        std::shared_ptr< Tube const > _tube;

    }; // Box

  } // model::
} // flappy_box::