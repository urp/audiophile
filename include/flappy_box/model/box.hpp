# pragma once

# include "model/game_object.hpp"

namespace flappy_box
{

  namespace model
  {
    class Box : public ::model::GameObject
    {
      public:

        Box( const std::string& = "Box" );

//         virtual sphere< distance_type, 2 > getBoundingSphere() const override;
//         virtual box< distance_type, 2 >    getBoundingBox()    const override;

        const double& angle() const { return _angle; }
        void setAngle( double a ) { _angle = a; }

        const vec3_type& position() const { return _position; }
        void setPosition( const vec3_type& p ) { _position = p; }

        const vec3_type& velocity() const { return _velocity; }
        void setVelocity( const vec3_type& v ) { _velocity = v; }

      private:

        double _angle;

        vec3_type _position;
        vec3_type _velocity;

        

    }; // DefaultGameObject

  } // model::
} // audiophile::